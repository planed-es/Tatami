import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import com.planed.tatami 1.0 as Tatami

View {
  property var actions: loadActions()
  id: root
  enabled: true
  isViewStackable: true

  function goToPage(pageName) {
    goToView(pageName + "/Index");
  }

  function goToView(viewPath, options = {}) {
    if (!isActiveView)
      application.replaceView(viewPath, options);
    else
      application.goToView(viewPath, options);
  }

  function isActionDown(action, currentView) {
    return currentView ? action.text === currentView.viewName : false;
  }

  function loadActions() {
    const actions = [];

    for (var i = 0 ; i < contentData.length ; ++i) {
      const item = contentData[i];
      if (item.text && typeof item.trigger == "function") {
        try { item.parentView = root; } catch (err) {}
        actions.push(item);
      }
    }
    return actions;
  }

  Component {
    id: menuEntryIcon
    Item {
      property var entryAction: action
      implicitHeight: 64
      implicitWidth: 64
      Button {
        width: menuList.width
        height: parent.height
        down: isActionDown(entryAction, application.currentView)
        onClicked: entryAction.trigger()
      }
      Image {
        source: Tatami.Icons.url(action.icon.name, 48)
        width: 48
        height: width
        anchors.centerIn: parent
      }
      MouseArea {
        anchors.fill: parent
        onClicked: action.trigger()
      }
    }
  }

  Component {
    id: menuEntryText
    Label {
      horizontalAlignment: Qt.AlignHCenter
      verticalAlignment: Qt.AlignVCenter
      text: action.text.charAt(0).toUpperCase() + action.text.slice(1)
      font.family: "Signika"
      font.pointSize: 18
      MouseArea {
        anchors.fill: parent
        onClicked: action.trigger()
      }
    }
  }

  Component {
    id: menuEntryShortcut
    Label {
      rightPadding: 30
      text: action.shortcut.enabled ? action.shortcut.nativeText : ""
      font.family: "Share Tech Mono"
      font.pointSize: 18
      MouseArea {
        anchors.fill: parent
        onClicked: action.trigger()
      }
    }
  }

  Flickable {
    id: flickable
    anchors.fill: parent
    contentHeight: Math.max(height, menuList.height)
    clip: true

    ScrollBar.vertical: ScrollBar {}

    GridLayout {
      id: menuList
      columns: 3
      width: parent.width
      rowSpacing: 1
      clip: true

      Repeater {
        model: root.actions.length * 3
        delegate: Loader {
          property var action: root.actions[Math.floor(index / 3)];

          Layout.fillWidth: index % 3 == 1
          sourceComponent: {
            switch (index % 3) {
            case 0: return menuEntryIcon;
            case 1: return menuEntryText;
            case 2: return menuEntryShortcut;
            }
          }
        }
      }
    }
  }
}
