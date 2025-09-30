#include "sessionanalytics.h"
#include "httpclient.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <filesystem>
#include <QDebug>
#define MAX_SESSION_HISTORY 20

/*
 * Session
 */
static TatamiSessionAnalytics* _singleton = nullptr;

void clearOldSessions()
{
  QStringList storedSessions = TatamiSessionAnalytics::storedSessions();

  while (storedSessions.size() > MAX_SESSION_HISTORY)
  {
    auto path = TatamiSessionAnalytics::storageDirectory().filesystemPath() / storedSessions.takeFirst().toStdString();
    QFile::remove(path);
  }
}

void submitLatestSession()
{
  const QStringList storedSessions = TatamiSessionAnalytics::storedSessions();

  if (storedSessions.size())
  {
    auto path = TatamiSessionAnalytics::storageDirectory().filesystemPath() / storedSessions.constLast().toStdString();
    QFile file(path);

    if (file.open(QIODevice::ReadOnly))
    {
      HttpClient* client = new HttpClient;

      qDebug() << "TatamiSessionAnalytics: sending last session";
      client->post("/client-sessions", QJsonDocument::fromJson(file.readAll()), [&](QNetworkReply*)
      {
        qDebug() << "TatamiSessionAnalytics: sent last session";
        // we gonna need to delete client at some point, or maybe not, its not that critical
      });
    }
  }
  else
    qDebug() << "TatamiSessionAnalytics: no previous session to submit";
}

TatamiSessionAnalytics::TatamiSessionAnalytics(QObject* parent) : QObject(parent)
{
  QString filename = "session_" + QDateTime::currentDateTime().toString("yyyy-MM-dd-hhmmss");

  qDebug() << "TatamiSessionAnalytics: storageDirectory=" << storageDirectory();
  Q_ASSERT(_singleton == nullptr);
  _singleton = this;
  storageFilepath = QByteArray((storageDirectory().filesystemAbsolutePath() / filename.toStdString()).string());
  submitLatestSession();
  clearOldSessions();
}

TatamiSessionAnalytics::~TatamiSessionAnalytics()
{
  _singleton = nullptr;
}

TatamiSessionAnalytics* TatamiSessionAnalytics::singleton()
{
  if (!_singleton)
    _singleton = new TatamiSessionAnalytics();
  return _singleton;
}

void TatamiSessionAnalytics::registerFunnel(TatamiSessionFunnel* funnel)
{
  if (funnel)
  {
    connect(funnel, &TatamiSessionFunnel::updated, this, &TatamiSessionAnalytics::save);
    connect(funnel, &TatamiSessionFunnel::beforeDestroy, this, &TatamiSessionAnalytics::unregisterFunnel);
    funnels.push_back(Funnel(funnel));
    qDebug() << "TatamiSessionAnalytics: registered funnel" << funnel->property("type").toByteArray();
    save();
  }
}

void TatamiSessionAnalytics::unregisterFunnel(TatamiSessionFunnel* funnel)
{
  auto it = std::find(funnels.begin(), funnels.end(), funnel);

  if (it != funnels.end())
  {
    it->data = funnel->toJson();
    save();
  }
}

QString TatamiSessionAnalytics::toJson() const
{
  QJsonArray json;

  for (const Funnel& funnel : funnels)
    json << funnel.toJson();
  return QJsonDocument(json).toJson();
}

void TatamiSessionAnalytics::save()
{
  QFile output(storageFilepath);

  std::filesystem::create_directories(storageDirectory().filesystemPath());
  if (output.open(QIODevice::WriteOnly))
  {
    output.write(toJson().toUtf8());
  }
  else
    qDebug() << "TatamiSessionAnalytics: failed to create file" << storageFilepath;
}

QDir TatamiSessionAnalytics::storageDirectory()
{
  std::filesystem::path localData = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation).toStdString();
  return (localData / "sessionAnalytics");
}

QStringList TatamiSessionAnalytics::storedSessions()
{
  return storageDirectory().entryList(QDir::Files, QDir::Time | QDir::Reversed);
}

/*
 * Funnel
 */
TatamiSessionFunnel::TatamiSessionFunnel(QObject* parent) : QObject(parent)
{
  auto* singleton = TatamiSessionAnalytics::singleton();

  openedAt = QDateTime::currentDateTime();
  if (singleton)
    singleton->registerFunnel(this);
  else
    qDebug() << "TatamiSessionFunnel created, but TatamiSessionAnalytics singleton unavailable";
}

TatamiSessionFunnel::~TatamiSessionFunnel()
{
  if (!closed)
    close();
  emit beforeDestroy(this);
}

QJsonObject TatamiSessionFunnel::toJson() const
{
  QJsonArray eventJson;
  QVariantMap json;

  json["type"] = type;
  json["openedAt"] = openedAt;
  if (closed)
    json["closedAt"] = closedAt;
  if (events.size())
  {
    for (const Event& event : events)
    {
      eventJson << QJsonObject::fromVariantMap({
        {"type",   event.type},
        {"time",   event.time},
        {"params", event.parameters}
      });
    }
    json["events"] = eventJson;
  }
  return QJsonObject::fromVariantMap(json);
}

void TatamiSessionFunnel::trigger(const QByteArray& type, const QVariantMap& params)
{
  events << Event{
    QDateTime::currentDateTime(),
    type,
    params
  };
}

void TatamiSessionFunnel::close()
{
  closed = true;
  closedAt = QDateTime::currentDateTime();
}
