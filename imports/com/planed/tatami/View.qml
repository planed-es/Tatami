import QtQuick 2.12
import QtQuick.Controls 2.12 as QQControls
import com.planed.tatami 1.0

QQControls.Pane {
  id: root
  property bool isActiveView: true
  property bool isAlreadyActive: true
  property var previousView: null
  property var viewActions: []
  property string viewName: ""
  property string viewIcon: ""
  property string viewPicto: viewIcon
  property string viewScope: ""
  property bool isViewStackable: false
  property Component stackState
  enabled: isActiveView
  state: "popped"

  property alias viewActionsComponent: viewActionLoader.sourceComponent
  property alias viewActionsItem: viewActionLoader.item
  property var actionBackup: null

  signal viewActivated()
  signal viewDeactivated()

  onIsActiveViewChanged: {
    if (isAlreadyActive !== isActiveView) {
      isAlreadyActive = isActiveView;
      if (isActiveView) {
        root.viewActionsComponent = root.actionBackup || root.viewActionsComponent;
        root.viewActionsComponent.enabled = true;
        root.actionBackup = null;
        viewActivated();
      }
      else {
        root.actionBackup = root.viewActionsComponent;
        root.actionBackup.enabled = false;
        root.viewActionsComponent = null;
        viewDeactivated();
      }
    }
  }

  background: Rectangle {
    color: "white"

    Image {
      anchors { bottom: parent.bottom; right: parent.right; margins: 10 }
      width: 256
      height: 256
      source: Icons.url(viewPicto, 256)
      opacity: 0.5
    }
  }

  Loader {
    id: viewActionLoader
  }

  states: [
    State { name: "stacked" },
    State { name: "deployed" },
    State { name: "popped" },
    State { name: "root-deployed" }
  ]

  transitions: [
    Transition {
      from: "deployed"
      to: "stacked"
      NumberAnimation {
        target: root; property: "x"; duration: application.transitionDuration;
        from: previousView.isViewStackable ? application.viewOffset : 0
        to: 0
      }
      NumberAnimation {
        target: root; property: "width"; duration: application.transitionDuration;
        from: previousView.isViewStackable ? application.viewWidth : application.width
        to: isViewStackable ? application.viewOffset : 0
      }
      PropertyAction {
        target: root; property: "isActiveView"; value: false
      }
    },
    Transition {
      from: "root-deployed"
      to: "stacked"
      NumberAnimation {
        target: root; property: "width"; duration: application.transitionDuration;
        from: application.width; to: application.viewOffset;
      }
      PropertyAction {
        target: root; property: "isActiveView"; value: false
      }
    },
    Transition {
      from: "stacked"
      to: "deployed"
      NumberAnimation {
        target: root; property: "x"; duration: application.transitionDuration;
        from: 0
        to: previousView.isViewStackable ? application.viewOffset : 0
      }
      NumberAnimation {
        target: root; property: "width"; duration: application.transitionDuration;
        from: isViewStackable ? application.viewOffset : 0
        to: previousView.isViewStackable ? application.viewWidth : application.width
      }
      PropertyAction {
        target: root; property: "isActiveView"; value: true
      }
    },
    Transition {
      from: "stacked"
      to: "root-deployed"
      NumberAnimation {
        target: root; property: "width"; duration: application.transitionDuration;
        from: application.viewOffset; to: application.width;
      }
      PropertyAction {
        target: root; property: "isActiveView"; value: true
      }
    },
    Transition {
      from: "deployed"
      to: "popped"

      NumberAnimation {
        target: root; property: "x"; duration: application.transitionDuration
        from: previousView.isViewStackable ? application.viewOffset : 0;
        to: application.width
        //easing.type: Easing.InOutQuad
      }
      PropertyAction {
        target: root; property: "isActiveView"; value: false
      }
    },
    Transition {
      from: "popped"
      to: "deployed"

      NumberAnimation {
        target: root; property: "x"; duration: application.transitionDuration
        from: application.width
        to: previousView.isViewStackable ? application.viewOffset : 0
      }
      NumberAnimation {
        target: root; property: "width"; duration: application.transitionDuration
        from: application.width
        to: previousView.isViewStackable ? application.viewWidth : application.width
      }
      PropertyAction {
        target: root; property: "isActiveView"; value: true
      }
    }
  ]
}
