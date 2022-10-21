#include "jsonmodelstore.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

JsonModelStore::JsonModelStore(QObject* parent) : PaginatedStore(parent)
{
}

void JsonModelStore::loadFromJson(const QJsonDocument& document)
{
  if (document.isArray())
  {
    for (QJsonValueRef value : document.array())
      loadFromJson(value.toObject());
  }
  else
    loadFromJson(document.object());
  emit modelsChanged();
}

void JsonModelStore::loadFromJson(const QJsonObject& object)
{
  ModelType* model = createModel(object.toVariantMap());

  decorateParsedModel(model);
  models.insert(model->getUuid(), model);
}
