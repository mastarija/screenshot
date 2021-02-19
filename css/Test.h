#include <windows.h>

struct Bitmap
  { void*         image;
    unsigned long bytes;
  };

struct Screen
  { int x;
    int y;
    int w;
    int h;
  };

struct ScreenList
  { unsigned char size;
    struct Screen list[1];
  };

struct ScreenList* ScreenList();
struct Bitmap*     ScreenShot( int x , int y , int w , int h );

BOOL ScreenCounter( HMONITOR hMonitor , HDC hDC , LPRECT pRect , LPARAM aSize );
BOOL ScreenFetcher( HMONITOR hMonitor , HDC hDC , LPRECT pRect , LPARAM aScreenList );
