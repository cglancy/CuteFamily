#ifndef CUTEFAMILY_H
#define CUTEFAMILY_H

#include <QtGlobal>

#ifdef Q_OS_WIN32

#ifdef CUTEFAMILY_EXPORTS
#define CUTEFAMILY_API __declspec(dllexport)
#else
#define CUTEFAMILY_API __declspec(dllimport)
#endif

#else

#define CUTEFAMILY_API __attribute__((visibility("default")))

#endif

#endif // CUTEFAMILY_H
