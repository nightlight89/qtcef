#ifndef CP_CEFVIEW_GLOBAL_H
#define CP_CEFVIEW_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CP_CEFVIEW_LIBRARY)
#  define CP_CEFVIEWSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CP_CEFVIEWSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CP_CEFVIEW_GLOBAL_H
