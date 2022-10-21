function refreshLayout(columnsHeader, rowsHeader) {
  this.forceLayout();
  columnsHeader.model = rowsHeader.model = 0;
  columnsHeader.model = this.columns > 0 ? this.columns : 1;
  rowsHeader.model    = this.rows > 0 ? this.rows : 1;
}

function columnWidthProvider(index) {
  const fontName     = this.style.fontName;
  const pointSize    = this.style.fontPointSize;
  const propertyName = this.model.propertyAt(index);
  const containerWidth = this.parent.width;
  const totalWidth     = this.model.columnTotalWidth(fontName, pointSize) + this.model.columnCount * (this.cellMargin * 2);
  var   columnWidth    = this.model.maxWidthForProperty(propertyName, fontName, pointSize) + this.cellMargin * 2;
  const ratio          = totalWidth > containerWidth ? totalWidth / containerWidth : 1;

  if (propertyName == this.mainColumn) {
    const remainingWidth = (this.width - this.leftMargin) - totalWidth;

    if (remainingWidth > 0)
      columnWidth += remainingWidth;
  }
  return columnWidth / ratio - 1;
}

function rowHeightProvider(index) {
  return 50;
}

function formatDateNumber(value) {
  if (value > 9)
    return value;
  return '0' + value;
}

function formatDate(value) {
  return [
    formatDateNumber(value.getDate()),
    formatDateNumber(value.getMonth() + 1),
    value.getFullYear()
  ].join('-');
}

const displayByTypes = {
  supplierUids: function(value) { return i18n.t("customFields.supplierPicker.label", { count: value.length || 0 }); }
};

function defaultCellDisplayer(value, propertyName) {
  if (value === undefined) {
    console.warn("Property", propertyName, "is undefined");
    return "N/A";
  }
  if      (value === true)  { return i18n.t("true"); }
  else if (value === false) { return i18n.t("false"); }
  else if (value.constructor === Date) { return formatDate(value); }
  else if (displayByTypes[propertyName] != undefined)
    return displayByTypes[propertyName](value);
  return value.toString();
}

function offsetForRow(index) {
  return (this.rowHeightProvider() + this.rowSpacing) * index;
}

function ensureCurrentIndexVisibility(columnsHeader) {
  const offset = offsetForRow.bind(this)(this.currentIndex);
  const visibleZone = {
    from: this.contentY,
    to: this.contentY + this.height
  };

  if (offset < visibleZone.from)
    this.contentY = offset + this.height - rowHeightProvider.bind(this)();
  else if (offset > visibleZone.to)
    this.contentY = offset - columnsHeader.height;
}
