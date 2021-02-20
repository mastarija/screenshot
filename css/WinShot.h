#include <windows.h>

////////////////////////////////////////////////////////////////////////////////

/*
  This data structure provides information about the virtual screens. It
  consists of X and Y coordinates of the virtual screens on a "mega screen" and
  their respective width and height.
*/
struct Screen
  { int x;
    int y;
    int w;
    int h;
  };

/*
  Data structure used to pass information about an inmemory bitmap. It contains
  a pointer to the bitmap data and a number of bytes that make up the image (or
  how many bytes of data should one read from the image pointer to get the full
  image).
*/
struct ScreenShot
  { void*         image;
    unsigned long bytes;
  };

/*
  A "dynamic" list of screen sizes. It contains a variable with number of
  elements in a list and a list of screen values. It is used to enumerate
  available screens.
*/
struct ScreenList
  { unsigned char count;
    struct Screen items[];
  };

////////////////////////////////////////////////////////////////////////////////

/*
  Returns a list of all detected virtual screens. This is mostly a utility
  function intended to be used with 'ScreenShot'.
*/
struct ScreenList* ScreenList();

/*
  Grabs an area of pixels defined by width, height and XY coordinates, and
  returns them in the bitmap format.
*/
struct ScreenShot* ScreenShot( int x , int y , int w , int h );

////////////////////////////////////////////////////////////////////////////////

/*
  Internal utility for cleaning up the allocated 'ScreenList' memory.
*/
void FreeScreenList( struct ScreenList* screenList );

/*
  Internal utility for cleaning up the allocated 'ScreenShot' memory.
*/
void FreeScreenShot( struct ScreenShot* screenShot );

/*
  Internal utility function used to get the number of virtual screens through
  Win32 API.
*/
BOOL  ScreenCounter( HMONITOR hMonitor , HDC hDC , LPRECT pRect , LPARAM aSize );

/*
  Internal utility function used to get the list (HScreenList) of relevant
  information on each of virtual screens through Win32 API.
*/
BOOL  ScreenFetcher( HMONITOR hMonitor , HDC hDC , LPRECT pRect , LPARAM aScreenList );

/*
  Internal utility function used to generate a proper bitmap format.
*/
DWORD ColorTableSize( BITMAPINFOHEADER* h );
