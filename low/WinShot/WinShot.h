#include "AllShot.h"
#include <windows.h>

#ifndef WIN_SHOT
#define WIN_SHOT

BOOL ss_ScreenEnum ( HMONITOR Arg1 , HDC Arg2 , LPRECT Arg3 , LPARAM Arg4 );
BOOL ss_ScreenItem ( HMONITOR Arg1 , HDC Arg2 , LPRECT Arg3 , LPARAM Arg4 );

#endif
