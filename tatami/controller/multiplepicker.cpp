#include "multiplepicker.h"

TatamiMultiplePickerController::TatamiMultiplePickerController(QObject* parent) : TatamiEditController(parent)
{
  connect(this, &TatamiMultiplePickerController::valueChanged, &displayService(), &SubsetService::refreshSelection);
}

bool TatamiMultiplePickerController::filter(const ModelType* model) const
{
  return model && value.indexOf(model->getUid()) < 0 && model->compare(getFilters());
}
