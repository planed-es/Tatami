#ifndef  PAGINATEDSTORE_H
# define PAGINATEDSTORE_H

# include "modelstore.h"

class TATAMI_EXPORT PaginatedStore : public ModelStore
{
public:
  typedef std::function<bool (const ModelType*, const ModelType*)> SortCompare;

  PaginatedStore(QObject* parent = nullptr);

  virtual void sort(SortCompare value) { sortCompare = value; emit modelsChanged(); }
  ModelList    sortedValues() const;
  ModelArray   page(unsigned int number, unsigned int itemsPerPage);
  ModelArray   filter(const QVariantMap& params) const { return ModelStore::filter(params); }
  ModelArray   filter(std::function<bool (const ModelType*)> callback) const override;

protected:
  SortCompare sortCompare;
};

#endif // PAGINATEDSTORE_H
