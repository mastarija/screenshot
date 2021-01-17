#include <stdio.h>
#include <windows.h>
#include "Test.h"

int main()
{
  ScreenList* list = ListScreens();

  printf("size : %d\n", list->size );

  for ( int i = 0 ; i < list->size ; i++ ) {
    Screen s = list->list[ i ];
    printf( "screen %d : <x , y> ( %d , %d ) - <w , h> ( %d , %d )\n" , i , s.x , s.y , s.w , s.h );
  }
}

// increase the value on 'aSizen' address
BOOL ScreenCounter( HMONITOR hMonitor, HDC hDC, LPRECT pRect, LPARAM aSize )
{
  int* pSize = (int*) aSize;  // cast the address into a pointer
  (*pSize)++;                 // increase size through a pointer
  return TRUE;                // continue iterating over screens
}

// fetch width , height and position of a virtual screen
BOOL ScreenFetcher( HMONITOR hMonitor , HDC hDC , LPRECT pRect , LPARAM aScreenList ) {
  ScreenList* list = ( ScreenList* ) aScreenList; // cast the address into a pointer
  int         indx = list->size;                  // get the current index / size

  MONITORINFO info;                                // monitor info temp var
              info.cbSize = sizeof( MONITORINFO ); // needed for G...M...InfoA

  // get the screen info
  GetMonitorInfoA( hMonitor , &info );

  // store relevant screen info into the list at index 'indx'
  list->list[indx].x = info.rcMonitor.left;
  list->list[indx].y = info.rcMonitor.top;
  list->list[indx].w = abs( info.rcMonitor.left - info.rcMonitor.right  );
  list->list[indx].h = abs( info.rcMonitor.top  - info.rcMonitor.bottom );

  list->size++; // increase the list index / size
  return TRUE;  // continue iterating over screens
}

// get the list of detected screens
ScreenList* ListScreens()
{
  int          size = 0;    // size of the list
  ScreenList*  list = NULL; // list of detected screens

  // run through detected screens increasing the 'size' of the list
  EnumDisplayMonitors( NULL , NULL , ScreenCounter , ( LPARAM ) &size );

  // allocate enough memory to store all of the detected screens' data
  list = (ScreenList*) malloc( sizeof( int ) + size * sizeof( Screen ) );


  // return null if memory allocation fails for some reason
  if ( list == NULL ) return NULL;

  // prepare the screen list size value for the 'ScreenDetailer' function
  list->size = 0;

  // load position and size of each monitor into the list
  EnumDisplayMonitors( NULL , NULL , ScreenFetcher , ( LPARAM ) list );

  // return the pointer to the 'ScreenList'
  return list;
}

