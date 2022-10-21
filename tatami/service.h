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
  typedef MODEL Model;

  explicit TatamiService(QObject* parent = nullptr) : TatamiServiceInterface(parent) {}

  void explicitSort(std::function<bool (const Model*, const Model*)> compare)
  {
    PaginatedStore::sort([compare](
      const ModelType* a,
      const ModelType* b)
    {
      return compare(reinterpret_cast<const Model*>(a), reinterpret_cast<const Model*>(b));
    });
  }

  Model* findIf(std::function<bool (const Model*)> compare) const
  {
    auto it = std::find_if(models.begin(), models.end(), [compare](const ModelType* model)
    {
      return compare(reinterpret_cast<const Model*>(model));
    });
    return it != models.end() ? reinterpret_cast<Model*>(*it) : nullptr;
  }

protected:
  ModelType* createModel(const QVariantMap& attributes = QVariantMap())
  {
    return MetaRecordable::factory<MODEL>(attributes, this);
  }
};

#endif
