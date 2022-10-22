#ifndef TATAMI_TESTLIB
#include "httpclient.h"
#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcessEnvironment>
#include <QUuid>
#include <QtWebSockets/QWebSocket>
#include <QDebug>

const QByteArray HttpClient::userAgent("Tatami/1.0");
const QUuid applicationUid = QUuid::createUuid();
static QByteArray defaultServerUrl;

HttpClient::HttpClient(QObject* parent) : QNetworkAccessManager(parent)
{
  serverUrl = QProcessEnvironment::systemEnvironment().value("TATAMI_SERVER_URL", defaultServerUrl);
}

void HttpClient::setDefaultServerUrl(const QByteArray& value)
{
  defaultServerUrl = value;
}

void HttpClient::decorateRequest(QNetworkRequest& request) const
{
  request.setRawHeader("User-Agent", userAgent);
  request.setRawHeader("Accept", "application/json");
  request.setRawHeader("X-Client-Uid", applicationUid.toByteArray());
}

void HttpClient::decorateJsonRequest(QNetworkRequest& request, int length) const
{
  decorateRequest(request);
  if (length > 0)
  {
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Length", QByteArray::number(length));
  }
}

QUrl HttpClient::getUrl(const QByteArray& path, QByteArray scheme) const
{
  QUrl url(serverUrl);
  QStringList parts = QString(path).split('?');

  if (scheme.length())
    url.setScheme(scheme);
  url.setPath(parts.takeFirst());
  url.setQuery(parts.join('?'));
  return url;
}

static void logRequest(const QByteArray& method, const QByteArray& path, const QJsonDocument& document)
{
  qDebug() << "Tatami:HttpClient:Request:" << method << ' ' << path << ':';
  //qDebug() << document.toJson(QJsonDocument::Indented).toStdString();
}

static void logResponse(const QByteArray& method, const QByteArray& path, const QJsonDocument& document)
{
  qDebug() << "Tatami:HttpClient:Response:" << method << ' ' << path << ':';
  //qDebug() << document.toJson(QJsonDocument::Indented).toStdString().c_str();
}

HttpClient::ResponseObject* HttpClient::post(const QByteArray& path, const QJsonDocument& document)
{
  QByteArray       body = document.toJson();
  QNetworkReply*   reply;
  QNetworkRequest* request = new QNetworkRequest(getUrl(path));

  logRequest("POST", path, document);
  decorateJsonRequest(*request, body.length());
  incrementRunningRequests();
  reply = QNetworkAccessManager::post(*request, body);
  connect(reply, &QNetworkReply::finished, [=]() { delete request; decrementRunningRequests(); });
  return reply;
}

HttpClient::ResponseObject* HttpClient::put(const QByteArray& path, const QJsonDocument& document)
{
  QByteArray       body = document.toJson();
  QNetworkReply*   reply;
  QNetworkRequest* request = new QNetworkRequest(getUrl(path));

  logRequest("PUT", path, document);
  decorateJsonRequest(*request, body.length());
  incrementRunningRequests();
  reply = QNetworkAccessManager::put(*request, body);
  connect(reply, &QNetworkReply::finished, [=]() { delete request; decrementRunningRequests(); });
  return reply;
}

HttpClient::ResponseObject* HttpClient::get(const QByteArray& path)
{
  QNetworkReply*   reply;
  QNetworkRequest* request = new QNetworkRequest(getUrl(path));

  logRequest("GET", path, QJsonDocument());
  decorateRequest(*request);
  incrementRunningRequests();
  reply = QNetworkAccessManager::get(*request);
  connect(reply, &QNetworkReply::finished, [=]() { delete request; decrementRunningRequests(); });
  return reply;
}

HttpClient::ResponseObject* HttpClient::get(const QByteArray& path, const QJsonDocument& document)
{
  QByteArray       body = document.toJson();
  QNetworkReply*   reply;
  QNetworkRequest* request = new QNetworkRequest(getUrl(path));

  logRequest("GET", path, document);
  decorateJsonRequest(*request, body.length());
  incrementRunningRequests();
  reply = QNetworkAccessManager::get(*request);
  connect(reply, &QNetworkReply::finished, [=]() { delete request; decrementRunningRequests(); });
  return reply;
}

HttpClient::ResponseObject* HttpClient::destroy(const QByteArray &path)
{
  QNetworkReply*   reply;
  QNetworkRequest* request = new QNetworkRequest(getUrl(path));

  logRequest("DELETE", path, QJsonDocument());
  decorateRequest(*request);
  incrementRunningRequests();
  reply = QNetworkAccessManager::deleteResource(*request);
  connect(reply, &QNetworkReply::finished, [=]() { delete request; decrementRunningRequests(); });
  return reply;
}

void HttpClient::listen(const QByteArray &path, QWebSocket& websocket)
{
  QByteArray scheme = serverUrl.scheme() == "https" ? "wss" : "ws";
  QNetworkRequest* request = new QNetworkRequest(getUrl(path, scheme));

  logRequest("WebSocket", path, QJsonDocument());
  decorateRequest(*request);
  websocket.open(*request);
}
#endif
