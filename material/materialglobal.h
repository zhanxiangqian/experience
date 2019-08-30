#ifndef __MATERIAL_GLOBAL_H__
#define __MATERIAL_GLOBAL_H__
#include <QtCore/qglobal.h>

#ifndef QT_STATIC
# if defined(MaterialLib_EXPORTS)
#  define MaterialLib_EXPORT Q_DECL_EXPORT
# else
#  define MaterialLib_EXPORT Q_DECL_IMPORT
# endif
#else
# define QTCLASSLIBRARY1_EXPORT
#endif

#endif