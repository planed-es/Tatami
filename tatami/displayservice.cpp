#include "displayservice.h"

DisplayService::DisplayService(QObject* parent) : SubsetService(parent)
{
  SubsetService::sort(std::bind(&DisplayService::sortWithSample, this, std::placeholders::_1, std::placeholders::_2, SortCompare()));
}

DisplayService::ModelType* DisplayService::startEdition(const ModelType& editedModel)
{
  if (state == ReadState)
  {
    sample = createModel();
    sample->copy(&editedModel);
    state = UpdateState;
    return sample;
  }
  return nullptr;
}

DisplayService::ModelType* DisplayService::startCreation()
{
  if (state == ReadState)
  {
    sample = createModel();
    qDebug() << "DisplayService sample:" << sample;
    state = CreateState;
    models.insert("", sample);
    emit modelAdded(sample);
    return sample;
  }
  return nullptr;
}

void DisplayService::endEdition(bool saveSample, std::function<void()> callback)
{
  if (saveSample && sample)
  {
    if (sample->isValid())
    {
      save(sample, [this, callback]()
      {
        sample->setParent(getSource());
        resetState();
        if (callback) callback();
      });
    }
    else
      emit invalidModel(sample);
  }
  else
  {
    resetState();
    if (callback) callback();
  }
}

void DisplayService::resetState()
{
  if (sample)
  {
    if (state == CreateState)
    {
      models.remove("");
      emit modelRemoved(sample);
    }
    if (get(sample->getUid()) != sample)
      sample->deleteLater();
    sample = nullptr;
  }
  state = ReadState;
}

void DisplayService::onSourceChanged()
{
  reloadModelsFromSource();
  if (state == CreateState)
    models.insert("", sample);
  emit modelsChanged();
}

void DisplayService::sort(TatamiServiceInterface::SortCompare value)
{
  SubsetService::sort(std::bind(&DisplayService::sortWithSample, this, std::placeholders::_1, std::placeholders::_2, value));
}

bool DisplayService::sortWithSample(const ModelType* a, const ModelType* b, SortCompare compare) const
{
  if (state == CreateState)
  {
    if (a == sample)
      return newLinePolicy == NewLineOnTop;
    if (b == sample)
      return newLinePolicy == NewLineAtBottom;
  }
  if (compare)
    return compare(a, b);
  return false;
}
