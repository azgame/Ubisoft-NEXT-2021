// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: reference additional headers your program requires here
#include "App/app.h"
#include "Utility/ScopedTimer.h"

// Set of universally used macros for convenience
#define SAFE_DELETE( x ) { if( x ) delete x; x = NULL; }
#define SAFE_DELETE_ARRAY( x ) { if( x ) delete[] x; x = NULL; }

#define __FILENAME__ (strrchr(__FILE__,'\\')+1)

#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#ifdef ASSERT
#undef ASSERT
#endif

#ifdef LOG
#undef LOG
#endif

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#ifdef RELEASE
#define ASSERT(expression,...) (void)(expression)
#define LOG(...) (void)()
#define ERROR(...) (void)()
#define BREAKPOINT() (void)()
#define SCOPEDTIMER(name) (void)(name)
#else //--Debug

#define ASSERT(expression,...) \
		if (!(bool)(expression)) { \
			App::Print(10, 400, __VA_ARGS__); \
	}
			//__debugbreak(); 

#define BREAKPOINT() __debugbreak();

#define SCOPEDTIMER(name) ScopedTimer name(__FUNCTION__, __FILENAME__, __LINE__);

#endif