#ifndef LISTNAVIGATIONCOMPONENT_H
#define LISTNAVIGATIONCOMPONENT_H

#include <QtQml/QQmlListProperty>
#include "../tablemodel.h"
#include "../displayservice.h"
#include "pagination.h"

class TATAMI_EXPORT ListNavigationComponent : public PaginationComponent
{
  Q_OBJECT

  typedef DisplayService::ModelType ModelType;
  typedef QQmlListProperty<MetaRecordable> QmlModelList;
  Q_PROPERTY(TatamiTableModel*      table          READ getTable CONSTANT)
  Q_PROPERTY(QmlModelList           models         READ getModels                            NOTIFY modelsChanged)
  Q_PROPERTY(MetaRecordable*        currentItem    READ currentRecord WRITE setCurrentRecord NOTIFY currentIndexChanged)
  Q_PROPERTY(int                    currentIndex   READ currentIndex  WRITE setCurrentIndex  NOTIFY currentIndexChanged)
  Q_PROPERTY(int                    pageIndex      READ pageIndex     WRITE setPageIndex     NOTIFY currentIndexChanged)
  Q_PROPERTY(int                    emptyRowsCount READ emptyRowsCount                       NOTIFY modelsChanged)
public:
  ListNavigationComponent(QObject* parent = nullptr);

  DisplayService&        displayService()     { return m_displayService; }
  TatamiTableModel*      getTable()           { return &table; }
  QmlModelList           getModels()          { return table.modelList(); }
  ModelType*             currentItem()  const { return m_currentItem; }
  int                    pageIndex()    const { return table.models().indexOf(m_currentItem); }
  int                    currentIndex() const { return pageIndex() + currentPage() * getItemsPerPage(); }
  void                   setCurrentItem(const ModelType*);
  void                   setCurrentIndex(int value);
  void                   setPageIndex(int value);
  int                    emptyRowsCount() const;
  unsigned int           count() const override { return m_displayService.count(); }

  Q_INVOKABLE ModelType* modelAt(unsigned int) const;
  Q_INVOKABLE void       goToPage(unsigned int);
  Q_INVOKABLE void       nextPage();
  Q_INVOKABLE void       previousPage();

public slots:
  void                   updateNavigation();
  virtual void           updateDisplayedRows();

signals:
  void                   currentIndexChanged();
  void                   modelsChanged();

protected:
  virtual void           setDisplayedRows(QVector<ModelType*> list);

private slots:
  void                   updatePageIndex();

private:
  MetaRecordable* currentRecord() { return m_currentItem; }
  void            setCurrentRecord(MetaRecordable* value) { m_currentItem = reinterpret_cast<ModelType*>(value); emit currentIndexChanged(); }

  TatamiTableModel  table;
  ModelType*        m_currentItem = nullptr;
  DisplayService    m_displayService;
};

#endif // LISTNAVIGATIONCOMPONENT_H
