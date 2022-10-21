#include "modelstore.h"

static int getCompareStrategyFromFilter(const QVariantMap& params)
{
  auto strategyIt = params.find("_strategy");

  if (strategyIt != params.end())
    return strategyIt.value().toInt();
  return MetaRecordComparable::Likeness;
}

bool filterWithVariantMap(const QVariantMap& params, const ModelStore::ModelType* model)
{
  return model->compare(params, getCompareStrategyFromFilter(params));
}

ModelStore::ModelStore(QObject* parent) : QObject(parent)
{
}

void ModelStore::insert(ModelType* model)
{
  replaceModel(model);
  emit modelAdded(model);
}

void ModelStore::remove(ModelType* model)
{
  auto it = models.find(model->getUuid());

  if (it != models.end())
  {
    models.erase(it);
    emit modelRemoved(it.value());
    disconnect(it.value(), &QObject::destroyed, this, &ModelStore::purgeModel);
    cleanUpModel(it.value());
  }
}

void ModelStore::replaceModel(ModelType* model)
{
  auto it = models.find(model->getUuid());
  if (it != models.end())
    cleanUpModel(it.value());
  models.insert(model->getUuid(), model);
  connect(model, &QObject::destroyed, this, &ModelStore::purgeModel);
}

void ModelStore::purgeModel(QObject* object)
{
  auto it = std::find_if(models.begin(), models.end(), [object](ModelType* ptr) { return object == ptr; });
  if (it != models.end())
  {
    models.erase(it);
    emit modelRemoved(reinterpret_cast<ModelType*>(object));
  }
}

void ModelStore::reset()
{
  for (auto it = models.begin() ; it != models.end() ; ++it)
    cleanUpModel(it.value());
  models.clear();
}

ModelStore::ModelType* ModelStore::get(const QByteArray& uid) const
{
  auto it = models.find(uid);

  if (it != models.end())
    return it.value();
  return nullptr;
}

ModelStore::ModelType* ModelStore::at(unsigned int index) const
{
  if (index < models.size())
  {
    auto it = models.begin();

    std::advance(it, index);
    return it.value();
  }
  return nullptr;
}

ModelStore::ModelType* ModelStore::findOne(const QVariantMap& params, int comparaisonType) const
{
  auto it = std::find_if(models.begin(), models.end(), [params, comparaisonType](ModelType* candidate) -> bool
  {
    return candidate->compare(params, comparaisonType);
  });

  return it == models.end() ? nullptr : it.value();
}

ModelStore::ModelArray ModelStore::getByUid(const QStringList& uids) const
{
  ModelStore::ModelArray result;

  result.reserve(uids.size());
  for (const QString& uid : uids)
  {
    ModelType* model = get(uid.toStdString().c_str());

    if (model)
      result.push_back(model);
  }
  return result;
}

ModelStore::ModelArray ModelStore::filter(const QVariantMap& params) const
{
  return filter(std::bind(&filterWithVariantMap, params, std::placeholders::_1));
}

ModelStore::ModelArray ModelStore::filter(std::function<bool (const ModelType*)> callback) const
{
  ModelStore::ModelArray result;

  result.reserve(models.size());
  for (auto it = models.begin() ; it != models.end() ; ++it)
  {
    if (callback(it.value()))
      result.push_back(it.value());
  }
  return result;
}
