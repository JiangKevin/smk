#pragma once
#include <emscripten.h>

EM_JS( int, get_windows_w, (), { return window.innerWidth; } );

EM_JS( int, get_windows_h, (), { return window.innerHeight; } );