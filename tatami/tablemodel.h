#ifndef  TATAMITABLEMODEL_H
# define TATAMITABLEMODEL_H

# include <metarecord-qt/tablemodel.h>
# include <QtQml/qqml.h>
# include "Tatami_global.h"

class TATAMI_EXPORT TatamiTableModel : public MetaRecordTableModel
{
  Q_OBJECT
  QML_NAMED_ELEMENT(TableModel)
public:
  explicit TatamiTableModel(QObject* parent = nullptr);

  Q_INVOKABLE int suggestColumnWidth(unsigned int column, const QString& fontName = "", int fontSize = -1) const;
  Q_INVOKABLE int maxWidthForProperty(const QByteArray& property, const QString& fontName = "", int fontSize = -1) const;
  Q_INVOKABLE int columnTotalWidth(const QString& fontName = "", int fontSize = -1) const;
};

#endif
