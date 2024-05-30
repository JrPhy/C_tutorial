直接使用 gcc 做編譯在檔案一多時會變得很麻煩，所以在一些大型專案中就會看到 MAKEFILE，下載原始碼後只要下 ```make``` 或是 ```make all```就自動把整個專案編成執行檔了。當然內部還是依靠指令，只不過可以寫成類似函數的方式來讓使用者更方便。

## 1. 語法
主要由 target 和 command 組成，target 有點類似函數，當我們下 make ${target} 時就會去執行這個 target 下的 command。
```
target:
    command
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
```

