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

### Menus

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
