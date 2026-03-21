#ifndef  METASERVICE_H
# define METASERVICE_H

# include "service.h"

class TATAMI_EXPORT MetaService : public TatamiServiceInterface
{
  Q_OBJECT
public:
  MetaService(QObject* parent = nullptr);

  void addSource(TatamiServiceInterface* source);
  void removeSource(TatamiServiceInterface* source);
  const QVector<TatamiServiceInterface*>& getSources() const { return sources; }

  // by default, fallback to the first source, if no source was found for the corresponding model
  virtual void save(ModelType*, std::function<void()>) override;
  virtual void destroy(ModelType*, std::function<void()> callback = std::function<void()>()) override;
  // broadcasts to ALL the sources
  virtual void loadFromJson(const QJsonObject&) override;

  template<typename... ARGS>
  void fetch(ARGS&&... args, Callback callback)
  {
    QVector<HttpService*> httpSources = fetchableSources();

    if (httpSources.size() > 0)
    {
      auto remaining = std::make_shared<int>(httpSources.size());

      for (auto* http : httpSources)
      {
        http->fetch(std::forward<ARGS>(args)..., [remaining, callback]()
        {
          int& remaining_ref = *remaining;

          if (--remaining_ref == 0 && callback)
            callback();
        });
      }
    }
    else if (callback)
      callback();
  }

signals:
  void refreshAll();

protected slots:
  void onSourceChanged();
  void onModelAdded(ModelType*);
  void onModelRemoved(ModelType*);
  void onModelSaved(ModelType*);
  void reloadModelsFromSource();

protected:
  virtual ModelType* createModel(const QVariantMap& attributes = QVariantMap()) override;

private:
  void connectToSource(TatamiServiceInterface*);
  void disconnectFromSource(TatamiServiceInterface*);
  TatamiServiceInterface* sourceFor(const ModelType*) const;
  QVector<HttpService*> fetchableSources() const;

  QVector<TatamiServiceInterface*> sources;
};

#endif
