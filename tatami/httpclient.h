#ifndef HTTPCLIENT_H
# define HTTPCLIENT_H
# ifdef TATAMI_TESTLIB
#  include "tests/httpclient.h"
# else
#  include <QtNetwork/QNetworkAccessManager>
#  include <QtNetwork/QNetworkReply>
#  include <QtNetwork/QAuthenticator>
#  include "Tatami_global.h"

class QJsonDocument;
class QWebSocket;

class TATAMI_EXPORT HttpClient : public QNetworkAccessManager
{
  Q_OBJECT

  unsigned short runningRequests = 0;
  QUrl           serverUrl;
  bool           debugMode = false;
public:
  typedef QNetworkReply ResponseObject;
  typedef QNetworkRequest::Attribute Attribute;
  typedef std::function<void(ResponseObject*)> Callback;

  static const QByteArray userAgent;

  HttpClient(QObject* parent = nullptr);

  static void setDefaultServerUrl(const QByteArray& value);
  static void setCredentials(const QString& username, const QString& password);
  void setServerUrl(const QUrl& value) { serverUrl = value; }
  inline bool isBusy() const { return runningRequests > 0; }

  ResponseObject* post(const QByteArray& path, const QJsonDocument&, Callback = Callback());
  ResponseObject* put(const QByteArray& path, const QJsonDocument&, Callback = Callback());
  ResponseObject* get(const QByteArray& path, Callback = Callback());
  ResponseObject* get(const QByteArray& path, const QJsonDocument&, Callback = Callback());
  ResponseObject* destroy(const QByteArray& path, Callback = Callback());
  void            listen(const QByteArray& path, QWebSocket&);

signals:
  void busyChanged();

private slots:
  void authenticateQuery(QNetworkReply*, QAuthenticator*);

private:
  virtual QUrl getUrl(const QByteArray& path, QByteArray protocol = "") const;
  virtual void decorateRequest(QNetworkRequest& request) const;
  virtual void decorateJsonRequest(QNetworkRequest& request, int length) const;
  void incrementRunningRequests() { runningRequests++; if (runningRequests == 1) { busyChanged(); } }
  void decrementRunningRequests() { runningRequests--; if (runningRequests == 0) { busyChanged(); } }
};

# endif
#endif // HTTPCLIENT_H
