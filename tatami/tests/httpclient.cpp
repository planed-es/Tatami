#include "httpclient.h"

StubResponse HttpClient::response;

StubResponse::StubResponse(QObject* parent) : QObject(parent)
{
  connect(this, &StubResponse::stub, this, [this](QByteArray method_, QByteArray value, QJsonDocument body)
  {
    method = method_;
    path = value;
    document = body;
  });
}

StubResponse* HttpClient::get(const QByteArray& path, Callback callback)
{
  emit response.stub("GET", path, QJsonDocument());
  if (callback) { callback(&response); }
  return &response;
}

StubResponse* HttpClient::get(const QByteArray& path, const QJsonDocument& document, Callback callback)
{
  emit response.stub("GET", path, document);
  if (callback) { callback(&response); }
  return &response;
}

StubResponse* HttpClient::put(const QByteArray& path, const QJsonDocument& document, Callback callback)
{
  emit response.stub("PUT", path, document);
  if (callback) { callback(&response); }
  return &response;
}

StubResponse* HttpClient::post(const QByteArray& path, const QJsonDocument& document, Callback callback)
{
  emit response.stub("POST", path, document);
  if (callback) { callback(&response); }
  return &response;
}

StubResponse* HttpClient::destroy(const QByteArray path, Callback callback)
{
  emit response.stub("DELETE", path, QJsonDocument());
  if (callback) { callback(&response); }
  return &response;
}
