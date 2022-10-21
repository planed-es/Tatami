#ifndef HTTPCLIENT_H
# define HTTPCLIENT_H
# ifdef TATAMI_TESTLIB
#  include "tests/httpclient.h"
# else
#  include <QtNetwork/QNetworkAccessManager>
#  include <QtNetwork/QNetworkReply>
#  include "Tatami_global.h"

class QJsonDocument;
class QWebSocket;

class TATAMI_EXPORT HttpClient : public QNetworkAccessManager
{
  Q_OBJECT

  unsigned short runningRequests = 0;
  QUrl           serverUrl;
public:
  typedef QNetworkReply ResponseObject;
  typedef QNetworkRequest::Attribute Attribute;

  static const QByteArray userAgent;

  HttpClient(QObject* parent = nullptr);

  void setServerUrl(const QUrl& value) { serverUrl = value; }
  inline bool isBusy() const { return runningRequests > 0; }

  ResponseObject* post(const QByteArray& path, const QJsonDocument&);
  ResponseObject* put(const QByteArray& path, const QJsonDocument&);
  ResponseObject* get(const QByteArray& path);
  ResponseObject* get(const QByteArray& path, const QJsonDocument&);
  ResponseObject* destroy(const QByteArray& path);
  void            listen(const QByteArray& path, QWebSocket&);

signals:
  void busyChanged();

private:
  virtual QUrl getUrl(const QByteArray& path, QByteArray protocol = "") const;
  virtual void decorateRequest(QNetworkRequest& request) const;
  virtual void decorateJsonRequest(QNetworkRequest& request, int length) const;
  void incrementRunningRequests() { runningRequests++; if (runningRequests == 1) { busyChanged(); } }
  void decrementRunningRequests() { runningRequests--; if (runningRequests == 0) { busyChanged(); } }
};

# endif
#endif // HTTPCLIENT_H
