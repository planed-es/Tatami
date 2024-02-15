#ifndef TEST_HTTPCLIENT_H
# define TEST_HTTPCLIENT_H

# include <QObject>
# include <QVariant>
# include <QJsonDocument>
# include "../Tatami_global.h"

class QWebSocket;

class TATAMI_EXPORT StubResponse : public QObject
{
  Q_OBJECT
public:
  enum Attribute
  {
    HttpStatusCodeAttribute = 0
  };

  StubResponse(QObject* parent = nullptr);

  QVariant attribute(Attribute) { return status; }
  QByteArray readAll() const { return body; }

  int           status = 200;
  QByteArray    body;
  QByteArray    path;
  QByteArray    method;
  QJsonDocument document;
signals:
  void stub(QByteArray method, QByteArray path, QJsonDocument body);
  void finished();
};

class TATAMI_EXPORT HttpClient : public QObject
{
  Q_OBJECT
public:
  typedef StubResponse ResponseObject;
  typedef StubResponse::Attribute Attribute;

  HttpClient(QObject* parent = nullptr) : QObject(parent) {}

  static void setCredentials(const QString& username, const QString& password) {}
  void setServerUrl(const QUrl& value) { serverUrl = value; }
  QUrl getServerUrl() const { return serverUrl; }
  inline bool isBusy() const { return false; }

  StubResponse* get(const QByteArray& path);
  StubResponse* get(const QByteArray& path, const QJsonDocument& document);
  StubResponse* put(const QByteArray& path, const QJsonDocument& document);
  StubResponse* post(const QByteArray& path, const QJsonDocument& document);
  StubResponse* destroy(const QByteArray path);
  void          listen(const QByteArray& path, QWebSocket&) {}

private:
  QUrl serverUrl;
  static StubResponse response;
};

#endif
