#include <QDebug>
#include "edit_controller.h"
#include "jsvaluetofunction.h"

std::function<void()> jsValueToCallback(QJSValue callback)
{
  if (callback.isCallable())
  {
    return [callback]()
    {
      QJSValue mutable_callback(callback);

      if (callback.isCallable())
        mutable_callback.call();
    };
  }
  return std::function<void()>();
}

TatamiEditController::TatamiEditController(QObject *parent) : TatamiController(parent)
{
  connect(this, &TatamiEditController::sampleChanged, this, &TatamiEditController::connectSample);
  connect(&displayService(), &DisplayService::invalidModel, this, &TatamiEditController::invalidModel);
}

void TatamiEditController::connectSample()
{
  if (sample)
  {
    connect(sample, &MetaRecordNotifiable::attributeChanged, this, &TatamiEditController::sampleAttributesChanged);
    decorateSample(sample);
  }
}

TatamiController::ModelType* TatamiEditController::startCreation()
{
  auto* result = displayService().startCreation();

  if (result)
  {
    sample = result;
    setCurrentItem(sample);
    emit sampleChanged();
    getTable()->setState(TatamiTableModel::CreateState);
  }
  return sample;
}

TatamiController::ModelType* TatamiEditController::startEdition(const ModelType* editedModel)
{
  if (betweenStates)
    return nullptr;
  if (editedModel && displayService().values().indexOf(const_cast<ModelType*>(editedModel)) >= 0)
  {
    sample = displayService().startEdition(*editedModel);

    setCurrentItem(editedModel);
    emit sampleChanged();
    getTable()->setState(TatamiTableModel::UpdateState);
    return sample;
  }
  return nullptr;
}

void TatamiEditController::endEdition(bool save, QJSValue jsCallback)
{
  auto callback = std::bind(&TatamiEditController::callbackWithIndexUpdate, this, currentIndex(), jsCallback);

  if (betweenStates)
    return ;
  betweenStates = true;
  displayService().endEdition(save, [this, callback]()
  {
    betweenStates = false;
    if (sample)
      disconnect(sample, &MetaRecordNotifiable::attributeChanged, this, &TatamiEditController::sampleAttributesChanged);
    sample = nullptr;
    getTable()->setState(TatamiTableModel::ReadState);
    emit sampleChanged();
    callback();
  });
}

void TatamiEditController::remove(ModelType* model, QJSValue jsCallback)
{
  auto callback = std::bind(&TatamiEditController::callbackWithIndexUpdate, this, currentIndex(), jsCallback);

  displayService().destroy(model, callback);
}

void TatamiEditController::callbackWithIndexUpdate(int index, QJSValue jsCallback)
{
  auto callback = jsValueToCallback(jsCallback);

  if (count() <= index) setCurrentIndex(count() - 1);
  else                  setCurrentIndex(index);
  if (callback) callback();
}
