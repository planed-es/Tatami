#ifndef  DISPLAYSERVICE_H
# define DISPLAYSERVICE_H

# include <QPointer>
# include "subsetservice.h"

class TATAMI_EXPORT DisplayService : public SubsetService
{
  Q_OBJECT
public:
  enum NewLinePolicy
  {
    NewLineOnTop    = 0,
    NewLineAtBottom = 1
  };

  enum EditState
  {
    ReadState   = 0,
    CreateState = 1,
    UpdateState = 2
  };

  DisplayService(QObject* parent = nullptr);

  void          setNewLinePolicy(NewLinePolicy value) { newLinePolicy = value; }

  ModelType*    startEdition(const ModelType& editedModel);
  ModelType*    startCreation();
  void          endEdition(bool saveSample, std::function<void()> callback);
  void          resetState();
  void          sort(TatamiServiceInterface::SortCompare value) override;
  void          generateSuggestions(std::function<QVector<ModelType*>()>);

signals:
  void          invalidModel(const ModelType*);

private:
  void          onSourceChanged() override;
  bool          sortWithSample(const ModelType* a, const ModelType* b, SortCompare compare) const;

  NewLinePolicy       newLinePolicy = NewLineAtBottom;
  EditState           state = ReadState;
  QPointer<ModelType> sample;
};
#endif // DISPLAYSERVICE_H
