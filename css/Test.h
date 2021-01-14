#include <windows.h>

typedef struct
  { long sx;
    long sy;
    long sw;
    long sh;
  } Screen;

typedef struct
  { unsigned char size;
    Screen        list[1];
  } ScreenList;

ScreenList* ListScreens();

BOOL ScreenCounter( HMONITOR hMonitor, HDC hDC, LPRECT pRect, LPARAM pCnt );
