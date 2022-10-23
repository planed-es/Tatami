#include "icons.h"
#include <QtGui/QIcon>
#include <QFileInfo>
#include <QDebug>

static const QStringList extensions{"png", "svg", "ico"};

QUrl TatamiIcons::url(const QString& name, QSize preferredSize)
{
  QIcon icon = QIcon::fromTheme(name);

  if (!icon.isNull())
  {
    auto sizes = icon.availableSizes();
    std::sort(sizes.begin(), sizes.end(), [](QSize a, QSize b) { return a.width() * a.height() > b.width() * b.height(); });
    auto size = sizes.contains(preferredSize) ? preferredSize : sizes.first();
    auto iconFolder = QIcon::themeName() + '/' + QString::number(size.width()) + 'x' + QString::number(size.height());

    for (const QString& searchPath : QIcon::themeSearchPaths())
    {
      for (const QString& extension : extensions)
      {
        QString filepath(searchPath + '/' + iconFolder + '/' + name + '.' + extension);
        if (QFileInfo::exists(filepath))
        {
          if (searchPath[0] == ':')
            return "qrc" + filepath;
          return filepath;
        }
      }
    }
  }
  else
    qDebug() << "TatamiIcons: icon" << name << "not found";
  return QUrl();
}
