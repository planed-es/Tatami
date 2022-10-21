#include "pagination.h"

PaginationComponent::PaginationComponent(QObject* parent) : RequestCountComponent(parent)
{
  connect(this, &PaginationComponent::countChanged,        this, &PaginationComponent::updatePagination);
  connect(this, &PaginationComponent::itemsPerPageChanged, this, &PaginationComponent::updatePagination);
}

void PaginationComponent::updatePagination()
{
  if (itemsPerPage > 0)
  {
    unsigned int currentCount = count();
    unsigned int total        = currentCount > 0 ? currentCount - 1 : 0;
    unsigned int newValue     = total / itemsPerPage;

    if (newValue != maxPage)
    {
      maxPage = newValue;
      emit maxPageChanged();
    }
  }
}

void PaginationComponent::setCurrentPage(unsigned int value)
{
  if (page != value && value <= maxPage + 1)
  {
    page = value;
    emit pageChanged();
  }
}
