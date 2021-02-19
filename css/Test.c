#include <stdio.h>
#include <windows.h>
#include "Test.h"

int main()
{
  printf("hello world\n");

  struct ScreenList* list = ScreenList();

  printf("size : %d\n", list->size );

  for ( int i = 0 ; i < list->size ; i++ )
  {
    struct Screen s = list->list[ i ];
    printf( "screen %d : <x , y> ( %d , %d ) - <w , h> ( %d , %d )\n" , i , s.x , s.y , s.w , s.h );
  }
}

////////////////////////////////////////////////////////////////////////////////

// get the list of detected screens
struct ScreenList* ScreenList()
{
  int                 size = 0;    // size of the list
  struct ScreenList*  list = NULL; // list of detected screens

  // run through detected screens increasing the 'size' of the list
  EnumDisplayMonitors( NULL , NULL , ScreenCounter , (LPARAM) &size );

  // allocate enough memory to store all of the detected screens' data
  list = (struct ScreenList*) malloc( sizeof(int) + size * sizeof(struct Screen) );

  // return null if memory allocation fails for some reason
  if ( list == NULL ) return NULL;

  // prepare the screen list size value for the 'ScreenDetailer' function
  list->size = 0;

  // load position and size of each monitor into the list
  EnumDisplayMonitors( NULL , NULL , ScreenFetcher , (LPARAM) list );

  // return the pointer to the 'ScreenList'
  return list;
}

struct Bitmap* ScreenShot( int x , int y , int w , int h )
{
  struct Bitmap*    result            = NULL;

  HDC               hScreen           = GetDC( NULL ); // whole screen device context
  HDC               hScreenMemory     = CreateCompatibleDC( hScreen ); // compatible DC in memory

  HBITMAP           hBitmap           = CreateCompatibleBitmap( hScreen , w , h ); // new bitmap for DC
  HBITMAP           hBitmapOld        = (HBITMAP) SelectObject( hScreenMemory , hBitmap ); // we select new bitmap to in-memory DC and store a reference to the old one

  BITMAPINFO*       pBitmapInfo       = (BITMAPINFO*)       malloc( sizeof( BITMAPINFOHEADER ) );
  BITMAPFILEHEADER* pBitmapFileHeader = (BITMAPFILEHEADER*) malloc( sizeof( BITMAPFILEHEADER ) );

  BYTE*             pPixelBuffer      = NULL;
  BYTE*             pBitmapBuffer     = NULL;

  DWORD             nColors           = 0;

  DWORD             sBitmapInfo       = sizeof( BITMAPINFOHEADER );
  DWORD             sPixelBuffer      = 0;
  DWORD             sBitmapFileHeader = sizeof( BITMAPFILEHEADER );

  return result;
}

////////////////////////////////////////////////////////////////////////////////

// increase the value on 'aSize' address
BOOL ScreenCounter( HMONITOR hMonitor, HDC hDC, LPRECT pRect, LPARAM aSize )
{
  int* pSize = (int*) aSize;  // cast the address into a pointer
  (*pSize)++;                 // increase size through a pointer
  return TRUE;                // continue iterating over screens
}

// fetch width , height and position of a virtual screen
BOOL ScreenFetcher( HMONITOR hMonitor , HDC hDC , LPRECT pRect , LPARAM aScreenList )
{
  struct ScreenList* list = (struct ScreenList*) aScreenList; // cast the address into a pointer
  int                indx = list->size;                       // get the current index / size

  MONITORINFO info;                              // monitor info temp var
              info.cbSize = sizeof(MONITORINFO); // needed for G...M...InfoA

  // get the screen info
  GetMonitorInfoA( hMonitor , &info );

  // store relevant screen info into the list at index 'indx'
  list->list[indx].x = (int) info.rcMonitor.left;
  list->list[indx].y = (int) info.rcMonitor.top;
  list->list[indx].w = (int) abs( info.rcMonitor.left - info.rcMonitor.right  );
  list->list[indx].h = (int) abs( info.rcMonitor.top  - info.rcMonitor.bottom );

  list->size++; // increase the list index / size
  return TRUE;  // continue iterating over screens
}

