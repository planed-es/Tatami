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
  void connectWith(HttpService*);
  void disconnectFrom(HttpService*);
  void resetRequestCount();

signals:
  void requestStarted();
  void requestEnded();

public slots:
  void onRequestStarted();
  void onRequestEnded();

private:
  int pendingRequestCount = 0;
};

#endif
