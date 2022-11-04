import QtQuick 2.12

Item {
  id: self
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

  function prepareActions() {
    if (generateActions !== null)
      actions = generateActions();
    actions.forEach(action => { action.actionSet = self; });
    self.attachActions();
  }

  function attachActions() {
    if (view)
      view.viewActions = actions;
  }

  enabled: view ? view.isActiveView : true
}
