#include "listnavigation.h"
#include <metarecord-qt/vector.h>

ListNavigationComponent::ListNavigationComponent(QObject* parent) : PaginationComponent(parent), table(this)
{
  connect(&m_displayService, &ModelStore::modelsChanged, this, &ListNavigationComponent::updateNavigation);
  connect(&m_displayService, &ModelStore::modelAdded,    this, &ListNavigationComponent::updateNavigation);
  connect(&m_displayService, &ModelStore::modelRemoved,  this, &ListNavigationComponent::updateNavigation);
  connect(this, &PaginationComponent::itemsPerPageChanged,   this, &ListNavigationComponent::updateNavigation);
  connect(this, &PaginationComponent::pageChanged,           this, &ListNavigationComponent::updateDisplayedRows);
  connect(&table, &TatamiTableModel::currentIndexChanged,    this, &ListNavigationComponent::updatePageIndex);
}

void ListNavigationComponent::setCurrentItem(const ModelType* model)
{
  int index = m_displayService.sortedValues().indexOf(const_cast<ModelType*>(model));

  if (index > 0)
    setCurrentIndex(index);
  else
    setCurrentIndex(0);
}

void ListNavigationComponent::setCurrentIndex(int value)
{
  setCurrentPage(value / getItemsPerPage());
  setPageIndex  (value % getItemsPerPage());
  table.setCurrentIndex(pageIndex());
}

void ListNavigationComponent::setPageIndex(int value)
{
  if (table.count() > 0)
  {
    if (value >= table.count()) { value = 0; }
    else if (value < 0) { value = table.count() - 1; }
    m_currentItem = reinterpret_cast<ModelType*>(table.models().at(value));
  }
  else
    m_currentItem = nullptr;
  emit currentIndexChanged();
}

void ListNavigationComponent::updateDisplayedRows()
{
  setDisplayedRows(
    m_displayService.page(currentPage(), getItemsPerPage())
  );
}

void ListNavigationComponent::setDisplayedRows(QVector<ModelType*> list)
{
  table.replaceRows(model_vector_cast<MetaRecordable>(list));
  emit modelsChanged();
}

void ListNavigationComponent::updateNavigation()
{
  emit countChanged(); // should call updatePagination
  updateDisplayedRows();
  setCurrentItem(m_currentItem);
}

int ListNavigationComponent::emptyRowsCount() const
{
  return getItemsPerPage() - table.count();
}

void ListNavigationComponent::updatePageIndex()
{
  if (pageIndex() != table.currentIndex())
    setPageIndex(table.currentIndex());
}

ListNavigationComponent::ModelType* ListNavigationComponent::modelAt(unsigned int index) const
{
  if (m_displayService.count() <= index)
    return nullptr;
  return m_displayService.sortedValues().at(index);
}

void ListNavigationComponent::goToPage(unsigned int page)
{
  if (page != currentPage())
    setCurrentIndex(page * getItemsPerPage());
}

void ListNavigationComponent::nextPage()
{
  goToPage(std::min(currentPage() + 1, getMaxPage()));
}

void ListNavigationComponent::previousPage()
{
  goToPage(std::max(static_cast<int>(currentPage()) - 1, 0));
}
