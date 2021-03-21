# ScreenShot

When you just want to take the damn screenshot without pulling down the whole Hackage to your machine.

## Windows

Not much to say at the moment. For now here's just a list of used `win32` API functions for quick reference.

```
BOOL EnumDisplayMonitors(
  HDC             hdc,
  LPCRECT         lprcClip,
  MONITORENUMPROC lpfnEnum,
  LPARAM          dwData
);

HDC GetDC(
  HWND hWnd
);

HDC CreateCompatibleDC(
  HDC hdc
);

HBITMAP CreateCompatibleBitmap(
  HDC hdc,
  int cx,
  int cy
);

HGDIOBJ SelectObject(
  HDC     hdc,
  HGDIOBJ h
);

BOOL BitBlt(
  HDC   hdc,
  int   x,
  int   y,
  int   cx,
  int   cy,
  HDC   hdcSrc,
  int   x1,
  int   y1,
  DWORD rop
);

int GetDIBits(
  HDC          hdc,
  HBITMAP      hbm,
  UINT         start,
  UINT         cLines,
  LPVOID       lpvBits,
  LPBITMAPINFO lpbmi,
  UINT         usage
);

BOOL DeleteObject(
  HGDIOBJ ho
);

BOOL DeleteDC(
  HDC hdc
);

int ReleaseDC(
  HWND hWnd,
  HDC  hDC
);
```