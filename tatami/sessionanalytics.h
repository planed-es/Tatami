#ifndef  TATAMIANALYTICS_H
# define TATAMIANALYTICS_H

# include <QString>
# include <QDateTime>
# include <QVariantMap>
# include <QJsonObject>
# include <QPointer>
# include <QDir>
# include <QtQml>

class TATAMI_EXPORT TatamiSessionFunnel : public QObject
{
  Q_OBJECT
  QML_NAMED_ELEMENT(SessionAnalytics)
  Q_PROPERTY(QByteArray type MEMBER type NOTIFY updated)

  struct Event
  {
    QDateTime   time;
    QByteArray  type;
    QVariantMap parameters;
  };
public:
  explicit TatamiSessionFunnel(QObject* parent = nullptr);
  ~TatamiSessionFunnel();

  QJsonObject toJson() const;

public slots:
  void trigger(const QByteArray& type, const QVariantMap& params = QVariantMap());
  void close();

signals:
  void updated();
  void beforeDestroy(TatamiSessionFunnel*);

private:
  QByteArray   type;
  QList<Event> events;
  bool         closed = false;
  QDateTime    openedAt, closedAt;
};

class TATAMI_EXPORT TatamiSessionAnalytics : public QObject
{
  Q_OBJECT

  struct Funnel
  {
    Funnel(TatamiSessionFunnel* funnel) : instance(funnel), address(static_cast<void*>(funnel)) {}

    QPointer<TatamiSessionFunnel> instance;
    std::optional<QJsonObject>    data;
    void*                         address;

    bool operator==(TatamiSessionFunnel* ptr) const { return address == static_cast<void*>(ptr); }
    QJsonObject toJson() const { return instance ? instance->toJson() : data.value_or(QJsonObject()); }
  };
public:
  explicit TatamiSessionAnalytics(QObject* parent = nullptr);
  ~TatamiSessionAnalytics();

  static TatamiSessionAnalytics* singleton();

  void registerFunnel(TatamiSessionFunnel*);
  void unregisterFunnel(TatamiSessionFunnel*);
  QString toJson() const;

  static QDir storageDirectory();
  static QStringList storedSessions();

public slots:
  void save();

private:
  QString storageFilepath;
  QList<Funnel> funnels;
};

#endif
