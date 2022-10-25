#include "tatami_plugin.h"
#include "tatami/tablemodel.h"
#include "tatami/controller.h"
#include "tatami/edit_controller.h"
#include <qqml.h>
#include <QtGui/QFontDatabase>

const QString TatamiPlugin::defaultFont = "Signika";
QJSEngine* TatamiPlugin::qmlEngine = nullptr;

void TatamiPlugin::registerFonts()
{
  QFontDatabase::addApplicationFont(":/com/planed/tatami/fonts/ShareTechMono-Regular.ttf");
  QFontDatabase::addApplicationFont(":/com/planed/tatami/fonts/signika.ttf");
}

void TatamiPlugin::registerTypes(const char *uri)
{
  // @uri com.planed.tatami
  qRegisterMetaType<MetaRecordable*>("const MetaRecordable*");
  qRegisterMetaType<MetaRecordable*>("MetaRecordable*");
  qRegisterMetaType<TatamiController*>("const TatamiController*");
  qRegisterMetaType<TatamiController*>("TatamiController*");
  qRegisterMetaType<TatamiEditController*>("const TatamiEditController*");
  qRegisterMetaType<TatamiEditController*>("TatamiEditController*");
}
