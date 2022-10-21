#include "tatami_plugin.h"
#include "tatami/tablemodel.h"
#include "tatami/controller.h"
#include "tatami/edit_controller.h"
#include <qqml.h>

void TatamiPlugin::registerTypes(const char *uri)
{
  // @uri com.planed.tatami
  qmlRegisterType<TatamiTableModel>(uri, 1, 0, "TableModel");

  qRegisterMetaType<MetaRecordable*>("const MetaRecordable*");
  qRegisterMetaType<MetaRecordable*>("MetaRecordable*");
  qRegisterMetaType<TatamiController*>("const TatamiController*");
  qRegisterMetaType<TatamiController*>("TatamiController*");
  qRegisterMetaType<TatamiEditController*>("const TatamiEditController*");
  qRegisterMetaType<TatamiEditController*>("TatamiEditController*");
}
