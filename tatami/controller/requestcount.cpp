#include "requestcount.h"
#include "globals.h"
#include "../service/httpservice.h"

RequestCountComponent::~RequestCountComponent()
{
  resetRequestCount();
}

void RequestCountComponent::resetRequestCount()
{
  TatamiGlobalState* globalState = TatamiGlobalState::get();

  while (pendingRequestCount > 0)
  {
    pendingRequestCount--;
    if (globalState)
      globalState->decrementPendingRequestCount();
  }
}

void RequestCountComponent::connectWith(HttpService* service)
{
  connect(service, &HttpService::requestStarted, this, &RequestCountComponent::onRequestStarted);
  connect(service, &HttpService::requestEnded,   this, &RequestCountComponent::onRequestEnded);
}

void RequestCountComponent::disconnectFrom(HttpService* service)
{
  disconnect(service, &HttpService::requestStarted, this, &RequestCountComponent::onRequestStarted);
  disconnect(service, &HttpService::requestEnded,   this, &RequestCountComponent::onRequestEnded);
}

void RequestCountComponent::onRequestStarted()
{
  TatamiGlobalState* globalState = TatamiGlobalState::get();

  pendingRequestCount++;
  if (globalState)
    globalState->incrementPendingRequestCount();
  emit requestStarted();
}

void RequestCountComponent::onRequestEnded()
{
  TatamiGlobalState* globalState = TatamiGlobalState::get();

  if (pendingRequestCount > 0)
    pendingRequestCount--;
  if (globalState)
    globalState->decrementPendingRequestCount();
  emit requestEnded();
}
