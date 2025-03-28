#ifndef TATAMI_TESTLIB
#include "httpclient.h"
#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcessEnvironment>
#include <QUuid>
#include <QtWebSockets/QWebSocket>
#include <QDebug>
#include <iostream>

const QByteArray HttpClient::userAgent("Tatami/1.0");
const QUuid applicationUid = QUuid::createUuid();
static QByteArray defaultServerUrl;
static QString username;
static QString password;

HttpClient::HttpClient(QObject* parent) : QNetworkAccessManager(parent)
{
  serverUrl = QProcessEnvironment::systemEnvironment().value("TATAMI_SERVER_URL", defaultServerUrl);
  debugMode = QProcessEnvironment::systemEnvironment().value("HTTP_DEBUG", "0") == "1";
  connect(this, &QNetworkAccessManager::authenticationRequired, this, &HttpClient::authenticateQuery);
}

void HttpClient::authenticateQuery(QNetworkReply*, QAuthenticator* authenticator)
{
  authenticator->setUser(username);
  authenticator->setPassword(password);
}

void HttpClient::setDefaultServerUrl(const QByteArray& value)
{
  defaultServerUrl = value;
}

void HttpClient::setCredentials(const QString& username_, const QString& password_)
{
  username = username_;
  password = password_;
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

static void logRequest(const QByteArray& method, const QByteArray& path, const QJsonDocument& document, bool debugMode)
{
  qDebug() << "Tatami:HttpClient:Request:" << method << ' ' << path << ':';
  if (debugMode)
    std::cout << document.toJson(QJsonDocument::Indented).toStdString() << std::endl;
}

static void logResponse(const QByteArray& method, const QByteArray& path, const QJsonDocument& document, bool debugMode)
{
  if (debugMode)
  {
    qDebug() << "Tatami:HttpClient:Response:" << method << ' ' << path << ':';
    std::cout << document.toJson(QJsonDocument::Indented).toStdString() << std::endl;
  }
}

HttpClient::ResponseObject* HttpClient::post(const QByteArray& path, const QJsonDocument& document, Callback callback)
{
  QByteArray       body = document.toJson();
  QNetworkReply*   reply;
  QNetworkRequest* request = new QNetworkRequest(getUrl(path));

  logRequest("POST", path, document, debugMode);
  decorateJsonRequest(*request, body.length());
  incrementRunningRequests();
  reply = QNetworkAccessManager::post(*request, body);
  connect(reply, &QNetworkReply::finished, [=]()
  {
    qDebug() << "Response status =" << reply->attribute(QNetworkRequest::Attribute::HttpStatusCodeAttribute).toUInt();
    //logResponse("POST", path, QJsonDocument::fromJson(reply->readAll()), debugMode);
    if (callback) { callback(reply); }
  });
  connect(reply, &QNetworkReply::finished, [=]() { delete request; decrementRunningRequests(); });
  return reply;
}

HttpClient::ResponseObject* HttpClient::put(const QByteArray& path, const QJsonDocument& document, Callback callback)
{
  QByteArray       body = document.toJson();
  QNetworkReply*   reply;
  QNetworkRequest* request = new QNetworkRequest(getUrl(path));

  logRequest("PUT", path, document, debugMode);
  decorateJsonRequest(*request, body.length());
  incrementRunningRequests();
  reply = QNetworkAccessManager::put(*request, body);
  if (callback)
    connect(reply, &QNetworkReply::finished, std::bind(callback, reply));
  connect(reply, &QNetworkReply::finished, [=]() { delete request; decrementRunningRequests(); });
  return reply;
}

HttpClient::ResponseObject* HttpClient::get(const QByteArray& path, Callback callback)
{
  QNetworkReply*   reply;
  QNetworkRequest* request = new QNetworkRequest(getUrl(path));

  logRequest("GET", path, QJsonDocument(), debugMode);
  decorateRequest(*request);
  incrementRunningRequests();
  reply = QNetworkAccessManager::get(*request);
  if (callback)
    connect(reply, &QNetworkReply::finished, std::bind(callback, reply));
  connect(reply, &QNetworkReply::finished, [=]() { delete request; decrementRunningRequests(); });
  return reply;
}

HttpClient::ResponseObject* HttpClient::get(const QByteArray& path, const QJsonDocument& document, Callback callback)
{
  QByteArray       body = document.toJson();
  QNetworkReply*   reply;
  QNetworkRequest* request = new QNetworkRequest(getUrl(path));

  logRequest("GET", path, document, debugMode);
  decorateJsonRequest(*request, body.length());
  incrementRunningRequests();
  reply = QNetworkAccessManager::get(*request);
  if (callback)
    connect(reply, &QNetworkReply::finished, std::bind(callback, reply));
  connect(reply, &QNetworkReply::finished, [=]() { delete request; decrementRunningRequests(); });
  return reply;
}

HttpClient::ResponseObject* HttpClient::destroy(const QByteArray &path, Callback callback)
{
  QNetworkReply*   reply;
  QNetworkRequest* request = new QNetworkRequest(getUrl(path));

  logRequest("DELETE", path, QJsonDocument(), debugMode);
  decorateRequest(*request);
  incrementRunningRequests();
  reply = QNetworkAccessManager::deleteResource(*request);
  if (callback)
    connect(reply, &QNetworkReply::finished, std::bind(callback, reply));
  connect(reply, &QNetworkReply::finished, [=]() { delete request; decrementRunningRequests(); });
  return reply;
}

void HttpClient::listen(const QByteArray &path, QWebSocket& websocket)
{
  QByteArray scheme = serverUrl.scheme() == "https" ? "wss" : "ws";
  QNetworkRequest* request = new QNetworkRequest(getUrl(path, scheme));

  logRequest("WebSocket", path, QJsonDocument(), debugMode);
  decorateRequest(*request);
  websocket.open(*request);
}
#endif
