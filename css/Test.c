#include <stdio.h>
#include <windows.h>
#include "Test.h"

int main(void)
{
  printf("Hello, World!\n");
}

// GetScreenList :: IO ( Maybe ScreenList )
ScreenList* ListScreens()
{
  int          size = 0;    // size of the list
  ScreenList*  list = NULL; // list of detected screens

  // run through detected screens increasing the 'size' of the list
  EnumDisplayMonitors( NULL , NULL , ScreenCounter , (LPARAM) &size );

  // allocate enough memory to store all of the detected screens data
  list = (ScreenList*) malloc( sizeof( int ) + size * sizeof( Screen ) );

  // return null if memory allocation fails
  if ( list == NULL ) return NULL;

  //

  return list;
}

BOOL ScreenCounter( HMONITOR hMonitor, HDC hDC, LPRECT pRect, LPARAM aSize )
{
  int* pSize = (int*) aSize;  // cast the address into a pointer
  (*pSize)++;                 // increase size through a pointer
  return TRUE;                // continue iterating over screens
}
