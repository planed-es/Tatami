#include "filter.h"

static int getCompareStrategyFromFilter(const QVariantMap& params)
{
  auto strategyIt = params.find("_strategy");

  if (strategyIt != params.end())
    return strategyIt.value().toInt();
  return MetaRecordComparable::Likeness;
}

bool filterWithVariantMap(const QVariantMap& params, const MetaRecordComparable* model)
{
  return model->compare(params, getCompareStrategyFromFilter(params));
}
