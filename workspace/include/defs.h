#pragma once
#if defined (_WIN32) || defined (_WIN62)
#define DllExport __declspec(dllexport)
#else
#define DllExport
#endif
