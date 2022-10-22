import QtQuick 2.12
import QtQuick.Controls 2.12 as QQControls
import "." as Tatami

Tatami.Window {
  id: application

  property alias backEnabled: toolBar.backEnabled
  property alias stackView: mainView
  property alias toolBar: toolBar
  property var modal: null
  property int transitionDuration: 250
  property real viewOffset: {
    if (application.width >= 800)
      return 300;
    return 0;
  }
  property real viewWidth: {
    if (application.width >= 800)
      return application.width - viewOffset;
    return application.width;
  }
  property var views: []
  property var contextActions: []
  property var currentView
  property string initialView

  function goToPage(pageName) {
    console.log("Page", pageName);
    while (mainView.depth > 1)
      closeCurrentView();
    goToView(`${pageName}/Index`);
  }

  function goToView(viewName, properties = {}) {
    properties.previousView = mainView.currentItem;
    mainView.currentItem.state = "stacked";
    mainView.push(`qrc:/${viewName}.qml`, properties);
    mainView.currentItem.state = "deployed";
    views.push(mainView.currentItem);
  }

  function replaceView(viewName, properties = {}) {
    properties.previousView = mainView.currentItem.previousView;
    mainView.currentItem.state = "popped";
    goToViewTimer.callback = function() {
      mainView.pop();
      mainView.push(`qrc:/${viewName}.qml`, properties);
      mainView.currentItem.state = "deployed";
      views.pop();
      views.push(mainView.currentItem);
    }
    goToViewTimer.running = true;
  }

  Timer {
    property var callback
    id: goToViewTimer
    interval: application.transitionDuration
    onTriggered: {
      callback(mainView.currentItem);
      callback = undefined;
    }
  }

  function closeCurrentView() {
    if (mainView.currentItem !== mainView.initialItem) {
      mainView.currentItem.state = "popped";
      views.pop();
      mainView.pop();
      mainView.currentItem.state = mainView.depth > 1 ? "deployed" : "root-deployed";
    }
  }

  onWidthChanged: resizeTimer.running = true
  onHeightChanged: resizeTimer.running = true

  Timer {
    id: resizeTimer
    interval: 50
    onTriggered: {
      views.forEach(view => {
        var backup = view.state;
        view.state = backup === "deployed" ? "popped" : "deployed";
        view.state = backup;
      });
    }
  }

  header: ToolBar {
    id: toolBar
    viewActions: mainView.currentItem.viewActions
    viewDepth: mainView.depth
  }

  Breadcrumbs {
    id: breadcrumbsRow
    stackView: mainView
    enabled: backEnabled
    anchors { left: parent.left; top: parent.top; right: parent.right }
    onDepthClicked: {
      while (mainView.depth > depth + 1)
        toolBar.exitAction.trigger();
    }
  }

  QQControls.StackView {
    id: mainView
    property real offset: 300
    property int transitionDuration: 400
    property int altDepth: 0
    anchors { left: parent.left; top: breadcrumbsRow.bottom; right: parent.right; bottom: parent.bottom }
    state: "root"
    initialItem: `qrc:/${application.initialView}`
    onCurrentItemChanged: {
      for (var i = Math.max(0, mainView.depth - 2) ; i < mainView.depth ; ++i)
        mainView.get(i).visible = true;
      transitionTimer.running = true;
      application.currentView = currentItem;
    }

    Timer {
      id: transitionTimer
      interval: mainView.transitionDuration + 1
      onTriggered: {
        for (var i = 0 ; i < mainView.depth ; ++i)
          mainView.get(i).visible = i >= mainView.depth - 2;
        mainView.altDepth = mainView.depth;
        mainView.currentItem.isActiveViewChanged();
      }
    }

    Transition {
      id: emptyTransition
    }

    pushEnter: emptyTransition
    popExit:   emptyTransition
    pushExit:  emptyTransition
    popEnter:  emptyTransition
  } // END StackView

  Rectangle {
    id: dialogOverlay
    visible: application.modal
    color: Qt.rgba(0, 0, 0, 0.3)
    anchors { top: header.bottom; left: parent.left; bottom: parent.bottom; right: parent.right }
    MouseArea {
      enabled: application.modal
      anchors.fill: parent
    }
  }
}
