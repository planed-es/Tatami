#ifndef  SAMPLECONTROLLERCOMPONENT_H
# define SAMPLECONTROLLERCOMPONENT_H

# include "controller.h"

class TATAMI_EXPORT TatamiEditController : public TatamiController
{
  Q_OBJECT

  Q_PROPERTY(MetaRecordable* sample READ getSample NOTIFY sampleChanged)
public:

  explicit TatamiEditController(QObject* parent = nullptr);

  ModelType* getSample() const { return sample; }

  template<typename MODEL>
  MODEL* sampleAs() const { return reinterpret_cast<MODEL*>(sample); }

  Q_INVOKABLE virtual ModelType* startEdition(const ModelType*);
  Q_INVOKABLE virtual ModelType* startCreation();
  Q_INVOKABLE virtual void endEdition(bool save, QJSValue callback = QJSValue());
  Q_INVOKABLE virtual void remove(ModelType*, QJSValue callback = QJSValue());
  Q_INVOKABLE virtual bool canEdit(const ModelType*) { return true; }
  Q_INVOKABLE virtual bool canRemove(const ModelType*) { return true; }

signals:
  void sampleChanged();
  void sampleAttributesChanged();
  void invalidModel(const ModelType*);
  void inputError(QString);

private slots:
  void connectSample();

protected slots:
  virtual void decorateSample(ModelType*) const {}

protected:
  void callbackWithIndexUpdate(int lastIndex, QJSValue callback);

  ModelType* sample = nullptr;
};

#endif // SAMPLECONTROLLERCOMPONENT_H
