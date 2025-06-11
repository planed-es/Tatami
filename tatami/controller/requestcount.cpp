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

  if (globalState)
  {
    while (ownedPendingRequestCount > 0)
    {
      ownedPendingRequestCount--;
      globalState->decrementPendingRequestCount();
    }
  }
}

void RequestCountComponent::connectWith(RequestCountComponent* other)
{
  connect(other, &RequestCountComponent::requestStarted, this, &RequestCountComponent::watchedRequestStarted);
  connect(other, &RequestCountComponent::requestEnded,   this, &RequestCountComponent::watchedRequestEnded);
}

void RequestCountComponent::disconnectFrom(RequestCountComponent* other)
{
  disconnect(other, &RequestCountComponent::requestStarted, this, &RequestCountComponent::watchedRequestStarted);
  disconnect(other, &RequestCountComponent::requestEnded,   this, &RequestCountComponent::watchedRequestEnded);
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

void RequestCountComponent::watchRequest(HttpClient::ResponseObject* reply)
{
  if (!reply->isFinished())
  {
    connect(reply, &HttpClient::ResponseObject::finished, this, &RequestCountComponent::onRequestEnded);
    onRequestStarted();
  }
}

void RequestCountComponent::watchedRequestStarted()
{
  pendingRequestCount++;
  emit requestStarted();
}

void RequestCountComponent::watchedRequestEnded()
{
  if (pendingRequestCount > 0)
    pendingRequestCount--;
  emit requestEnded();
}

void RequestCountComponent::onRequestStarted()
{
  TatamiGlobalState* globalState = TatamiGlobalState::get();

  ownedPendingRequestCount++;
  if (globalState)
    globalState->incrementPendingRequestCount();
  watchedRequestStarted();
}

void RequestCountComponent::onRequestEnded()
{
  TatamiGlobalState* globalState = TatamiGlobalState::get();

  if (ownedPendingRequestCount > 0)
    ownedPendingRequestCount--;
  if (globalState)
    globalState->decrementPendingRequestCount();
  watchedRequestEnded();
}
