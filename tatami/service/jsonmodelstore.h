#ifndef  JSONMODELSTORE_H
# define JSONMODELSTORE_H

# include "paginatedstore.h"

class TATAMI_EXPORT JsonModelStore : public PaginatedStore
{
public:
  explicit JsonModelStore(QObject* parent = nullptr);

  virtual ModelType* createModel(const QVariantMap& attributes = QVariantMap()) = 0;
  virtual void              loadFromJson(const QJsonDocument&);
  virtual void              loadFromJson(const QJsonObject&);

protected:
  virtual void              decorateParsedModel(ModelType*) {};
};

#endif // JSONMODELSTORE_H
