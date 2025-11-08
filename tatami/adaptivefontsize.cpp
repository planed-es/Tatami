#include "adaptivefontsize.h"
#include <QtGui/QFont>
#include <QtGui/QFontMetrics>

double TatamiAdaptiveFontSize::fontPixelSize() const
{
  if (m_width > 0)
  {
    QFont font(m_fontName, m_maxSize);

    for (double size = m_maxSize ; size > m_minSize ; --size)
    {
      font.setPixelSize(size);
      QFontMetrics metrics(font);
      int textWidth = metrics.horizontalAdvance(m_text);
      if (textWidth < m_width)
        return size;
    }
    return m_minSize;
  }
  return m_maxSize;
}
