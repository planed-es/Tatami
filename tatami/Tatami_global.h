#ifndef  TATAMI_GLOBAL_H
# define TATAMI_GLOBAL_H

# include <QtCore/qglobal.h>

# if defined(TATAMI_LIBRARY)
#  define TATAMI_EXPORT Q_DECL_EXPORT
# else
#  define TATAMI_EXPORT Q_DECL_IMPORT
#endif

#endif // TATAMI_GLOBAL_H
