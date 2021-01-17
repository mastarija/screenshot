#include <windows.h>

typedef struct
  { long x;
    long y;
    long w;
    long h;
  } Screen;

typedef struct
  { unsigned char size;
    Screen        list[1];
  } ScreenList;

ScreenList* ListScreens();

BOOL ScreenCounter( HMONITOR hMonitor , HDC hDC , LPRECT pRect , LPARAM aSize );
BOOL ScreenFetcher( HMONITOR hMonitor , HDC hDC , LPRECT pRect , LPARAM aScreenList );
