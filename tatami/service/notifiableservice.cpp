#include "notifiableservice.h"
#include <QJsonDocument>

NotifiableService::NotifiableService(QObject* parent) : HttpService(parent)
{
  connect(&websocket, &QWebSocket::textMessageReceived, this, &NotifiableService::receivedNotification);
  connect(&websocket, &QWebSocket::errorOccurred, this, &NotifiableService::receivedNotificationError);
}

void NotifiableService::enableNotifications(bool value)
{
  if (value != withNotifications)
  {
    withNotifications = value;
    if (withNotifications)
    {
      connect(this, &HttpService::pathChanged, this, &NotifiableService::listenToNotifications);
      listenToNotifications();
    }
    else
    {
      disconnect(this, &HttpService::pathChanged, this, &NotifiableService::listenToNotifications);
      websocket.close();
    }
    emit notificationsEnabledChanged();
  }
}

void NotifiableService::listenToNotifications()
{
  websocket.close();
  http.listen(QByteArray(getPath()).replace("/dailyProcess", "") + "/listen", websocket);
}

void NotifiableService::listenToNotificationsAt(const QByteArray& notificationPath)
{
  qDebug() << "NotifiableService::listenToNotificationsAt" << this << notificationPath;
  websocket.close();
  http.listen(notificationPath, websocket);
  withNotifications = true;
  disconnect(this, &HttpService::pathChanged, this, &NotifiableService::listenToNotifications);
}

void NotifiableService::receivedNotificationError(QAbstractSocket::SocketError error)
{
  qDebug() << "NotifiableService::receivedNotificationError" << this << ":" << error;
}

void NotifiableService::receivedNotification(const QString& message)
{
  QJsonObject data = QJsonDocument::fromJson(message.toUtf8()).object();
  QJsonObject modelData = data["model"].toObject();
  QString operation = data["operation"].toString();
  QByteArray uid = modelData["_id"].toObject()["$oid"].toString().toUtf8();

  if (operation == "create")
  {
    qDebug() << this << "-> notification create model" << uid;
    loadFromJson(modelData);
    emit modelAdded(get(uid));
  }
  else
  {
    ModelType* model = get(uid);

    if (operation == "update" && model)
    {
      qDebug() << this << "-> notification updated model" << model->getUid();
      model->fromNotification(modelData);
      emit modelSaved(model);
    }
    else if (operation == "delete" && model)
    {
      qDebug() << this << "-> notification deleted model" << model->getUid();
      models.remove(uid);
      emit modelRemoved(model);
      cleanUpModel(model);
    }
    else
      qDebug() << this << "-> notification: nothing do to on" << model << operation;
  }
}
