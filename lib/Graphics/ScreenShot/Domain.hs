module Graphics.ScreenShot.Domain where
--

data Screen = Screen
  { x :: !Int
  , y :: !Int
  , w :: !Int
  , h :: !Int
  } deriving ( Show )

type ScreenList = [ Screen ]
