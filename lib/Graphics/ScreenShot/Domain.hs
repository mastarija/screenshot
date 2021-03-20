module Graphics.ScreenShot.Domain where
--
import Data.Word ( Word8 )
--

data Window = Window
  { x :: Int
  , y :: Int
  , w :: Int
  , h :: Int
  }

data Screen = Screen
  { i :: Int
  , x :: Int
  , y :: Int
  , w :: Int
  , h :: Int
  }

data ScreenShot = ScreenShot
  { bits :: [ Word8 ]
  }
