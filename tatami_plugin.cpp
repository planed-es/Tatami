#include "tatami_plugin.h"
#include "tatami/tablemodel.h"
#include <qqml.h>

void TatamiPlugin::registerTypes(const char *uri)
{
  // @uri com.planed.tatami
  qmlRegisterType<TatamiTableModel>(uri, 1, 0, "TableModel");

  qRegisterMetaType<MetaRecordable*>("const MetaRecordable*");
  qRegisterMetaType<MetaRecordable*>("MetaRecordable*");
}
