#ifndef  MODELSTORE_H
# define MODELSTORE_H

# include <QObject>
# include <QMap>
# include <QByteArray>
# include <metarecord-qt/metarecordnotifiable.h>
# include <metarecord-qt/vector.h>
# include "../Tatami_global.h"

class TATAMI_EXPORT ModelStore : public QObject
{
  Q_OBJECT
public:
  typedef MetaRecordNotifiable         ModelType;
  typedef QMap<QByteArray, ModelType*> Models;
  typedef QList<ModelType*>            ModelList;
  typedef QVector<ModelType*>          ModelArray;

  ModelStore(QObject* parent = nullptr);

  void               insert(ModelType* model);
  void               remove(ModelType* model);
  void               removeIf(std::function<bool(const ModelType&)>);
  unsigned int       count()  const { return static_cast<unsigned int>(models.size()); }
  QList<QByteArray>  uids()   const { return models.keys(); }
  ModelList          values() const { return models.values(); }
  void               clear(); // resets AND emits modelsChanged

  ModelType*         get(const QByteArray& uid) const;
  ModelType*         at(unsigned int index) const;
  ModelType*         findOne(const QVariantMap&, int comparaisonType = MetaRecordComparable::Identical) const;
  ModelArray         find(QVariantMap params, int comparaisonType = MetaRecordComparable::Identical) const;
  ModelArray         getByUid(const QStringList& uids) const;
  ModelArray         getByUid(const QList<QByteArray>& uids) const;
  ModelArray         filter(const QVariantMap& params) const;
  virtual ModelArray filter(std::function<bool (const ModelType*)>) const;

  template<typename MODEL>
  MODEL* getAs(const QByteArray& uid) const { return reinterpret_cast<MODEL*>(get(uid)); }

  template<typename ARRAY_TYPE>
  void loadFromArray(const ARRAY_TYPE& array)
  {
    for (auto* model : array)
      replaceModel(model);
    emit modelsChanged();
  }

signals:
  void modelsChanged();
  void modelAdded(ModelType*);
  void modelSaved(ModelType*);
  void modelRemoved(ModelType*);

protected:
  void        reset(); // clears all stored model, but does NOT emit modelsChanged
  void        replaceModel(ModelType* model);
  inline void cleanUpModel(ModelType* model) const { if (model->parent() == this) model->deleteLater(); }

  Models models;
private slots:
  void purgeModel(QObject*, QByteArray uid);
};

#endif // MODELSTORE_H
