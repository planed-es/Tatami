#include "metaservice.h"

#define SOURCE_SIGNALS(command, source) \
  connect(source, &QObject::destroyed, this, [this, source]() { removeSource(source); }); \
  command(source, &ModelStore::modelsChanged, this, &MetaService::onSourceChanged); \
  command(source, &ModelStore::modelAdded,    this, &MetaService::onModelAdded);    \
  command(source, &ModelStore::modelRemoved,  this, &MetaService::onModelRemoved);  \
  command(source, &ModelStore::modelSaved,    this, &MetaService::onModelSaved);    \
  if (auto* http = qobject_cast<HttpService*>(source)) {                            \
    command(http, &HttpService::fetchFailure, this, &MetaService::fetchFailure);    \
    command(http, &HttpService::postFailure,  this, &MetaService::postFailure);     \
  }

MetaService::MetaService(QObject* parent) : TatamiServiceInterface(parent)
{
  connect(this, &MetaService::refreshAll, this, &MetaService::onSourceChanged);
}

TatamiServiceInterface* MetaService::sourceFor(const ModelType* model) const
{
  for (TatamiServiceInterface* service : sources)
  {
    if (service->contains(model))
      return service;
  }
  return nullptr;
}

void MetaService::addSource(TatamiServiceInterface* source)
{
  if (source && !sources.contains(source))
  {
    bool newModels = false;

    sources.push_back(source);
    connectToSource(source);
    for (ModelType* model : source->values())
    {
      if (!contains(model))
      {
        models.insert(model->getUid(), model);
        newModels = true;
        emit modelAdded(model);
      }
    }
    if (newModels)
      emit modelsChanged();
  }
}

void MetaService::removeSource(TatamiServiceInterface* source)
{
  if (source && sources.contains(source))
  {
    bool removedModels = false;

    sources.removeOne(source);
    disconnectFromSource(source);
    for (ModelType* model : source->values())
    {
      if (sourceFor(model) == nullptr)
      {
        models.remove(model->getUid());
        removedModels = true;
        emit modelRemoved(model);
      }
    }
    if (removedModels)
      emit modelsChanged();
  }
}

void MetaService::save(ModelType* model, std::function<void()> callback)
{
  TatamiServiceInterface* owner = sourceFor(model);

  if (owner)
    owner->save(model, callback);
  else if (!sources.isEmpty())
    sources.first()->save(model, callback);
}
 
void MetaService::destroy(ModelType* model, std::function<void()> callback)
{
  TatamiServiceInterface* owner = sourceFor(model);

  if (owner)
    owner->destroy(model, callback);
  else if (!sources.isEmpty())
    sources.first()->destroy(model, callback);
}
 
void MetaService::loadFromJson(const QJsonObject& json)
{
  for (auto* source : sources)
    source->loadFromJson(json);
}

TatamiServiceInterface::ModelType* MetaService::createModel(const QVariantMap& attributes)
{
  if (!sources.isEmpty())
    return sources.first()->createModel(attributes);
  else
    qDebug() << "/!\\ MetaService::createModel called, but no source provided";
  return nullptr;
}

void MetaService::connectToSource(TatamiServiceInterface* source)
{
  SOURCE_SIGNALS(connect, source)
}
 
void MetaService::disconnectFromSource(TatamiServiceInterface* source)
{
  SOURCE_SIGNALS(disconnect, source)
}

void MetaService::reloadModelsFromSource()
{
  models.clear();
  for (auto* source : sources)
  {
    for (auto* model : source->values())
      models.insert(model->getUid(), model);
  }
}

void MetaService::onSourceChanged()
{
  reloadModelsFromSource();
  emit modelsChanged();
}
 
void MetaService::onModelAdded(ModelType* model)
{
  if (!models.contains(model->getUid()))
  {
    models.insert(model->getUid(), model);
    emit modelAdded(model);
  }
  else
  {
    models.insert(model->getUid(), model);
  }
}
 
void MetaService::onModelRemoved(ModelType* model)
{
  if (model && !sourceFor(model))
  {
    auto it = models.find(model->getUid());
    if (it != models.end())
    {
      models.erase(it);
      emit modelRemoved(model);
    }
  }
}
 
void MetaService::onModelSaved(ModelType* model)
{
  if (contains(model))
    emit modelSaved(model);
  else if (model)
  {
    models.insert(model->getUid(), model);
    emit modelAdded(model);
  }
}

QVector<HttpService*> MetaService::fetchableSources() const
{
  QVector<HttpService*> httpSources;

  for (auto* source : sources)
  {
    auto* httpSource = qobject_cast<HttpService*>(source);

    if (httpSource)
      httpSources.push_back(httpSource);
  }
  return httpSources;
}
