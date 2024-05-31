MAKEFILE 拿來編譯專案很方便，但是 MAKEFILE 卻不太好撰寫，而且專案一大要改也不容易，所以就有 CMake 誕生。CMake 的語法較 MAKEFILE，且跨平便建置很方便，現今大型的專案也都支援用 CMake 建置。

## 1. 使用
CMake 會去讀取 CMakeList.txt，此檔案通常與源碼在同個目錄下，並會另外建立 build，這樣可以把原始碼與執行檔分開，會讓專案更乾淨。進入 build 後執行 cmake .. 就會生成建置檔案，最後在 make 即可開始編譯。所以在安裝時通常會有以下三行命令
```
mkdir build && cd build
cmake ../
make
```
如果源碼有許多部分需要獨立編譯，並放在其他資料夾中，則每個獨立的部分都需要寫一個 CMakeList.txt。
cmake ../
```
-- The C compiler identification is GNU 9.4.0
-- The CXX compiler identification is GNU 9.4.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Failed
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - found
-- Found Threads: TRUE
-- Configuring done
-- Generating done
-- Build files have been written to: tcpip/build
```
make
```
Scanning dependencies of target client_thread
[ 25%] Building C object CMakeFiles/client_thread.dir/client_thread.c.o
[ 50%] Linking C executable client_thread
[ 50%] Built target client_thread
Scanning dependencies of target server_thread
[ 75%] Building C object CMakeFiles/server_thread.dir/server_thread.c.o
[100%] Linking C executable server_thread
[100%] Built target server_thread
```
這樣就會在 build 目錄下生成 server_thread 與 client_thread 兩個執行檔。

## 2. CMakeList.txt
主要由以下幾個部分組成，真正有作用的只有 ```add_executable(<name> <options>... <sources>...)```，會把原始碼編成執行檔，前面兩行皆可忽略。
```
cmake_minimum_required(VERSION 3.16) # 決定 cmake 版本
project(ex2) # 專案名稱
add_executable(ex2 main.c calc.c) # 把那些檔案編成執行檔
```
當然也有可能有好幾個執行檔，可以參考我的 [tcpip repo](https://github.com/JrPhy/tcpip/blob/main/CMakeLists.txt)

## 3. 編譯與連結函式庫
在大型專案中有可能會將某個檔案編成函式庫，cmake 中使用 ```add_library(<name> <SHARED|STATIC|MODULE|INTERFACE> <sources>...)``` 來將原始檔編成函式庫， STATIC 就是編成 .a， SHARED 編成 .so，其餘兩個不常用到。在編譯成函式庫 cmake 會在名稱前綴加上 lib，所以 name 就不用再另外加 lib
```
add_library(add STATIC add.c) # 產生 libadd.a
add_library(divide SHARED divide.c) # 產生 libdivide.so
```
再來把 library 跟 header 檔都加入進執行檔內
```
TARGET_LINK_LIBRARIES(server_thread add)
target_include_directories(add PUBLIC tcpip)
```
這樣在 make 的時候就會 link 進去了
```
Scanning dependencies of target add
[ 12%] Building C object CMakeFiles/add.dir/add.c.o
[ 25%] Linking C static library libadd.a
[ 25%] Built target add
Scanning dependencies of target client_thread
[ 37%] Building C object CMakeFiles/client_thread.dir/client_thread.c.o
[ 50%] Linking C executable client_thread
[ 50%] Built target client_thread
Scanning dependencies of target divide
[ 62%] Building C object CMakeFiles/divide.dir/divide.c.o
[ 75%] Linking C shared library libdivide.so
[ 75%] Built target divide
Scanning dependencies of target server_thread
[ 87%] Building C object CMakeFiles/server_thread.dir/server_thread.c.o
[100%] Linking C executable server_thread
[100%] Built target server_thread
```
