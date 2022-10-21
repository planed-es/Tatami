#ifndef  TATAMI_GLOBAL_STATE_H
# define TATAMI_GLOBAL_STATE_H

# include <QObject>
# include "../Tatami_global.h"

class TATAMI_EXPORT TatamiGlobalState : public QObject
{
  Q_OBJECT
  Q_PROPERTY(unsigned int pendingRequestCount MEMBER pendingRequestCount NOTIFY pendingRequestCountChanged)

  static TatamiGlobalState* instance;
public:
  explicit TatamiGlobalState(QObject* parent = nullptr);
  virtual ~TatamiGlobalState();

  static TatamiGlobalState* get() { return instance; }

  void incrementPendingRequestCount() { pendingRequestCount++; emit pendingRequestCountChanged(); }
  void decrementPendingRequestCount() { pendingRequestCount--; emit pendingRequestCountChanged(); }

signals:
  void pendingRequestCountChanged();
private:
  unsigned int pendingRequestCount = 0;
};

#endif
