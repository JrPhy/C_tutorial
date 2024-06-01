在寫 C 語言程式後會需要做編譯，將程式碼編譯成可執行檔，在 windows 就是 .exe，linux 則是一個文件。編譯後的內容通常是看不懂的，所以隨意執行一個未知的執行檔是很危險的一件事，因為不知道裡面到底寫了什麼。\
如果是使用 IDE 開發，那通常可以有一些圖形化介面或快捷鍵可以幫你編譯跟執行，也有使用 linux 搭配 IDE 做開發，這時就需要用到編譯器的編譯指令。在此使用 gcc 為例，C++ 只要將 gcc 改為 g++ 即可，也有其他編譯器如 clang, llvm 等。

## 1. .c 到執行檔
如果只有一個文件 main.c，那麼可以用命令行直接操作。
| 使用 gcc 編譯器 | 將後方文件編為執行檔 | 執行檔名稱 | 要編譯的文件 |
| --- | --- | --- | --- |
| gcc | -o | main | main.c |

雖然只有下 -o，但實際上[編譯流程](https://github.com/JrPhy/C_tutorial/blob/main/CH6-%E7%B7%A8%E8%AD%AF%E6%B5%81%E7%A8%8B%E8%88%87%E9%A0%90%E8%99%95%E7%90%86.md)會有四步。先將 .c 文件，包含 C 標準的 #include 編成一個 .o 檔案，再編成執行檔。因為只有一個 .cpp 而已，如果有多個 .cpp，就會有多個 .o，最後會把所有的 .o 連結再一起，再編成一個執行檔。也可以分開來做
1. gcc -E main.c -o main.i --> 展開 include
2. gcc -S main.i -o main.s --> 將上一步的文件編成組合語言
3. gcc -c main.s -o main.o --> 將上一步的文件彙編成一個編譯單元
4. gcc -o main main.o --> 將編譯單元編成可執行檔\
最後只會保留執行檔，上面的中間產物都不會保留。當然也可以直接 ```gcc main.c```，此時會產生一個 a.out 可執行文件。
```
gcc -o main main.c add.c
```
上方指令會將 add.c, main.c 兩個檔案編成一個可執行文件 add。若有很多個 .cpp 也可用 *.cpp 來取代。

## 2. 連結靜態庫與自訂 header
除了標準函式庫以外，也常常用到別人的函式庫，就會有許多 .h, .o, .a(靜態函式庫), .so(動態函式庫)，這時就需要使用其他選項將他們編進執行檔中。除了自己寫的 main 與 add 外，還用到別人寫的 divide。gcc/g++ 中 -I 來指定要引入的 header，-l 來指定要引入的 .a，或是 -L(path) 來引入該 path 中的 lib 檔 (-I/-L 與檔案間沒有空格)。通常編成 .a 或 .so 前面會多個 lib，例如 libdivide.a 或是 libdivide.so，在編譯時寫其原本的名稱或是全名皆可。
```
gcc -o calculate add.c main.c -I./divide.h -L -ldivide
gcc -o calculate add.c main.c -I./divide.h libdivide.so
```

#### 1. 靜態函式庫 .a
靜態函式庫由一個或多個 .o 後再製作而成，在 linux 中利用 ```ar rcs```製作。 ```ar rcs libxxx.a xx1.o xx2.o``` 將兩個 .o 製成一個 .a。編成執行檔後檔案會較大，因為實際上也包進執行檔中了，所以執行時不需再把 .a 也跟執行檔放一起。

#### 2. 動態函式庫 .so
利用 gcc 將檔案編成 .so，可直接從一個或多個 .cpp 或 .o 編成。因為是執行時才會連結，所以執行檔較小，但 .so 當要與執行檔放在同一個目錄。
```
gcc -shared -fPIC -o libtest.so test.o
gcc -shared -o libtest.so -fPIC test.c
```

## 3. 自訂 flag
在 C 語言中常用預定義來去決定是否要將某個函數也編入，例如在程式中加入
```
#ifdef DEBUG
    ....
#else
    ....
#endif
```
那麼在編譯時加上 -DDEBUG，就會把後面的程式碼也編進去，這在控制執行檔式開發用還是客戶用很方便。

當然 gcc 的功能很多，還有 -O 是優化，但上面算是比較常用的幾個指令了。
