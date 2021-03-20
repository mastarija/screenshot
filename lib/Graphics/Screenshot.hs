module Graphics.ScreenShot where
--
import Graphics.ScreenShot.Domain ( Screen , BitMap )
--

makeWindow :: Screen -> Int -> Int -> Int -> Int -> Maybe Window
makeWindow = undefined

screenList :: IO [ Screen ]
screenList = undefined

screenShot :: Screen -> Window -> IO ( Maybe BitMap )
screenShot = undefined
