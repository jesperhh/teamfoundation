REM Copyright (C) 2014 Maksim Klimov
REM 
REM Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
REM 
REM The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
REM 
REM THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

REM Generate .lib files using existing .dll files
REM Helpful to skip QtC building and use installed QtC binaries
REM First argument is a directory with dlls
REM Second argument is an output directory

setlocal ENABLEDELAYEDEXPANSION
@ECHO OFF
SET "INPUT_DIR=%1"
SET "OUTPUT_DIR=%2"
for /F %%x in ('dir /B/D %INPUT_DIR%\*.dll') do (
 call :makelib %%x %%~nx %INPUT_DIR%)

goto:End

:makelib
  set x=%1
  set NAME=%2
  set FP=%3
  echo !NAME!
  dumpbin /exports %FP%\%x% > !OUTPUT_DIR!\!NAME!_export.txt
  echo LIBRARY !NAME! > !OUTPUT_DIR!\!NAME!.def 
  echo EXPORTS >> !OUTPUT_DIR!\!NAME!.def 
  call :prepend !OUTPUT_DIR!\!NAME!
  lib /def:!OUTPUT_DIR!\!NAME!.def /out:!OUTPUT_DIR!\!NAME!.lib /machine:x86
goto:eof

:prepend
  for /f "skip=19 tokens=4" %%a in (%1_export.txt) do echo %%a >> %1.def
goto:eof
:End