#pragma once

#ifdef ENGINE_EXPORTS			//MACRO
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif
