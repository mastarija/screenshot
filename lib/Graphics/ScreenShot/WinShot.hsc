{-# LANGUAGE RecordWildCards #-}
{-# LANGUAGE ForeignFunctionInterface #-}
--
module Graphics.ScreenShot.WinShot where
--
import Foreign
import Foreign.C.Types
--
import Data.ByteString as BS
--
import Graphics.ScreenShot as SS
--

#include "WinShot.h"

foreign import ccall "ScreenShot"
  c_ScreenShot :: CInt -> CInt -> CInt -> CInt -> IO ( Ptr CHBitmap )

foreign import ccall "EnumScreens"
  c_EnumScreens :: IO ( Ptr CHScreenList )

--

data CHBitmap = CHBitmap
  { cimage :: !( Ptr ByteString ) -- pointer to the image data
  , cbytes :: !CULong             -- number of image bytes
  } deriving ( Eq, Show )

peekCHBitmap :: Ptr CHBitmap -> IO CHBitmap
peekCHBitmap p = CHBitmap
  <$> (#{peek HBitmap, image} p)
  <*> (#{peek HBitmap, bytes} p)

--

data CHScreen = CHScreen
  { cvsx :: !CLong -- virtual screen x coordinate
  , cvsy :: !CLong -- virtual screen y coordinate
  , cvsw :: !CLong -- virtual screen width
  , cvsh :: !CLong -- virtual screen height
  } deriving ( Eq, Show )

instance Storable CHScreen where
  sizeOf    _ = #{size HScreen}
  alignment _ = #{alignment HScreen}
  peek p      = CHScreen
    <$> ( #{peek HScreen, vsx} p )
    <*> ( #{peek HScreen, vsy} p )
    <*> ( #{peek HScreen, vsw} p )
    <*> ( #{peek HScreen, vsh} p )
  poke p v    = do
    #{poke HScreen, vsx} p $ cvsx v
    #{poke HScreen, vsy} p $ cvsy v
    #{poke HScreen, vsw} p $ cvsw v
    #{poke HScreen, vsh} p $ cvsh v

--

data CHScreenList = CHScreenList
  { ccnt     :: !CInt              -- number of screens in a list
  , cscreens :: !( Ptr CHScreen )  -- pointer to a screen array
  } deriving ( Show )

peekCHScreenList :: Ptr CHScreenList -> IO CHScreenList
peekCHScreenList p = CHScreenList
  <$> ( #{peek HScreenList, cnt} p )
  <*> ( pure $ #{ptr HScreenList, screens} p )

--

sc :: ScreenShot
sc = ScreenShot {..}
  where screenEnum :: IO [Screen]
        screenEnum = do
          p <- c_EnumScreens >>= newForeignPtr finalizerFree
          withForeignPtr p helper
          where helper :: Ptr CHScreenList -> IO [ Screen ]
                helper p = do
                  CHScreenList{..} <- peekCHScreenList p
                  fmap screenFromCHScreen <$> peekArray (fromIntegral ccnt) cscreens

                screenFromCHScreen :: CHScreen -> Screen
                screenFromCHScreen CHScreen{..} = Screen
                  (fromIntegral cvsx)
                  (fromIntegral cvsy)
                  (fromIntegral cvsw)
                  (fromIntegral cvsh)

        screenShot :: Screen -> IO Bitmap
        screenShot Screen{..} = do
          sp <- c_ScreenShot
            (fromIntegral x)
            (fromIntegral y)
            (fromIntegral w)
            (fromIntegral h) >>= newForeignPtr finalizerFree
          withForeignPtr sp helper
          where helper :: Ptr CHBitmap -> IO Bitmap
                helper p = do
                  CHBitmap{..}  <- peekCHBitmap p
                  fpimage       <- newForeignPtr finalizerFree cimage
                  bs <- withForeignPtr fpimage $ \ pimage -> do
                    bitearr <- peekArray (fromIntegral cbytes) (castPtr pimage)
                    pure $ pack bitearr
                  pure $ Bitmap bs

        screenSave :: FilePath -> Bitmap -> IO ()
        screenSave fp bmp = BS.writeFile fp (unBitmap bmp)