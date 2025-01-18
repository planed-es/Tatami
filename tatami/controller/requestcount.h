#ifndef  REQUESTCOUNT_COMPONENT_H
# define REQUESTCOUNT_COMPONENT_H

# include <QObject>
# include "../Tatami_global.h"

class HttpService;

class TATAMI_EXPORT RequestCountComponent : public QObject
{
  Q_OBJECT
public:
  explicit RequestCountComponent(QObject* parent = nullptr) : QObject(parent) {}
  virtual ~RequestCountComponent();

  inline bool hasPendingRequests() const { return pendingRequestCount > 0; }

protected:
  void connectWith(RequestCountComponent*);
  void disconnectFrom(RequestCountComponent*);
  void connectWith(HttpService*);
  void disconnectFrom(HttpService*);
  void resetRequestCount();

signals:
  void requestStarted();
  void requestEnded();

public slots:
  void onRequestStarted();
  void onRequestEnded();

private slots:
  void watchedRequestStarted();
  void watchedRequestEnded();

private:
  int pendingRequestCount = 0;
  int ownedPendingRequestCount = 0;
};

#endif
