#ifndef  PAGINATION_COMPONENT_H
# define PAGINATION_COMPONENT_H

# include "requestcount.h"

class TATAMI_EXPORT PaginationComponent : public RequestCountComponent
{
  Q_OBJECT
  Q_PROPERTY(unsigned int count        READ count          NOTIFY countChanged)
  Q_PROPERTY(unsigned int maxPage      MEMBER maxPage      NOTIFY maxPageChanged)
  Q_PROPERTY(unsigned int itemsPerPage MEMBER itemsPerPage NOTIFY itemsPerPageChanged)
  Q_PROPERTY(unsigned int page         READ currentPage WRITE setCurrentPage NOTIFY pageChanged)
public:
  explicit PaginationComponent(QObject* parent = nullptr);

  virtual unsigned int count() const = 0;
  void                 setCurrentPage(unsigned int value);
  unsigned int         currentPage()     const { return page; }
  unsigned int         getMaxPage()      const { return maxPage; }
  unsigned int         getItemsPerPage() const { return itemsPerPage; }

signals:
  void countChanged();
  void pageChanged();
  void maxPageChanged();
  void itemsPerPageChanged();

public slots:
  void updatePagination();

protected:
  unsigned int page         = 0;
  unsigned int maxPage      = 0;
  unsigned int itemsPerPage = 10;
};

#endif
