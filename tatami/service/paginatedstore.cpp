#include "paginatedstore.h"

PaginatedStore::PaginatedStore(QObject* parent) : ModelStore(parent)
{
}

PaginatedStore::ModelList PaginatedStore::sortedValues() const
{
  auto result = models.values();

  if (sortCompare)
    std::stable_sort(result.begin(), result.end(), sortCompare);
  return result;
}

PaginatedStore::ModelArray PaginatedStore::page(unsigned int pageNumber, unsigned int itemsPerPage)
{
  ModelArray   result;
  auto         src  = sortedValues();
  auto         it   = src.begin();
  auto         end  = src.begin();
  unsigned int max  = static_cast<unsigned int>(models.size());
  unsigned int from = std::min<unsigned int>(max, itemsPerPage * pageNumber);
  unsigned int to   = std::min<unsigned int>(max, itemsPerPage * (pageNumber + 1));

  result.reserve(itemsPerPage);
  std::advance(it, from);
  std::advance(end, to);
  std::copy(it, end, std::back_inserter(result));
  return result;
}

PaginatedStore::ModelArray PaginatedStore::filter(std::function<bool (const ModelType*)> callback) const
{
  ModelArray result = ModelStore::filter(callback);

  if (sortCompare)
    std::sort(result.begin(), result.end(), sortCompare);
  return result;
}
