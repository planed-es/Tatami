#ifndef  TATAMILINEMETRICS_H
# define TATAMILINEMETRICS_H

# include <QObject>
# include <QtQml/qqml.h>

class TatamiAdaptiveFontSize : public QObject
{
  Q_OBJECT
  QML_NAMED_ELEMENT(AdaptiveFontSize)
  Q_PROPERTY(QString text MEMBER m_text NOTIFY updated)
  Q_PROPERTY(QString fontName MEMBER m_fontName NOTIFY updated)
  Q_PROPERTY(double minSize MEMBER m_minSize NOTIFY updated)
  Q_PROPERTY(double maxSize MEMBER m_maxSize NOTIFY updated)
  Q_PROPERTY(double width MEMBER m_width NOTIFY updated)
  Q_PROPERTY(double fontPixelSize READ fontPixelSize NOTIFY updated)
public:
  TatamiAdaptiveFontSize(QObject* parent = nullptr) : QObject(parent) {}

  double fontPixelSize() const;

signals:
  void updated();

private:
  QString m_text;
  QString m_fontName;
  double  m_minSize = 1;
  double  m_maxSize = 0;
  double  m_width = 0;
};

#endif
