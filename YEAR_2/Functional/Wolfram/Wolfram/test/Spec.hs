
import Test.HUnit
import GtArgs (checkValue, checkValueNeg, getOptions)
import Display (resizeLine, resizeMove)
import Rules (generate)

-- === Tests pour GtArgs ===
testCheckValue :: Test
testCheckValue = TestList
    [ "checkValue '123'" ~: checkValue "123" ~?= True
    , "checkValue 'abc'" ~: checkValue "abc" ~?= False
    , "checkValue '12a3'" ~: checkValue "12a3" ~?= False
    , "checkValue ''" ~: checkValue "" ~?= False
    ]

testCheckValueNeg :: Test
testCheckValueNeg = TestList
    [ "checkValueNeg '-5'" ~: checkValueNeg "-5" ~?= True
    , "checkValueNeg '0'" ~: checkValueNeg "0" ~?= True
    , "checkValueNeg '10'" ~: checkValueNeg "10" ~?= True
    , "checkValueNeg '--5'" ~: checkValueNeg "--5" ~?= False
    ]

testGetOptions :: Test
testGetOptions = TestList
    [ "Valid --rule" ~: getOptions ["--rule", "30"] ["0", "0", "-1", "80", "0"] ~?= ["30", "0", "-1", "80", "0"]
    , "Invalid --rule" ~: getOptions ["--rule", "abc"] ["0", "0", "-1", "80", "0"] ~?= ["err", "0", "-1", "80", "0"]
    , "Valid --move" ~: getOptions ["--move", "-5"] ["0", "0", "-1", "80", "0"] ~?= ["0", "0", "-1", "80", "-5"]
    , "Unknown option" ~: getOptions ["--unknown", "10"] ["0", "0", "-1", "80", "0"] ~?= ["null"]
    ]

-- === Tests pour Display ===
testResizeLine :: Test
testResizeLine = TestList
    [ "resizeLine '***' 0" ~: resizeLine "***" 0 ~?= "***"
    , "resizeLine '***' 2" ~: resizeLine "***" 2 ~?= "  ***"
    , "resizeLine '***' (-2)" ~: resizeLine "***" (-2) ~?= "*"
    ]

testResizeMove :: Test
testResizeMove = TestList
    [ "resizeMove '***' 0" ~: resizeMove "***" 0 ~?= "***"
    , "resizeMove '***' 2" ~: resizeMove "***" 2 ~?= "***  "
    , "resizeMove '***' (-2)" ~: resizeMove "***" (-2) ~?= "*"
    ]

-- === Tests pour Rules ===
testGenerate :: Test
testGenerate = TestList
    [ "generate Rule 30" ~: generate 30 "  *  " ~?= " * * "
    , "generate Rule 90" ~: generate 90 "  *  " ~?= " * * "
    , "generate Rule 110" ~: generate 110 "  *  " ~?= "**  "
    ]

-- === Liste des tests ===
main :: IO Counts
main = runTestTT $ TestList
    [ testCheckValue
    , testCheckValueNeg
    , testGetOptions
    , testResizeLine
    , testResizeMove
    , testGenerate
    ]
