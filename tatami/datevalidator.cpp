#include "datevalidator.h"
#include <QDate>

static bool isNumber(char value)
{
  return value >= '0' && value <= '9';
}

TatamiDateValidator::TatamiDateValidator(QObject* parent) : QValidator(parent)
{
  updateRegExp();
}

void TatamiDateValidator::updateRegExp()
{
  QString exp;

  exp  = "\\d{1,2}";
  exp += separator;
  exp += "\\d{1,2}";
  exp += separator;
  exp += "\\d{1,4}$";
  regExp = QRegularExpression(exp);
}

QValidator::State TatamiDateValidator::validate(QString& input, int& pos) const
{
  if (input.indexOf(regExp) == 0)
  {
    bool validDate = QDate::fromString(input, "dd-MM-yyyy").isValid();
    return validDate ? QValidator::Acceptable : QValidator::Intermediate;
  }
  return QValidator::Invalid;
}
