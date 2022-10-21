#ifndef TEST_HTTPCLIENT_H
# define TEST_HTTPCLIENT_H

# ifdef TATAMI_TESTLIB
#  include <QObject>
#  include <QVariant>
#  include <QJsonDocument>
#  include "../Tatami_global.h"

class QWebSocket;

class TATAMI_EXPORT StubResponse : public QObject
{
  Q_OBJECT
public:
  enum Attribute
  {
    HttpStatusCodeAttribute = 0
  };

  StubResponse(QObject* parent = nullptr) : QObject(parent)
  {
    connect(this, &StubResponse::stub, this, [this](QByteArray method_, QByteArray value, QJsonDocument body)
    {
      method = method_;
      path = value;
      document = body;
    });
  }

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

  void setServerUrl(const QUrl&) {}
  inline bool isBusy() const { return false; }

  StubResponse* get(const QByteArray& path) { emit response.stub("GET", path, QJsonDocument()); return &response; }
  StubResponse* get(const QByteArray& path, const QJsonDocument& document) { emit response.stub("GET", path, document); return &response; }
  StubResponse* put(const QByteArray& path, const QJsonDocument& document) { emit response.stub("PUT", path, document); return &response; }
  StubResponse* post(const QByteArray& path, const QJsonDocument& document) { emit response.stub("POST", path, document); return &response; }
  StubResponse* destroy(const QByteArray path) { emit response.stub("DELETE", path, QJsonDocument()); return &response; }
  void          listen(const QByteArray& path, QWebSocket&) {}

private:
  static StubResponse response;
};

# endif
#endif
