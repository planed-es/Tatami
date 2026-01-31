#ifndef  HTTPSERVICE_H
# define HTTPSERVICE_H

# include <QUrlQuery>
# include "jsonmodelstore.h"
# include "../httpclient.h"

class TATAMI_EXPORT HttpService : public JsonModelStore
{
  Q_OBJECT
public:
  typedef std::function<void ()> Callback;

  explicit HttpService(QObject* parent = nullptr);

  void setPath(const QUrl& url) { path = url.path().toUtf8(); urlQuery = QUrlQuery(url.query()); emit pathChanged(); }
  void setPath(const QByteArray& value) { path = value; emit pathChanged(); }
  void withPath(const QUrl& url, std::function<void()>);
  void withPath(const QByteArray& value,  std::function<void()> callback) { withPath(QUrl(value), callback); }

  QUrl getUrl() const { return getUrl(path); }
  QUrl getUrl(const QByteArray& path) const;
  const QByteArray& getPath() const { return path; }
  virtual QByteArray pathFor(const ModelType&) const;

  void fetch(Callback = Callback());
  void fetch(const QUrlQuery&, Callback = Callback());
  void fetch(const QJsonDocument&, Callback = Callback());
  void fetchUids(const QStringList& uids, Callback = Callback());
  void fetchUids(const QVector<QByteArray>& uids, Callback = Callback());
  void fetchOne(const QByteArray& uid) { fetchOne(uid, [](ModelType*){}); }
  void fetchOne(const QByteArray& uid, std::function<void (ModelType*)>);
  void save(ModelType* model) { save(model, [](){}); }
  virtual void save(ModelType*, std::function<void()> callback);
  virtual void destroy(ModelType*, std::function<void()> callback = std::function<void()>());
  virtual void destroyAll(std::function<void()> callback = std::function<void()>());

signals:
  void pathChanged();
  void requestStarted();
  void requestEnded();
  void fetchFailure(QString);
  void postFailure(QJsonObject);

protected:
  virtual void create(ModelType*, std::function<void()> callback);
  virtual void update(ModelType*, std::function<void()> callback);
  virtual void parseCreateResponse(ModelType*, HttpClient::ResponseObject*);

  virtual void receivedFetchReply(HttpClient::ResponseObject*, Callback = Callback(), bool withReset = true);
  virtual void receivedErrorReply(HttpClient::ResponseObject*);
  virtual void receivedCreateErrorReply(HttpClient::ResponseObject*);

  HttpClient  http;
private:
  QByteArray  path;
  QUrlQuery   urlQuery;

};

Q_DECLARE_INTERFACE(HttpService, "HttpService")

#endif // HTTPSERVICE_H
