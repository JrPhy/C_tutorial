在大型程式中如果要去 trace code 會非常困難，gdb 是一個很好的工具，可以知道在執行期間，程式碼從 main() 進入後，在往下走到哪個函數。相關的原理可以到其他地方查看，這篇會已設定與使用方式為主。

## 1. 設定
要使用 gdb 時要在編譯時加上 -g 的選項，執行時在最前面加上 gdb 就可以進入 gdb 了。
```
gcc -g -o main main.c 
gdb ./main
```
之後會跳出一堆資訊，主要是告訴你 gdb 版本跟一些 license 資訊。看到有 ```(gdb)``` 就可以開始輸入指令了，首先先輸入 ```start or r``` 讓程式進入 main。
```
Reading symbols from ./run...
(gdb) start
Temporary breakpoint 1 at 0x117f: file main.c, line 6.
Starting program: /home/ubuntu/maketest/run

Temporary breakpoint 1, main () at main.c:6
6       int main() {
```
這時候程式就停在 main 函數了。這時可以使用下列指令
```
(gdb) c/continue        # 到下個中斷點或結束
(gdb) n/next            # 下一行，不進入函數調用
(gdb) s/step            # 下一行，進入函數調用
(gdb) b/break line/func # 在某行或某個函數設中斷點
(gdb) l                 # 顯示當前行前後的程式碼
```
