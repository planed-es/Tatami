#ifndef  SUBSETSERVICE_H
# define SUBSETSERVICE_H

# include "service.h"
# include <functional>

class TATAMI_EXPORT SubsetService : public TatamiServiceInterface
{
  Q_OBJECT
public:
  SubsetService(QObject* parent = nullptr);
  SubsetService(TatamiServiceInterface* _source, std::function<bool(const ModelType*)> selector);

  void setSource(TatamiServiceInterface*);
  TatamiServiceInterface* getSource() { return source; }

  virtual void save(ModelType*, std::function<void()>) override;
  virtual void destroy(ModelType*, std::function<void()> callback = std::function<void()>()) override;
  virtual void loadFromJson(const QJsonObject&) override;

  void setSelector(std::function<bool(const ModelType*)>);
  void setSelector(QVariantMap);

signals:
  void refreshSelection();

protected slots:
  virtual void onSourceChanged();
  void onModelAdded(ModelType*);
  void onModelRemoved(ModelType*);
  void onModelSaved(ModelType*);

private:
  void connectToSource();
  void disconnectFromSource();
  TatamiServiceInterface* source;
  std::function<bool(const ModelType*)> selector;

protected:
  void reloadModelsFromSource();
  virtual ModelType* createModel(const QVariantMap& attributes = QVariantMap()) override { return source ? source->createModel(attributes) : nullptr; }
};

#endif
