module Graphics.ScreenShot.Domain where
--
import Data.Word ( Word8 )
--

data Screen = Screen
  { x :: Int
  , y :: Int
  , w :: Int
  , h :: Int
  }

data ScreenShot = ScreenShot
  { bits :: [ Word8 ]
  }
