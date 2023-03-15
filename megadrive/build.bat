cd ..\megadrive
set SGDK_PATH=.\sgdk
rem %SGDK_PATH%\bin\make -f %SGDK_PATH%\makelib.gen
%SGDK_PATH%\bin\make -f %SGDK_PATH%\makefile.gen clean
%SGDK_PATH%\bin\make -f %SGDK_PATH%\makefile.gen
