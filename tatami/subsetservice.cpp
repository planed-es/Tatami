#include "subsetservice.h"
#include "service/filter.h"
#define SOURCE_SIGNALS(command) \
  command(source, &ModelStore::modelsChanged, this, &SubsetService::onSourceChanged); \
  command(source, &ModelStore::modelAdded,    this, &SubsetService::onModelAdded); \
  command(source, &ModelStore::modelRemoved,  this, &SubsetService::onModelRemoved); \
  command(source, &ModelStore::modelSaved,    this, &SubsetService::onModelSaved); \
  command(source, &HttpService::fetchFailure, this, &SubsetService::fetchFailure); \
  command(source, &HttpService::postFailure,  this, &SubsetService::postFailure);

SubsetService::SubsetService(TatamiServiceInterface* _source, std::function<bool(const ModelType*)> selector) :
  TatamiServiceInterface(_source), source(_source), selector(selector)
{
  connect(this, &SubsetService::refreshSelection, this, &SubsetService::onSourceChanged);
  connectToSource();
}

SubsetService::SubsetService(QObject* parent) : TatamiServiceInterface(parent), source(nullptr)
{
  connect(this, &SubsetService::refreshSelection, this, &SubsetService::onSourceChanged);
}

void SubsetService::connectToSource()
{
  if (source)
  {
    SOURCE_SIGNALS(connect)
  }
  onSourceChanged();
}

void SubsetService::disconnectFromSource()
{
  SOURCE_SIGNALS(disconnect)
}

void SubsetService::save(ModelType* model, std::function<void()> callback)
{
  qDebug() << this << "SubsetService forwarding save to its source" << source;
  if (source)
    source->save(model, callback);
}

void SubsetService::destroy(ModelType* model, std::function<void ()> callback)
{
  if (source)
    source->destroy(model, callback);
}

void SubsetService::loadFromJson(const QJsonObject& json)
{
  if (source)
    source->loadFromJson(json);
}

void SubsetService::onSourceChanged()
{
  reloadModelsFromSource();
  emit modelsChanged();
}

void SubsetService::reloadModelsFromSource()
{
  models.clear();
  if (source)
  {
    const auto list = source->filter(selector);

    for (auto* model : list)
      models.insert(model->getUid(), model);
  }
}

void SubsetService::onModelAdded(ModelType* model)
{
  if (selector(model))
  {
    models.insert(model->getUid(), model);
    emit modelAdded(model);
  }
}

void SubsetService::onModelRemoved(ModelType* model)
{
  auto it = models.find(model->getUid());

  if (it != models.end())
  {
    models.erase(it);
    emit modelRemoved(model);
  }
}

void SubsetService::onModelSaved(ModelType* model)
{
  if (selector(model))
  {
    auto it = models.find(model->getUid());

    if (it != models.end())
      emit modelSaved(model);
    else
    {
      models.insert(model->getUid(), model);
      emit modelAdded(model);
    }
  }
  else
    onModelRemoved(model);
}

void SubsetService::setSource(TatamiServiceInterface* p_source)
{
  if (source)
    disconnectFromSource();
  source = p_source;
  connectToSource();
}

void SubsetService::setSelector(QVariantMap filters)
{
  selector = std::bind(&filterWithVariantMap, filters, std::placeholders::_1);
  onSourceChanged();
}

void SubsetService::setSelector(std::function<bool (const ModelType*)> p_selector)
{
  selector = p_selector;
  onSourceChanged();
}
