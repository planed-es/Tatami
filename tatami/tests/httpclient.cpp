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

StubResponse* HttpClient::get(const QByteArray& path)
{ emit response.stub("GET", path, QJsonDocument()); return &response; }
StubResponse* HttpClient::get(const QByteArray& path, const QJsonDocument& document)
{ emit response.stub("GET", path, document); return &response; }
StubResponse* HttpClient::put(const QByteArray& path, const QJsonDocument& document)
{ emit response.stub("PUT", path, document); return &response; }
StubResponse* HttpClient::post(const QByteArray& path, const QJsonDocument& document)
{ emit response.stub("POST", path, document); return &response; }
StubResponse* HttpClient::destroy(const QByteArray path)
{ emit response.stub("DELETE", path, QJsonDocument()); return &response; }
