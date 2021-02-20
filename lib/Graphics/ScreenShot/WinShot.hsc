{-# LANGUAGE ForeignFunctionInterface #-}
--
module Graphics.ScreenShot.WinShot where
--
import Foreign
import Foreign.C.Types
--
#include "WinShot.h"

data CScreen = CScreen
  { cx :: !CInt
  , cy :: !CInt
  , cw :: !CInt
  , ch :: !CInt
  } deriving ( Show )

data CScreenList = CScreenList
  { count :: !CInt
  , items :: !( Ptr CScreen )
  } deriving ( Show )

instance Storable CScreen where
  sizeOf _ = #{size Screen}
  alignment _ = #{alignment Screen}
  peek p = CScreen
    <$> ( #{peek Screen, x} p )
    <*> ( #{peek Screen, y} p )
    <*> ( #{peek Screen, w} p )
    <*> ( #{peek Screen, h} p )
  poke p v = do
    #{poke Screen, x} p $ cx v
    #{poke Screen, y} p $ cy v
    #{poke Screen, w} p $ cw v
    #{poke Screen, h} p $ ch v

