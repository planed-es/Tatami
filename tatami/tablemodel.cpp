#include "tablemodel.h"
#include "../tatami_plugin.h"
#include <QtGui/QFont>
#include <QtGui/QFontMetrics>

TatamiTableModel::TatamiTableModel(QObject* parent) : MetaRecordTableModel(parent)
{
  javascriptEngine = TatamiPlugin::qmlEngine();
}

int TatamiTableModel::maxWidthForProperty(const QByteArray& propertyName, const QString& fontName, int fontSize) const
{
  int columnIndex = getColumnIndex(propertyName);

  if (columnIndex >= 0)
  {
    const Column& column = columns[columnIndex];
    QFont         font = fontName.isEmpty() ? QFont() : QFont(fontName, fontSize);
    QFontMetrics  metrics(font);
    int           maxLength = metrics.horizontalAdvance(column.label);

    if (column.enforcedWidth)
      return static_cast<int>(column.enforcedWidth);
    for (int row = 0 ; row < list.size() ; ++row)
    {
      QModelIndex index  = createIndex(row, columnIndex);
      QVariant    value  = data(index);
      int         length = metrics.horizontalAdvance(value.toString());

      if (length > maxLength)
        maxLength = length;
    }
    return maxLength;
  }
  return 0;
}

int TatamiTableModel::suggestColumnWidth(unsigned int column, const QString& fontName, int fontSize) const
{
  int maxLength = 0;

  if (column < columns.count())
  {
    QByteArray propertyName = columns[column].property;

    return maxWidthForProperty(propertyName, fontName, fontSize);
  }
  return maxLength;
}

int TatamiTableModel::columnTotalWidth(const QString& fontName, int fontSize) const
{
  int        width = 0;

  for (const Column& column : columns)
    width += maxWidthForProperty(column.property, fontName, fontSize);
  return width;
}
