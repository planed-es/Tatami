import QtQuick 2.12

Item {
  property color  headerBackgroundColor: "#CCCCCC"
  property color  headerTextColor:       "#000000"
  property color  textColor:             "#000000"
  property color  selectionColor:        "#3E65FF"
  property color  selectionTextColor:    "#FFFFFF"
  property color  backgroundColor:       "#e2e2e2"
  property color  altBackgroundColor:    "#FFFFFF"
  property string fontHeaderName:        "Signika"
  property int    fontHeaderPointSize:   15
  property string fontName:              "Share Tech Mono"
  property int    fontPointSize:         17

  states: [
    State { name: "active" },
    State { name: "inactive" }
  ]

  transitions: [
    Transition {
      to: "active"
      ColorAnimation {
        
      }
    },
    Transition {
      to: "inactive"
    }
  ]
}
