@echo off
echo Building C++ project...

REM Clean previous build artifacts
echo Cleaning previous build artifacts...
del /Q *.o src\app\Procesess\*.o src\app\fileHanding\*.o src\app\encryptDecrypt\*.o *.exe 2>nul

REM Set compiler flags
set CXXFLAGS=-std=c++17 -g -Wall -I. -Isrc/app/encryptDecrypt -Isrc/app/fileHanding -Isrc/app/Procesess

echo Compiling source files...

REM Compile main.cpp
g++ %CXXFLAGS% -c main.cpp -o main.o
if %errorlevel% neq 0 (
    echo Failed to compile main.cpp
    exit /b 1
)

REM Compile ProcessManagement.cpp
g++ %CXXFLAGS% -c src/app/Procesess/ProcessManagement.cpp -o src/app/Procesess/ProcessManagement.o
if %errorlevel% neq 0 (
    echo Failed to compile ProcessManagement.cpp
    exit /b 1
)

REM Compile IO.cpp
g++ %CXXFLAGS% -c src/app/fileHanding/IO.cpp -o src/app/fileHanding/IO.o
if %errorlevel% neq 0 (
    echo Failed to compile IO.cpp
    exit /b 1
)

REM Compile ReadENV.cpp
g++ %CXXFLAGS% -c src/app/fileHanding/ReadENV.cpp -o src/app/fileHanding/ReadENV.o
if %errorlevel% neq 0 (
    echo Failed to compile ReadENV.cpp
    exit /b 1
)

REM Compile Cryption.cpp
g++ %CXXFLAGS% -c src/app/encryptDecrypt/Cryption.cpp -o src/app/encryptDecrypt/Cryption.o
if %errorlevel% neq 0 (
    echo Failed to compile Cryption.cpp
    exit /b 1
)

echo Linking object files...
g++ %CXXFLAGS% main.o src/app/Procesess/ProcessManagement.o src/app/fileHanding/IO.o src/app/fileHanding/ReadENV.o src/app/encryptDecrypt/Cryption.o -o encrypt_decrypt
if %errorlevel% neq 0 (
    echo Failed to link object files
    exit /b 1
)

echo Build completed successfully! Executable: encrypt_decrypt.exe
pause