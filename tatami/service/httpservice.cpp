#include "httpservice.h"
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonArray>
#include <QProcessEnvironment>
#include <QDebug>
#include <iostream>
#include <qi18n.h>

static QJsonArray toJsonArray(const QVector<QByteArray>& source)
{
  QJsonArray array;

  for (const auto& item : source)
    array << QJsonValue::fromVariant(item);
  return array;
}

HttpService::HttpService(QObject* parent) : JsonModelStore(parent)
{
}

void HttpService::withPath(const QUrl& url, std::function<void()> callback)
{
  QByteArray backup = path;

  setPath(url);
  callback();
  path = backup;
}

QByteArray HttpService::pathFor(const ModelType& model) const
{
  if (model.isPersistent())
    return path + '/' + model.getUid();
  return path;
}

void HttpService::fetch(Callback userCallback)
{
  auto* reply    = http.get(path);
  auto  callback = std::bind(&HttpService::receivedFetchReply, this, reply, userCallback, true);

  emit requestStarted();
  connect(reply, &HttpClient::ResponseObject::finished, this, callback);
}

void HttpService::fetch(const QUrlQuery& params, Callback callback)
{
  QUrl url(path);

  url.setQuery(params);
  withPath(url, [this, callback]() { fetch(callback); });
}

void HttpService::fetch(const QJsonDocument& document, Callback userCallback)
{
  auto* reply    = http.post(path, document);
  auto  callback = std::bind(&HttpService::receivedFetchReply, this, reply, userCallback, true);

  emit requestStarted();
  connect(reply, &HttpClient::ResponseObject::finished, this, callback);
}

void HttpService::fetchOne(const QByteArray& uid, std::function<void (ModelType*)> then)
{
  auto* reply = http.get(path + '/' + uid);
  auto  callback = [this, reply, then, uid]()
  {
    receivedFetchReply(reply, Callback(), false);
    then(get(uid));
  };

  emit requestStarted();
  connect(reply, &HttpClient::ResponseObject::finished, this, callback);
}

void HttpService::fetchUids(const QStringList& list, Callback userCallback)
{
  QVector<QByteArray> uids;

  for (const auto& uid : list)
    uids.push_back(QByteArray(uid.toStdString().c_str()));
  fetchUids(uids, userCallback);
}

void HttpService::fetchUids(const QVector<QByteArray>& uids, Callback userCallback)
{
  auto* reply    = http.post(path + "/withUids", QJsonDocument(toJsonArray(uids)));
  auto  callback = std::bind(&HttpService::receivedFetchReply, this, reply, userCallback, true);

  emit requestStarted();
  connect(reply, &HttpClient::ResponseObject::finished, this, callback);
}

void HttpService::receivedFetchReply(HttpClient::ResponseObject* reply, Callback callback, bool withReset)
{
  emit requestEnded();
  if (reply->attribute(HttpClient::Attribute::HttpStatusCodeAttribute).toUInt() == 200)
  {
    auto a = reply->readAll();
    auto debugMode = QProcessEnvironment::systemEnvironment().value("HTTP_DEBUG", "0") == "1";

    if (debugMode)
      std::cout << path.toStdString() << " -> " << a.toStdString() << std::endl;
    if (withReset)
      reset();
    loadFromJson(QJsonDocument::fromJson(a));
    if (callback)
      callback();
  }
  else
    receivedErrorReply(reply);
}

void HttpService::save(ModelType* model, std::function<void()> callback)
{
  if (model->isPersistent())
    update(model, callback);
  else
    create(model, callback);
}

void HttpService::parseCreateResponse(ModelType* model, HttpClient::ResponseObject* reply)
{
  model->setUid(reply->readAll());
}

void HttpService::create(ModelType* model, std::function<void()> callback)
{
  auto* reply = http.post(pathFor(*model), QJsonDocument(model->toJson()));

  emit requestStarted();
  connect(reply, &HttpClient::ResponseObject::finished, this, [this, reply, model, callback]()
  {
    unsigned int status = reply->attribute(HttpClient::Attribute::HttpStatusCodeAttribute).toUInt();

    emit requestEnded();
    switch (status)
    {
      case 200:
        model->setParent(this);
        parseCreateResponse(model, reply);
        models.insert(model->getUid(), model);
        emit modelAdded(model);
        if (callback) callback();
        break ;
      case 422:
        receivedCreateErrorReply(reply);
        break ;
      default:
        receivedErrorReply(reply);
        break ;
    }
  });
}

void HttpService::update(ModelType* model, std::function<void()> callback)
{
  auto* reply = http.post(pathFor(*model), QJsonDocument(model->toJson()));

  emit requestStarted();
  connect(reply, &HttpClient::ResponseObject::finished, this, [this, reply, model, callback]()
  {
    unsigned int status = reply->attribute(HttpClient::Attribute::HttpStatusCodeAttribute).toUInt();
    ModelType* ownedModel;

    emit requestEnded();
    switch (status)
    {
      case 200:
        ownedModel = get(model->getUid());
        if (ownedModel)
          ownedModel->copy(model);
        else
        {
          model->setParent(this);
          models.insert(model->getUid(), model);
        }
        emit modelSaved(model);
        if (callback) callback();
        break ;
      case 422:
        receivedCreateErrorReply(reply);
        break ;
      default:
        receivedErrorReply(reply);
        break ;
    }
  });
}

void HttpService::destroy(ModelType* model, std::function<void ()> callback)
{
  auto* reply = http.destroy(pathFor(*model));

  emit requestStarted();
  connect(reply, &HttpClient::ResponseObject::finished, this, [this, reply, model, callback]()
  {
    unsigned int status = reply->attribute(HttpClient::Attribute::HttpStatusCodeAttribute).toUInt();

    emit requestEnded();
    switch (status)
    {
      case 200:
        models.remove(model->getUid());
        emit modelRemoved(model);
        model->setUid(QByteArray());
        emit model->attributeChanged();
        if (callback)
          callback();
        break ;
      default:
        receivedErrorReply(reply);
        break ;
    }
  });
}

void HttpService::destroyAll(std::function<void ()> callback)
{
  QSharedPointer<short> count(new short);
  const auto list = values();

  *count = 0;
  for (ModelType* model : list)
  {
    if (model->isPersistent())
    {
      *count += 1;
      destroy(model, [count, callback]()
      {
        *count -= 1;
        if (*count == 0 && callback)
          callback();
      });
    }
  }
  if (*count == 0)
    callback();
}

void HttpService::receivedCreateErrorReply(HttpClient::ResponseObject* reply)
{
  QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();

  qDebug() << "/!\\" << ("Tatami:HttpService:" + path) << "received create error reply" << response;
  if (response["errors"].isObject())
    emit postFailure(response["errors"].toObject());
  else
    receivedErrorReply(reply);
}

void HttpService::receivedErrorReply(HttpClient::ResponseObject* reply)
{
  unsigned int status = reply->attribute(HttpClient::Attribute::HttpStatusCodeAttribute).toUInt();

  qDebug() << "/!\\" << ("Tatami:Service:" + path) << "server responded with status" << status;
  if (status >= 400)
    emit fetchFailure(QI18n::get()->t("messages.serverRejectedQuery"));
  else if (status >= 500)
    emit fetchFailure(QI18n::get()->t("messages.serverFailure"));
  else if (status == 0)
    emit fetchFailure(QI18n::get()->t("messages.serverUnreachable"));
  else
    emit fetchFailure(QI18n::get()->t("messages.serverUnexpectedResponse"));
}
