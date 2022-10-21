#ifndef  TATAMI_SERVICE_H
# define TATAMI_SERVICE_H

# include "service/notifiableservice.h"
# include <algorithm>

class TATAMI_EXPORT TatamiServiceInterface : public NotifiableService
{
public:
  TatamiServiceInterface(QObject* parent = nullptr) : NotifiableService(parent) {}
};

template<typename MODEL>
class TatamiService : public TatamiServiceInterface
{
public:
  typedef MODEL ModelType;

  explicit TatamiService(QObject* parent = nullptr) : TatamiServiceInterface(parent) {}

  void explicitSort(std::function<bool (const ModelType*, const ModelType*)> compare)
  {
    PaginatedStore::sort([compare](
      const TatamiServiceInterface::ModelType* a,
      const TatamiServiceInterface::ModelType* b)
    {
      return compare(reinterpret_cast<const ModelType*>(a), reinterpret_cast<const ModelType*>(b));
    });
  }

  ModelType* findIf(std::function<bool (const ModelType*)> compare) const
  {
    auto it = std::find_if(models.begin(), models.end(), [compare](const TatamiServiceInterface::ModelType* model)
    {
      return compare(reinterpret_cast<const ModelType*>(model));
    });
    return it != models.end() ? reinterpret_cast<ModelType*>(*it) : nullptr;
  }

protected:
  ModelType* createModel(const QVariantMap& attributes = QVariantMap())
  {
    return MetaRecordable::factory<ModelType>(attributes, this);
  }
};

#endif
