#ifndef ALL_SHOT
#define ALL_SHOT

struct Screen
{
  int x;
  int y;
  int w;
  int h;
};

struct ScreenShot
{
  int size;
  int bits;
};

struct ScreenList
{
  int size;
  int list;
};

struct ScreenList ScreenList ();
struct ScreenShot ScreenShot ( struct Screen screen );

#endif
