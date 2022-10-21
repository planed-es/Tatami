#ifndef  NOTIFIABLESERVICE_H
# define NOTIFIABLESERVICE_H

# include "httpservice.h"
# include <QtWebSockets/QWebSocket>

class TATAMI_EXPORT NotifiableService : public HttpService
{
  Q_OBJECT
public:
  explicit NotifiableService(QObject* parent = nullptr);

  void enableNotifications(bool);
  bool notificationsEnabled() const { return withNotifications; }
  void listenToNotificationsAt(const QByteArray& path);

signals:
  void notificationsEnabledChanged();

private slots:
  void listenToNotifications();
  void receivedNotification(const QString&);
  void receivedNotificationError(QAbstractSocket::SocketError);

protected:
  QWebSocket websocket;
  bool       withNotifications = false;
};

#endif // NOTIFIABLESERVICE_H
