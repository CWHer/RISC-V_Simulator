@echo off

REM echo %time%
set /a startMS=%time:~9,2%
set /a startS=%time:~6,2%
set /a startM=%time:~3,2%
REM echo %startM% %startS% %startMS%

del in
copy testcases\array_test1.data .\in
echo array_test1
main.exe

del in
copy testcases\array_test2.data .\in
echo array_test2
main.exe

del in
copy testcases\basicopt1.data .\in
echo basicopt1
main.exe

del in
copy testcases\bulgarian.data .\in
echo bulgarian
main.exe

del in
copy testcases\expr.data .\in
echo expr
main.exe

del in
copy testcases\gcd.data .\in
echo gcd
main.exe

del in
copy testcases\hanoi.data .\in
echo hanoi
main.exe

del in
copy testcases\heart.data .\in
echo heart
main.exe

del in
copy testcases\lvalue2.data .\in
echo lvalue2
main.exe

del in
copy testcases\magic.data .\in
echo magic
main.exe

del in 
copy testcases\manyarguments.data .\in
echo manyarguments
main.exe

del in
copy testcases\multiarray.data .\in
echo multiarray
main.exe

del in
copy testcases\naive.data .\in
echo naive
main.exe

del in
copy testcases\qsort.data .\in
echo qsort
main.exe

del in
copy testcases\queens.data .\in
echo queens
main.exe

del in
copy testcases\statement_test.data .\in
echo statement_test
main.exe

del in
copy testcases\superloop.data .\in
echo superloop
main.exe

del in
copy testcases\tak.data .\in
echo tak
main.exe

del in
copy testcases\pi.data .\in
echo pi
main.exe

REM echo %time%
set /a endMS=%time:~9,2%
set /a endS=%time:~6,2%
set /a endM=%time:~3,2%
REM echo %endM% %endS% %endMS%

set /a diffS_=%endS%-%startS%
set /a diffMS_=%endMS%-%startMS%
set /a diffM_=%endM%-%startM%
set /a T=diffM_*60000+diffS_*1000+diffMS_*10
echo runtime:%T%ms

pause&&exit