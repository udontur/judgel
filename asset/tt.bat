@echo off

if "%1"=="comp" cmd /k "g++ C:\Users\hadri\code\github\judgel\judgel\judgel.cpp -o C:\Users\hadri\code\github\judgel\judgel\judgel.exe"
if "%1"=="ac" cmd /k "judgel 2 C:\Users\hadri\code\github\judgel\asset\tests\ac\testcase C:\Users\hadri\code\github\judgel\asset\tests\ac\sol.cpp"
if "%1"=="wa" cmd /k "judgel 1 C:\Users\hadri\code\github\judgel\asset\tests\wa\testcase C:\Users\hadri\code\github\judgel\asset\tests\wa\sol.cpp"
if "%1"=="rte" cmd /k "judgel 9 C:\Users\hadri\code\github\judgel\asset\tests\rte\testcase C:\Users\hadri\code\github\judgel\asset\tests\rte\sol.cpp"
if "%1"=="tle" cmd /k "judgel 2 C:\Users\hadri\code\github\judgel\asset\tests\tle\testcase C:\Users\hadri\code\github\judgel\asset\tests\tle\sol.cpp"
if "%1"=="ca" cmd /k "judgel 99 C:\Users\hadri\code\github\judgel\asset\tests\ca\testcase C:\Users\hadri\code\github\judgel\asset\tests\ca\sol.cpp"
