在大型程式中如果要去 trace code 會非常困難，gdb 是一個很好的工具，可以知道在執行期間，程式碼從 main() 進入後，在往下走到哪個函數。相關的原理可以到其他地方查看，這篇會已設定與使用方式為主。

## 1. 設定
要使用 gdb 時要在編譯時加上 -g 的選項，執行時在最前面加上 gdb 就可以進入 gdb 了。
```
gcc -g -o main main.c 
gdb ./main
```
如果有其他 .o 檔要 link，那每個 .c 檔在編譯時都需要加上 .o
```
#SHELL=/bin/bash
objects := $(patsubst %.c,%.o,$(wildcard *.c))

run: $(objects)
	gcc -g -o $@ $^ -I./

$(objects): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@ -g
```
之後會跳出一堆資訊，主要是告訴你 gdb 版本跟一些 license 資訊。看到有 ```(gdb)``` 就可以開始輸入指令了，首先先輸入 ```start or r``` 讓程式進入 main。
```
Reading symbols from ./run...
(gdb) start
```
## 2. 中斷點
用 break 或 b 來設定中斷點，後面加上檔名、行號，或是檔名+行號。可以再開始前後設定。用 clear 加上檔名、行號，或是檔名+行號來移除中斷點。
```
b main.c
b 15
b main.c 15
```
設完之後可以用 ```i b``` 來看中斷點的資訊
```
(gdb) i b
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x0000555555555220 in printList at main.c:20
        breakpoint already hit 1 time
2       breakpoint     keep y   0x000055555555544d in deleteNode at main.c:88
(gdb)
```
## 3. 進入 DEBUG
```
Temporary breakpoint 1 at 0x117f: file main.c, line 6.
Starting program: /home/ubuntu/maketest/run

Temporary breakpoint 1, main () at main.c:6
6       int main() {
```
這時候程式就停在 main 函數了。這時可以使用下列指令繼續往下或看程式碼
| c/continue | n/next | s/step | l/list (m, n) |
| --- | --- | --- | --- |
| 到下個中斷點或結束 | 下一行，不進入函數調用 | 下一行，進入函數調用 | 顯示當前行前後的程式碼 |
```
(gdb) n
24              list = list->next;
(gdb) n
21          while(list != NULL)
(gdb) l
16          return tmpNode;
17      }
18
19      void printList(node *list)
20      {
21          while(list != NULL)
22          {
23              printf("%d->", list->data);
24              list = list->next;
25          }
(gdb)
```
可以利用下列指令來查看變數
| print/display {var} | ptype | explore |
| --- | --- | --- |
| 值 | 變數型別 | 詳細變數內容 |
```
(gdb) print list->data
$1 = 4
(gdb) ptype list
type = struct _node {
    int data;
    struct _node *next;
} *
(gdb) explore list
'list' is a pointer to a value of type 'node'
Continue exploring it as a pointer to a single value [y/n]: y
The value of '*list' is of type 'node' which is a typedef of type 'struct _node'
The value of '*list' is a struct/class of type 'struct _node' with the following fields:

  data = 4 .. (Value of type 'int')
  next = <Enter 1 to explore this field of type 'struct _node *'>
```
如果說專案很大的話，可以用 bt/backtrace 來查看 callstack
```
(gdb) bt
#0  printList (list=0x555555559710) at main.c:23
#1  0x000055555555556b in main () at main.c:119
```
## 4. tui
gdb 也有內建 ui 來讓使用者有比較好的介面可看，執行時加上 --tui 即可開啟，剩下的操作都與前述一樣。
```
┌──main.c──────────────────────────────────────────────────────────────────────┐
│   107         {                                                              │
│   108             /* Initialize nodes */                                     │
│B+>109             node *one = newNode(1);                                    │
│   110             node *two = newNode(2);                                    │
│   111             node *three = newNode(3);                                  │
│   112                                                                        │
│   113             one->next = two;                                           │
│   114             two->next = three;                                         │
│   115             three->next = NULL;                                        │
│   116             // insert new node at the beginning                        │
│   117             printList(one);                                            │
│b+ 118             insertNode(&one, 4, 0);                                    │
│   119             printList(one);                                            │
│   120             // insert new node at the ending                           │
│   121             insertNode(&one, 5, 10);                                   │
│   122             printList(one);                                            │
│   123             // insert new node in the middle                           │
│   124             insertNode(&one, 7, 2);                                    │
│   125             printList(one);                                            │
│   126                                                                        │
│b+ 127             deleteNode(&one, 10);                                      │
│   128             printList(one);                                            │
│   129                                                                        │
│   130             deleteNode(&one, 3);                                       │
│   131             printList(one);                                            │
│   132                                                                        │
│   133             reverse(&one);                                             │
│   134             printList(one);                                            │
│   135                                                                        │
│   136             return 0;                                                  │
│   137         }                                                              │
└──────────────────────────────────────────────────────────────────────────────┘
native process 103 In: main                                                                                                                                                             L109  PC: 0x5555555554ef
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from ./run...
(gdb) b 109
Breakpoint 1 at 0x14ef: file main.c, line 109.
(gdb) b 118
Breakpoint 2 at 0x1549: file main.c, line 118.
(gdb) b 127
Breakpoint 3 at 0x15af: file main.c, line 127.
(gdb) r
Starting program: /home/ubunru/maketest/run

Breakpoint 1, main () at main.c:109
(gdb)
```

## 5. 與 VSCODE 使用
VSCODE 是一個微軟開發的強大 IDE，本身並不包含編譯器跟 GDB，主要是通過圖形化介面，並且呼叫 gcc 與 gdb 來達成視覺化的效果，可以直接設中斷點，看到物件的值與資料結構。編譯時需要自己加上 -g 與需要的選項並編出執行檔，debugger 主要是做圖形化與叫戶介面使用。可以直接按 ```Ctrl+F5``` 來配置 DEBUG，在 ```~/.vscode``` 下會有兩個檔案，
task.json
```
{
    "tasks": [
        {
            "type": "cppbuild",
            "label": "C/C++: g++.exe build active file",
            "command": "/usr/bin/gcc",
            "args": [],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "detail": "Task generated by Debugger."
        }
    ],
    "version": "2.0.0"
}
```
command 要設定編譯器位置，如果是 c++ 則改成 g++。args 則是看有什麼選項要開。
launch.json
```
{
    // Use IntelliSense to learn about possible attributes.
    // Hover to view descriptions of existing attributes.
    // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(gdb) Launch",
            "type": "cppdbg",
            "request": "launch",
            "program": "/home/ubuntu/maketest/run",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${fileDirname}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "miDebuggerPath": "/usr/bin/gdb"
        }

    ]
}
```
這邊主要是 program 和 cwd 要設對，cwd 可以保留變量，program 如果找不到的話可以給絕對路徑，執行檔名稱與 main 函數所在檔案的名稱要相同，不同的話就自行設定。如果是用 wsl 的話確認一下目前 VSCODE 是否有 Remote Wsl，或是直接從 Windows 進入，兩者路徑不同，可由右下角檢查。
