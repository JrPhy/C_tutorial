C 語言是一個非常簡單的語言，其關鍵字在 C99 以前僅有 32 個，C99 新增了 5 個來到 37 個，C11 則再新增 7 個關鍵字來到 42 個，且 C 語言的後面版本皆是向下相容的，不會像 python or C++ 一樣。而 C99 的 99 代表這個標準是 1999 年制定或發表，C 的版本迭代並不像 C++ 一樣幾乎三年一版，目前的版本為 C89(C90)、C99、C11、C17(C18)、C23(預計)。以下分別介紹個標準的來源與關鍵字

1. C89/C90 \
這是 C 語言第一次給出一個統一個規格，由美國國家標準委員會訂定，又稱為 ANSI C。在此版本中共有 32 個關鍵字如下

| 變數型別 | 流程控制 | 生命週期 | 其他 |
| --- | --- | --- | --- |
| char | if | auto | const |
| short | else | extern | register |
| int | continue | static | typedef |
| unsigned | break | volatile | sizeof |
| long | for | | |
| float | switch | | |
| double | default | |
| signed | case | | |
| void | do | | |
| union | while | | |
| enum | goto | | |
| struct | return | |  |

2. C99 \
比 C89 多了以下關鍵字：restrict, inline, _ Complex, _ Imaginary, _ Bool。在這版最重要的是增加了可變長度的陣列，例如 int a[m]，在使用上方便許多，但是速度也比固定長度陣列慢許多。而在這版也新增了 restrict 關鍵字，讓指標傳遞能夠更加快速

3. C11 \
比 C99 多了以下關鍵字：_ Alignas, _ Alignof, _ Atomic, _ Generic, _ Noreturn, _ Static_assert, _ Thread_local。在這版最重要的是增加了對於多執行緒與泛型。不過對於多執行緒的開發，多數人仍喜歡使用第三方套件 pthread。

4. C18 \
此版本未新增關鍵字
