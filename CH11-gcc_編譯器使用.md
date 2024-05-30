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
4. gcc -o main main.o --> 將編譯單元編成可執行檔
也可直接 gcc -o main main.c，最後只會保留執行檔，上面的中間產物都不會保留。
```
gcc -o add.c main.c -o add
```
上方指令會將 add.c, main.c 兩個檔案編成一個可執行文件 add。若有很多個 .cpp 也可用 *.cpp 來取代。當然也可以直接 ```gcc main.c```，此時會產生一個 a.out 可執行文件。

## 2. 連結靜態庫與自訂 header
除了標準函式庫以外，也常常用到別人的函式庫，就會有許多 .h, .o, .a(靜態函式庫), .so(動態函式庫)，這時就需要使用其他選項將他們編進執行檔中。
```
gcc -o add.c main.c -I./divide.h -L./divide.a -o calculate
```
除了自己寫的 main 與 add 外，還用到別人寫的 divide。gcc/g++ 中 -I 來指定要引入的 header，-L 來指定要引入的 .a (-I/-L 與檔案間沒有空格)，
