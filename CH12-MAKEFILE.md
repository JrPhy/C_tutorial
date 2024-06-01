直接使用 gcc 做編譯在檔案一多時會變得很麻煩，所以在一些大型專案中就會看到 MAKEFILE，下載原始碼後只要下 ```make``` 或是 ```make all```就自動把整個專案編成執行檔了。當然內部還是依靠指令，只不過可以寫成類似函數的方式來讓使用者更方便。只有一個 makefile 的話會直接命名為 Makefile，有多個的話則會在副檔名加上 .mk

## 1. 語法
主要由 target 和 command 組成，target 有點類似函數，當我們下 make ${target} 時就會去執行這個 target 下的 command，註解為 **#**。
```
target:
    command 
# 不執行
```
command 前面為 **tab**。target 通常會以要編成的檔案命名，例如有個 main.c，想要先編成 main.o 再編成 main，則寫成
```
main: main.o
    gcc -o main main.o

main.o:
    gcc -c main.c -o main.o
```
而 target 後面也可以加上 dependency，有點類似函數中去呼叫另一個函數。在上述中 main 會先去呼叫 main.o 這個 target，執行完其 command 後再執行原本的 command，所以下了 ```make main``` 實際上的執行順序為
```
gcc -c main.c -o main.o
gcc -o main main.o
```
所以在寫 dependencie 時要**注意順序**，否則就會報錯。當我們只有下 ```make``` 時會去執行最上面的 target，所以一般會在最上面寫最後一步，或是用 default 來指定 target
```
default: main

main: main.i main.s main.o
	gcc -o main main.o

main.o:
	gcc -c main.s -o main.o

main.i:
	gcc -E main.c -o main.i

main.s:
	gcc -S main.i -o main.s

clean:
	rm *.o .a
```
除了一般的 target 外，makefile 中還常見 .PHONY 這個關鍵字，告訴 make 後面的 dependency 並非要產生檔案。所以當 makefile 中有 clean 這個 target，剛好同個資料夾內也有名為 clean 的檔案，此時就不會執行 clean。改為下面的寫法，就不會去管是否有 clean 這個檔案存在
```
...
.PHONY:clean
clean:
	rm *.i *.s *.o
```
## 2. 變數
在 makefile 中也可以自訂變數與呼叫變數，例如可以先把專案的絕對路徑設為一個變數，並用 $() 取得變數，之後再編譯的時候就不用寫的那麼長，或是也可以指定編譯器
```
CC = gcc
path=/home/user/project
FLAG+= O3
FLAG+= DEBUG
default: main
main: main.i main.s main.o
	$(CC) -o main main.o $(FLAG) -I$(path)/include -L$(path)/lib
```
這樣在執行 ```make``` 時就會使用 gcc 編譯器，以及引入 project 下的 header 與 lib。

#### 1. $(MAKE)
在變數中有個特別的變數 $(MAKE)，可以算是 makefile 的保留字，常搭配 -C 這個選項，會進去子目錄中執行裡面的 makefile。在[文件](https://github.com/yyluoyong/Make-3.8-Chinese-Manuals/blob/master/main.pdf)中有個例子
```
#maindir Makefile
………
………
.PHONY :test
test:
@echo “main makelevel = $(MAKELEVEL)”
@$(MAKE) –C subdir dislevel
#subdir Makefile
………..
………..
.PHONY : test
test :
@echo “subdir makelevel = $(MAKELEVEL)”
```
執行的結果為
```
main makelevel = 0
make[1]: Entering directory `/…../ subdir '
subdir makelevel = 1
make[1]: Leaving directory `/…../ subdir '
```
當專案夠大時就可以使用 $(MAKE) 變數來減少 makefile 寫的內容。

#### 2. 其他保留變數
在 makefile 中把 ```$(CC)``` 保留給 gcc，```$(CXX)``` 保留給 g++，也有對應的 ```$(CFLAGS), $(CXXFLAGS)```，分別對應 gcc 與 g++ 的 flags，也就是 -o, -c -DDEBUG 這些選項。另外還有 ```$(OS)```，可以搭配 ifeq 拿來寫跨平台的 MAKEFILE。

#### 3. 賦值
| = | := | ?= | += |
| --- | --- | --- | --- |
| 基本的賦值 | 覆蓋之前的值 | 如果未定義就給等號後面的值 | 加等號後面的值 |
| x = foo | x := foo |  |  |
| y = $(x) bar | y := $(x) bar |  |  |
| x = abc | x := abc |  |  |

上述中如果單純用 = 賦值，那麼 y 為 abc bar。若用 := 賦值，那麼 y 為 foo bar。

#### 4. 外部傳入
makefile 也支援從命令行傳參數進去，例如我們想要從外面決定是否要開啟 DEBUG
```
CC = gcc
path=/home/user/project
FLAG += O3
DEBUG =
default: main
main: main.i main.s main.o
	$(CC) $(DEBUG) -o main main.o $(FLAG) -I$(path)/include -L$(path)/lib
```
執行 make DEBUG = -DDEBUG 就可以把 -DDEBUG 放入 DEBUG 中。

## 3. ```$@, $^, $<, $?```
第一次看到這東西應該滿多人以為是亂碼的，但這個在 makefile 中分別有以下意思
1. $@  表示目標文件
2. $^  表示所有的依賴文件
3. $<  表示第一個依賴文件
4. $?  表示比目標還要新的依賴文件列表\
例如有個 makefile 內容為
```
all: main.o add.o divide.o
	gcc -o calculate main.o hello.o hi.o

main.o: main.c
	gcc -c main.c

add.o: add.c
	gcc -c add.c

divide.o: divide.c
	gcc -c divide.c

.PHONY: clean
clean:
	rm *.o main
```
用那些符號就可以改寫成
```
all: main.o add.o divide.o
	gcc -o calculate $@ $^

main.o: main.c
	gcc -c $<

add.o: add.c
	gcc -c $<

divide.o: divide.c
	gcc -c $<

.PHONY: clean
clean:
	rm *.o main
```

## 4. 流程
在 makefile 中有四種 if，結尾搭配 endif 來用
| 關鍵字 | 用法 | 意思 |
| --- | --- | --- |
| ifeq | ifeq(A, B) | 兩者**相同**為 true |
| ifneq | ifneq(A, B)  | 兩者**不同**為 true |
| ifdef | ifdef(A, B)  | 有值為 true |
| ifndef | ifndef(A, B)  | 沒值為 true |

```
# Detect underlying system.
ifeq ($(OS),Windows_NT)
	detected_OS := Windows
else
	detected_OS := $(shell sh -c 'uname -s 2>/dev/null || echo not')
endif

export detected_OS

# Set default C compiler.
# Clean implict CC variable.
CC=

ifndef CC
	ifeq ($(detected_OS),Windows)
		CC=cl
	else ifeq ($(detected_OS),Darwin)
		CC=clang
	else
		CC=gcc
	endif
endif  # CC
```
上面的寫法會先根據 $(OS) 來取得是在哪個平台，然後定義平台的名稱。因為不同平台常用的 C 編譯器也不一定相同，所以再根據平台去設定使用的 C 編譯器。
## 5. 一些 flags
| 多核編譯 -j | 指定跑哪個 makefile -f | 進入 sub_dir 去跑 makefile -C |
| --- | --- | --- |
|     |     |     |

make -j4 -C sub_dir -f makefile 表示使用 4 個 core 去跑 ./sub_dir/makefile
