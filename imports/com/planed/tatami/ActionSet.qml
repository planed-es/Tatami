import QtQuick 2.12

Item {
  id: self
  property var actions: []
  property var view
  property bool backEnabled: true
  property var generateActions: null

  signal actionTriggered(QtObject action);

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
    try {
      if (generateActions !== null)
        actions = generateActions();
      actions.forEach(action => { action.actionSet = self; });
    } catch (err) {
      console.log("Tatami.ActionSet.prepareActions: ", err);
    }
    self.attachActions();
  }

  function attachActions() {
    if (view)
      view.viewActions = actions;
  }

  Repeater {
    model: self.actions
    delegate: Item {
      Connections {
        target: self.actions[index]
        function onTriggered() {
          self.actionTriggered(self.actions[index]);
        }
      }
    }
  }

  enabled: view ? view.isActiveView : true
}
