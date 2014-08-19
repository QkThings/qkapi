#ifndef QKAPI_GLOBAL_H
#define QKAPI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QKAPI_LIBRARY)
#  define QKAPISHARED_EXPORT Q_DECL_EXPORT
#else
#  define QKAPISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QKAPI_GLOBAL_H
