#ifndef  APPLICATIONCONTROLLER_H
# define APPLICATIONCONTROLLER_H

# include "service.h"
# include "controller/listnavigation.h"

class TATAMI_EXPORT TatamiController : public ListNavigationComponent
{
  Q_OBJECT

  Q_PROPERTY(QVariantMap filters MEMBER filters NOTIFY filtersChanged)
  Q_PROPERTY(bool withNotifications MEMBER withNotifications NOTIFY notificationsEnabledChanged)
public:
  typedef TatamiServiceInterface::ModelType ModelType;

  explicit TatamiController(QObject* parent = nullptr);
  virtual ~TatamiController();

  template<typename MODEL>
  void makeService(const QByteArray& path)
  {
    auto* newService = new TatamiService<MODEL>(this);

    newService->setPath(path);
    useService(newService);
    newService->fetch();
  }

  virtual void            useService(TatamiServiceInterface*);
  void                    useDefaultColumns();
  void                    discardService();
  void                    enableNotifications(bool);
  bool                    notificationsEnabled() const { return withNotifications; }
  TatamiServiceInterface* getService() const { return service; }
  const QVariantMap&      getFilters() const { return filters; }

signals:
  void notificationsEnabledChanged();
  void filtersChanged();
  void fetchFailure(QString errorMessage);
  void postFailure(QJsonObject errors);
  void invalidQueryFailure(QString errorMessage);

protected:
  virtual bool filter(const ModelType*) const;
  QVariantMap  filters;
private:
  TatamiServiceInterface* service = nullptr;
  bool withNotifications = false;
};

#endif // APPLICATIONCONTROLLER_H
