#include "controller.h"
#include "service/filter.h"
#include "subsetservice.h"
#include "controller/multiplepicker.h"

TatamiController::TatamiController(QObject* parent) : ListNavigationComponent(parent)
{
  connect(this, &TatamiController::filtersChanged, this, [this]()
  {
    displayService().refreshSelection();
    setCurrentIndex(0);
  });
  displayService().setSelector(std::bind(&TatamiController::filter, this, std::placeholders::_1));
}

TatamiController::~TatamiController()
{
}

bool TatamiController::filter(const ModelType* value) const
{
  return filterWithVariantMap(getFilters(), value);
}

void TatamiController::useService(TatamiServiceInterface* value)
{
  if (service != value)
  {
    discardService();
    service = value;
    service->enableNotifications(withNotifications);
    displayService().setSource(service);
    connect(service, &HttpService::fetchFailure, this, &TatamiController::fetchFailure);
    connectWith(service);
  }
}

void TatamiController::discardService()
{
  if (service)
  {
    resetRequestCount();
    displayService().setSource(nullptr);
    if (service->parent() == this)
      delete service;
    else
    {
      disconnect(service, &HttpService::fetchFailure, this, &TatamiController::fetchFailure);
      disconnectFrom(service);
    }
  }
}

void TatamiController::useDefaultColumns()
{
  if (service)
  {
    ModelType* sample = service->createModel();

    getTable()->initializeColumns(*sample);
  }
  else
    qDebug() << "TatamiController::useDefaultColumns called, but service is null.";
}

TatamiMultiplePickerController* TatamiController::makeMultiplePickerController()
{
  auto* controller = new TatamiMultiplePickerController(this);
  auto* service = new SubsetService(getService(), [this](const ModelType* model) -> bool
  {
    return filter(model);
  });

  controller->useService(service);
  return controller;
}

void TatamiController::enableNotifications(bool value)
{
  qDebug() << this << "TatamiController::enableNotifications";
  if (withNotifications != value)
  {
    qDebug() << this << "TatamiController::enableNotifications -> updating" << value;
    withNotifications = value;
    if (service)
      service->enableNotifications(value);
    emit notificationsEnabledChanged();
  }
}
