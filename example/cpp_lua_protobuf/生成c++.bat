 @echo off
 
 set SOURCE_FOLDER=.
 set CPP_COMPILER_PATH=protoc.exe
 set CPP_TARGET_PATH=./client/
 
 ::���������ļ�
 for /f "delims=" %%i in ('dir /b "%SOURCE_FOLDER%\*.proto"') do (

     echo %CPP_COMPILER_PATH% --cpp_out=%CPP_TARGET_PATH% %%i
     %CPP_COMPILER_PATH% --cpp_out=%CPP_TARGET_PATH% %%i
 
 )

pause
 
