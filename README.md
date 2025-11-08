# Tatami

Tatami is a small library setting up the groundwork for writing data management applications. It provides a set of tools for implementing keyboard-driven user interfaces powered by [QML](https://en.wikipedia.org/wiki/QML).

## Using Tatami in a Qt application

The QML components of the Tatami plugin must first be registered, which can typically be done within your `main` function:

```c++
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>
#include <QQuickStyle>
#include <QIcon>
#include <QDebug>
#include <tatami_plugin.h>
#include <tatami/httpclient.h>

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  QQmlApplicationEngine engine;
  TatamiPlugin tatami;

  TatamiPlugin::qmlEngine = &engine;
  tatami.registerFonts();
  tatami.registerTypes("com.planed.tatami");
  engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
  return app.exec();
}
```

This should look familiar if you've already implemented QML applications. In this example, we've taken some extra-steps, namely:

- Instantiate the `TatamiPlugin` global object;
- Initialize the `TatamiPlugin::qmlEngine` global property, using a pointer to our `QQmlApplicationEngine`;
- Call the method `registerFonts`
- Call the method `registerTypes`.

The parameter to `registerTypes` is the namespace you'll use when importing Tatami's QML types within your own QML files. The traditional namespace should be `com.planed.tatami`, but you might customize it however you please.

## Tatami QML Application

Tatami comes with an `Application` component which will manage the contextual behaviors of your user interface: application window, transition between views, breadcrumbs, and toolbar. This component also features a [StackView](https://doc.qt.io/qt-6/qml-qtquick-controls2-stackview.html) in which your applications views will be displayed.

Here's an example of what your `main.qml` file could look like, using the `Application` component:

```QML
import QtQuick 2.12
import QtQuick.Controls 2.12
import com.planed.tatami 1.0 as Tatami

Tatami.Application {
  id: application
  title: "My Tatami Application"
  initialView: "Homepage.qml"
}
```

The `Application` component extends on the [ApplicationWindow](https://doc.qt.io/qt-6/qml-qtquick-controls2-applicationwindow.html) component, meaning you can overload any property defined for that component.

The `initialView` property is an alias to the similarly named property in the application's [StackView](https://doc.qt.io/qt-6/qml-qtquick-controls2-stackview.html): use it to define which view will be loaded by default, and will be the root of all your other views.

## Tatami Views

A view should provide several properties and transitions for the `Application` component to properly display them and their contextual informations, the most important of which are:

- Name
- Icon
- [Actions](https://doc.qt.io/qt-6/qml-qtquick-controls2-action.html)

The name and icon of a view will appear in the breadcrumbs, while their actions will both be displayed in the toolbar, and made triggerable through shortcuts when the view is _active_ (meaning the view component matches the [currentItem](https://doc.qt.io/qt-6/qml-qtquick-controls2-stackview.html#currentItem-prop) property on the application's StackView).

Tatami provides a base component which your views should extend on to expose these features. Let's use it to create a `Homepage.qml` view:

```QML
import QtQuick 2.12
import com.planed.tatami 1.0 as Tatami

Tatami.View {
  viewName: "Homepage"
  viewIcon: "homepage"

  Text {
    anchors.centerIn: parent
    text: "This tatami is ready to be used."
  }
}
```

### Navigation

The `Application` component also provides three JavaScript functions which should be used to navigate between your views:

- `goToView` is used to stack a new view on top of the current one. This is the most common way to move from one view to another.

- `replaceView` is similar to `goToView`, but instead of stacking a new view on top of the current one, it will replace the current view with a new one. The transitions used will communicate this to the user by popping the current view to the right instead of the left side.

- `closeCurrentView` can be used at anytime to close the current view and move back to the previous one. It produces the same behavior as the `Back` button, which is by default always present in the toolbar.

These functions can be called from anywhere in your application, using the `id` you set for your `Application` component. In the `main.qml` we created for this example, the `id` was `application`, meaning these functions could be used as such:

```QML
import QtQuick 2.12
import QtQuick.Controls 2.12
import com.planed.tatami 1.0 as Tatami

Tatami.View {
  Button {
    anchors.centerIn: parent
    text: "Go to next view"
    onClicked: application.goToView("MyView.qml")
  }
}
```

In some cases, you will need to send parameters to the views you are loading. To that end, the `goToView` and `replaceView` functions also provide an optional parameter in which you may specify the properties of the new view you wish to customize.

For instance, let's create a `MyView.qml` file such as:

```QML
import QtQuick 2.12
import QtQuick.Controls 2.12
import com.planed.tatami 1.0 as Tatami

Tatami.View {
  property alias text: textDisplay.text

  Text {
    id: textDisplay
    text: "Default text"
    anchors.centerIn: parent
  }
}
```

We may then overload the `text` property on `textDisplay` by opening the views with more specific calls to `goToView`:

```QML
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import com.planed.tatami 1.0 as Tatami

Tatami.View {
  ColumnLayout {
    anchors.centerIn: parent
    Button {
      text: "Option 0"
      onClicked: application.goToView("MyView.qml") // No text overload
    }
    Button {
      text: "Option 1"
      onClicked: application.goToView("MyView.qml", { text: "Overloaded text" })
    }
    Button {
      text: "Option 2"
      onClicked: application.goToView("MyView.qml", { text: "Alternative text overload" })
    }
  }
}
```

### Actions

Action represents an abstract user interface action that can have shortcuts and can be assigned to menu items and toolbar buttons.

Views can expose a set of actions using the _ActionSet_ and _Action_ component: 

- The _Action_ component from Tatami inherits the [Action](https://doc.qt.io/qt-6/qml-qtquick-controls2-action.html) component from _QtQuick.Controls_.
- The _ActionSet_ component manages a set of actions, enabling or disabling them altogether. It may also be bound to a _View_ which will make the action list appear in the _toolbar_.

#### Adding actions to a view

Adding actions to a view is pretty simple: we just use the `viewActionsComponent` property, allowing us to define a custom `ActionSet` component:

```QML
import QtQuick 2.12
import com.planed.tatami 1.0 as Tatami

Tatami.View {
  viewName: "My view with actions"

  Text {
    id: viewText
    anchors.centerIn: parent
    text: "No action were triggered."
  }

  viewActionsComponent: Component {
    Tatami.ActionSet {
      actions: [action1, action2]

      Tatami.Action {
        id: action1
        text: "Action 1"
        onTriggered: viewText.text = "Action #1 was triggered"
      }

      Tatami.Action {
        id: action2
        text: "Action 2"
        onTriggered: viewText.text = "Action #2 was triggered"
      }
    }
  }
}
```

#### Swapping action sets within a view

In some cases, you may want to make different actions available to your user depending on your own view's context. This can be achieved by declaring multiple `ActionSet` components and updating the `viewActionsComponent` property at runtime:

```QML
import QtQuick 2.12
import com.planed.tatami 1.0 as Tatami

Tatami.View {
  viewName: "My view with multiple action sets"

  Text {
    id: viewText
    anchors.centerIn: parent
    text: "No action were triggered."
  }

  // Defining a property determining which ActionSet component should be active
  property int actionState: 0

  // Declaring a bunch of ActionSet
  Component {
    id: defaultActionSet
    Tatami.ActionSet {
      actions: [action0]
      Tatami.Action {
        id: action0
        text: "Start"
        onTriggered: actionState = 1
      }
    }
  }

  Component {
    id: altActionSet1
    Tatami.ActionSet {
      actions: [action1]
      Tatami.Action {
        id: action1
        text: "Action 1"
        onTriggered: {
          viewText.text = "Action #1 was triggered";
          actionState = 2;
        }
      }
    }
  }

  Component {
    id: altActionSet2
    Tatami.ActionSet {
      actions: [action2]
      Tatami.Action {
        id: action2
        text: "Action 2"
        onTriggered: {
          viewText.text = "Action #2 was triggered";
          actionState = 1;
        }
      }
    }
  }

  // Picking the current action set based on the current value of `actionState`
  viewActionsComponent: pickActionSet(actionState)

  function pickActionSet(state) {
    if (state == 0)
      return defaultActionSet;
    return state == 1 ? altActionSet1 : altActionSet2;
  }
}
```

Such a system allows different set of actions to be made available to the user in the same view. Furthermore, declaring the `ActionSet` within a `Component` ensures that the action sets aren't always instantiated within the view: instead, only the component currently assigned to the `viewActionsComponent` property is instantiated. This protects you against conflicting shortcuts from multiple action sets.

### Menus

Tatami also comes with a couple of helper components to help you quickly build menus. Let's create a simple menu view by refactoring our menu from the _Navigation_ tutorial:

```QML
import QtQuick 2.12
import com.planed.tatami 1.0 as Tatami

Tatami.Menu {
  viewName: "My menu"
  viewIcon: "start-here"

  Tatami.MenuEntry {
    text: "Option 0"
    icon.name: "start-here"
    onTriggered: goToView("MyView.qml")
  }

  Tatami.MenuEntry {
    text: "Option 1"
    icon.name: "start-here"
    onTriggered: goToView("MyView.qml", { text: "Overloaded text" })
  }

  Tatami.MenuEntry {
    text: "Option 2"
    icon.name: "start-here"
    onTriggered: goToView("MyView.qml", { text: "Alternative text overload" })
  }
}
```

Note that we are not calling `goToView` from the `application` object anymore. That's because `Menu` comes with its own implementation for `goToView`, implementing different behavior depending on whether the state of the Menu view is _active_ or _stacked_.

## Services

### Setting the global application font

```c++
#include <QApplication>
#include <tatami_plugin.h>

void setDefaultApplicationFont(QApplication& app)
{
  int defaultFontSize = 18;
  QFont defaultFont(Tatami::defaultFont, defaultFontSize);

  app.setFont(defaultFont);
}
```

### Adjusting text font size to fit a certain width

Sometimes, it can be very painful to fit a text in an element of predefined width. Tatami comes with the `AdaptiveFontSize` component, which helps you find the best font width, within a given range, to fit within a given width. Here's how to use it:

```qml
import QtQuick
import com.planed.tatami as Tatami

TextField {
  id: root
  font.pixelSize: adaptiveFontSize.fontPixelSize

  Tatami.AdaptiveFontSize {
    id: adaptiveFontSize
    text: root.text
    fontName: root.font.family
    width: root.width - root.leftPadding - root.rightPadding
    maxSize: 20 // Use pixel sizes to define the range,
    minSize: 8  // default minSize is 1
  }
}
```
