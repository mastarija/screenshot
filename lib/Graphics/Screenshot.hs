{-# LANGUAGE DeriveGeneric #-}
--
module Graphics.ScreenShot where
--

data Screen = Screen
  { x :: !Int
  , y :: !Int
  , w :: !Int
  , h :: !Int
  }

data ScreenShot = ScreenShot
  {
  }
