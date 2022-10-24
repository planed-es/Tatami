#ifndef TATAMI_PLUGIN_H
#define TATAMI_PLUGIN_H

#include <QQmlExtensionPlugin>

class QJSEngine;

class TatamiPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)
public:
    static const QString defaultFont;
    static QJSEngine* qmlEngine();

    void registerFonts();
    void registerTypes(const char *uri) override;
};

#endif // TATAMI_PLUGIN_H
