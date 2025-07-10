#ifndef  TATAMIDATEVALIDATOR_H
# define TATAMIDATEVALIDATOR_H

# include "Tatami_global.h"
# include <QValidator>
# include <QRegularExpression>

class TATAMI_EXPORT TatamiDateValidator : public QValidator
{
  Q_PROPERTY(char separator MEMBER separator NOTIFY separatorChanged)
public:
  TatamiDateValidator(QObject* parent = nullptr);

  QValidator::State validate(QString& input, int& pos) const;

signals:
  void separatorChanged();

private slots:
  void updateRegExp();

private:
  char separator = '-';
  QRegularExpression regExp;
};

#endif
