#ifndef  TATAMI_ICONS_H
# define TATAMI_ICONS_H

# include <QtQml/qqml.h>
# include <QSize>
# include <QUrl>

class TatamiIcons : public QObject
{
  Q_OBJECT
  QML_NAMED_ELEMENT(Icons)
  QML_SINGLETON
public:
  TatamiIcons(QObject* parent = nullptr) : QObject(parent) {}

  Q_INVOKABLE QUrl url(const QString& name, unsigned short size) { return url(name, QSize(size, size)); }
  Q_INVOKABLE QUrl url(const QString& name, QSize preferredSize);
};

#endif
