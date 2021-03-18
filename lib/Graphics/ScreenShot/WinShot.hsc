{-# LANGUAGE ForeignFunctionInterface #-}
--
#include "WinShot.h"
--
module Graphics.ScreenShot.WinShot where
--
import Foreign
import Foreign.C.Types
--
import Graphics.ScreenShot.Domain
--

data CScreen = CScreen
  { _cs_x :: !CInt
  , _cs_y :: !CInt
  , _cs_w :: !CInt
  , _cs_h :: !CInt
  } deriving ( Show )

peekCScreen :: Ptr CScreen -> IO CScreen
peekCScreen p = CScreen
  <$> ( #{ peek struct Screen , x } p )
  <*> ( #{ peek struct Screen , y } p )
  <*> ( #{ peek struct Screen , w } p )
  <*> ( #{ peek struct Screen , h } p )

--

data CScreenList = CScreenList
  { _cs_count :: !CInt
  , _cs_items :: !( Ptr CScreen )
  } deriving ( Show )

peekCScreenList :: Ptr CScreenList -> IO CScreenList
peekCScreenList p = CScreenList
  <$> ( #{ peek struct ScreenList , count } p )
  <*> ( #{ peek struct ScreenList , items } p )

--

data CScreenShot = CScreenShot
  { _cs_image :: !( Ptr CUChar )
  , _cs_bytes :: !CULong
  } deriving ( Show )

peekCSCreenShot :: Ptr CScreenShot -> IO CScreenShot
peekCSCreenShot p = CScreenShot
  <$> ( #{ peek struct ScreenShot , image } p )
  <*> ( #{ peek struct ScreenShot , bytes } p )

--

foreign import ccall "ScreenList"
  _cs_ScreenList :: IO ( Ptr CScreenList )

foreign import ccall "ScreenShot"
  _cs_ScreenShot :: IO ( Ptr CScreenShot )

foreign import ccall "&FreeScreenList"
  _cs_p_FreeScreenList :: FunPtr ( CScreenList -> IO () )

foreign import ccall "&FreeScreenShot"
  _cs_p_FreeScreenShot :: FunPtr ( CScreenShot -> IO () )

--

screenList :: IO ScreenList
screenList = do
  sl <- _cs_ScreenList >>= peekCScreenList
  undefined
