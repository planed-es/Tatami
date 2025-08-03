#ifndef  TATAMIMULTIPLEPICKER_H
# define TATAMIMULTIPLEPICKER_H

# include "../edit_controller.h"

class TATAMI_EXPORT TatamiMultiplePickerController : public TatamiEditController
{
  Q_OBJECT
  Q_PROPERTY(QStringList value MEMBER value NOTIFY valueChanged)
public:
  explicit TatamiMultiplePickerController(QObject *parent = nullptr);

signals:
  void valueChanged();

private:
  bool filter(const ModelType*) const override;

  QStringList value;
};

#endif
