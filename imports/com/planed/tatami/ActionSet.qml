import QtQuick 2.12

Item {
  property var actions: []
  property var view
  property bool backEnabled: true
  property var generateActions: null

  Component.onCompleted: {
    prepareActions();
    application.backEnabled = backEnabled;
  }

  Component.onDestruction: {
    if (view && view.viewActions === actions)
      view.viewActions = [];
    if (!backEnabled)
      application.backEnabled = true;
  }

  onEnabledChanged: {
    actions.forEach(action => {
      action.enabled = enabled;
      action.shortcut.enabled = enabled;
    });
  }

  function prepareActions() {
    if (generateActions !== null)
      actions = generateActions();
    this.attachActions();
  }

  function attachActions() {
    if (view)
      view.viewActions = actions;
  }

  enabled: view ? view.isActiveView : true
}
