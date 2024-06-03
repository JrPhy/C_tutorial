MAKEFILE 拿來編譯專案很方便，但是 MAKEFILE 卻不太好撰寫，而且專案一大要改也不容易，更不用說一個專案要可以跨平台使用，如果要在一個專案中加入某個檔案，那就需要去修改其編譯文件。CMake 的誕生解決了這問題，我們只需要撰寫一個 CMakeLists.txt，就會根據平台生成不同的編譯文件，現今大型的專案也都支援用 CMake 建置，如 OpenCV，不論是在 WIN32, LINUX, ANDROID 或是 ARM，都可以輕鬆的建置跟編譯，可以參考[這篇文章](https://www.jb51.net/article/274466.htm)的圖。

## 1. 使用
CMake 會去讀取 CMakeLists.txt，此檔案通常與源碼在同個目錄下，並會另外建立 build，這樣可以把原始碼與執行檔分開，會讓專案更乾淨。進入 build 後執行 cmake .. 就會生成建置檔案，最後在 make 即可開始編譯。所以在安裝時通常會有以下三行命令
```
mkdir build && cd build
cmake ../
make
```
如果源碼有許多部分需要獨立編譯，並放在其他資料夾中，則每個獨立的部分都需要寫一個 CMakeList.txt。\
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
這樣就會在 build 目錄下生成 server_thread 與 client_thread 兩個執行檔。可以用 **message() **函數來除錯。

## 2. CMakeList.txt
主要由以下幾個部分組成，真正有作用的只有 ```add_executable(<name> <options>... <sources>...)```，會把原始碼編成執行檔，前面兩行皆可忽略，# 為註解。
```
cmake_minimum_required(VERSION 3.16) # 決定 cmake 版本
project(math) # 專案名稱
add_executable(ex2 main.c add.c) # 把那些檔案編成執行檔
```
當然也有可能有好幾個執行檔，可以參考 [tcpip repo](https://github.com/JrPhy/tcpip/blob/main/CMakeLists.txt)。如果檔案很多，可以把原始碼放進 src 資料夾，並在該資料夾內另外寫一個 CMakeList.txt。或是使用 ```aux_source_directory(${PATH} SRC_FILES)```、```file(GLOB SRC_FILES "${PROJECT_SOURCE_DIR}/*.c")``` 來找出特定路徑下的原始檔放入 SRC_FILES 變數中，再利用 ```add_executable(ex2 ${SRC_FILES})``` 編成執行檔。

## 3. 編譯與連結函式庫
在大型專案中有可能會將某個檔案編成函式庫或是使用第三方函式庫，cmake 中使用 ```add_library(<name> <SHARED|STATIC|MODULE|INTERFACE> <sources>...)``` 來將原始檔編成函式庫， STATIC 就是編成 .a， SHARED 編成 .so，其餘兩個不常用到。並利用 ```find_package``` 來連結第三方函式庫。

#### 1. 自己的函式庫
在編譯成函式庫 cmake 會在名稱前綴加上 lib，所以 name 就不用再另外加 lib
```
add_library(add STATIC add.c) # 產生 libadd.a
add_library(divide SHARED divide.c) # 產生 libdivide.so
```
再來把 library 跟 header 檔都加入進執行檔內，分別使用 link_directories 與 include_directories 來將需要的函式庫與 header 位置加入，header 通常是放在 include，是用 ```link_directories(directory1 directory2 ...)``` 來選資料夾，也可以用 target_link_libraries 來指定函式庫
```
include_directories(${PROJECT_NAME})
# 相當於 gcc -I$(PROJECT_NAME)
link_directories(${PROJECT_NAME})
# 相當於 gcc -L$(PROJECT_NAME)
target_link_libraries(server_thread add)
# 相當於 gcc -o server_thread server_thread.c -ladd.a
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
#### 2. add_subdirectory
通常專案會將大部分的 .c, .cpp 文件放入 src 資料夾中，.h 放入 include 中(也有少數的 .cpp)，當然也可以指定 src 下的原始碼，不過我們也可以在 src 中另外寫個 CMakeLists.txt，然後將最上層的 CMakeLists.txt 加上
```
add_subdirectory(src)
```
剩下的放進 src/CMakeLists.txt 中。這樣就會自動去跑 src 中的 CMakeLists.txt 了。\
```
tcpip/
├── CMakeLists.txt
├── Makefile
├── README.md
├── build
├── include
│   └── add.h
└── src
    ├── CMakeLists.txt
    ├── add.c
    ├── client_thread.c
    ├── divide.c
    └── server_thread.c
CMakeLists.txt
```
```
cmake_minimum_required(VERSION 3.16) # 決定 cmake 版本
project(tcpip) # 專案名稱
add_subdirectory(src)
```
src/CMakeLists.txt
```
add_executable(server_thread server_thread.c) # 把 server_thread.c 編成執行檔
add_executable(client_thread client_thread.c)
add_library(add STATIC add.c) # 把 add.c 編成 libadd.a
add_library(divide SHARED divide.c) # 把 divide.c 編成 libdivide.so
set_target_properties(server_thread client_thread PROPERTIES
    COMPILE_FLAGS "-pthread"
    LINK_FLAGS "-pthread"
)
include_directories(${PROJECT_NAME})
link_directories(${PROJECT_NAME})
```
#### 3. find_package
如果是要用第三方的函式庫，可以利用 ```find_package(<name> ...)```，後面有許多選項可以加，我比較常用到是 [REQUIRED] 或 [version]。例如要使用多執行緒，那麼可以先看看是否有內建 Thread
```
find_package(Threads REQUIRED)
```
```
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Failed
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - found
-- Found Threads: TRUE
```
執行時就會去找相關的第三方函式庫，若沒找到就會出錯。如果是另外下載第三方函式庫如 OpenCV，那就會另外開一個 cmake 資料夾，裡面建立一個 FindOpenCV.cmake，使得整個專案較有秩序。

## 4. 變數
CMake 的變數常用在設定路徑與 FLAG，利用 set(name, value) 來宣告，${name} 來取值。如果有給 project({name}) 的話，那就可以用 ${PROJECT_NAME} 來取得專案名稱
```
set(SRC_DIR ${PROJECT_NAME}/src)
set(LIB_DIR ${PROJECT_NAME}/libs)
set(INC_DIR ${PROJECT_NAME}/include)
set(SOURCE 
    A.cpp
    B.cpp)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -ggdb -D DEBUG")
```
最後一行的寫法是追加參數或是傳入參數，{} 後面就是預設的，這樣就可以一直把選項加進去，不用再去找到那段。當然 CMake 中也有一些保留用的變數名稱，如 WIN32, APPLE, ANDROID 等常見的平台名稱，當然還有 CXX_FLAG 等。下方列出一些常用的變數
| 變數 | 用途 |
| :--- | :--- |
| PROJECT_NAME | 專案名 |
| CMAKE_BINARY_DIR | 構建目錄 |
| CMAKE_SOURCE_DIR | 專案根目錄 |
| CMAKE_CURRRENT_BINARY_DIR | 所在的目錄 |
| CMAKE_CURRENT_LIST_FILE | 當前 CMakeLists.txt 完整路徑 |

也可以用 ${ENV_var} 來取得當前的環境變數。
#### 1. 外部傳入
cmake 也可以像 makefile 一樣傳入變數，來決定是要建置 debug 或 release，像上方的 ```set(CMAKE_BUILD_TYPE Debug)``` 就是要做這件事，再命令行執行 ```cmake -DCMAKE_BUILD_TYPE=Release ..``` 就可以了。還有像指定安裝位置也常用 ```CMAKE_INSTALL_PREFIX ```，當然更常是搭配 if 做使用。

## 5. 安裝目錄
在建置好之後就開始編譯，編譯後的檔案通常會希望放在另外的地方，至少要跟原始碼分開放，所以一個大型專案會在分出以下資料夾
| src | bin | lib | include |
| --- | --- | --- | --- |
| 原始碼 | 執行檔 | .a, .so | header |\
在最後使用 install，然後執行 make install 就會把那些檔案搬移到指定位置
```
install(TARGETS server_thread client_thread add divide
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include
    )
```
## 5. 流程控制
如 makefile 一樣使用 if 與上述的平台變數來建立一個跨平台的 cmake。結構為
```
if(...)
    ...
elseif(...)
    ...
else()
    ...
endif(...)
```
在寫 cmake 用於大型專案時，會根據以下順序來寫
1. 決定安裝目錄
2. 建置選項 (Debug, Release)
3. 專案名稱
4. 平台
5. 編譯器
在此用 [openCV](https://github.com/opencv/opencv/tree/4.x) 的例子來做說明，內部有個 CMakeLists.txt，內容有許多的 if 與變數。建置選項如果沒有特別指明，那就會以 **Release** 來做
```
if(NOT DEFINED CMAKE_BUILD_TYPE
    AND NOT OPENCV_SKIP_DEFAULT_BUILD_TYPE
)
  message(STATUS "'Release' build type is used by default. Use CMAKE_BUILD_TYPE to specify build type (Release or Debug)")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build")
endif()
if(DEFINED CMAKE_BUILD_TYPE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "${CMAKE_CONFIGURATION_TYPES}")
endif()
```
根據不同的平台選擇安裝目錄，安裝目錄就是讓編譯出來的函式庫與要使用的 header 檔放在另外一個資料夾，這樣在做修改時就是改該資料夾的內容。
```
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)  # https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT.html
  if(NOT CMAKE_TOOLCHAIN_FILE)
    if(WIN32)
      set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation Directory" FORCE)
    else()
      set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Installation Directory" FORCE)
    endif()
  else()
    # any cross-compiling
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation Directory" FORCE)
  endif()
endif()
```
接著就去看是用哪個編譯器，根據不同的編譯器來加入編譯選項
