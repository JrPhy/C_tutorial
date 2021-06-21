我們從前面幾章已知變數有一般變數 ```(int a;)``` 與指標變數 ```(int *a;)```，而函數本身若有回傳值，因其佔有記憶體位置與值，故也可當成變數 ```(int add(int a, int b))```在此我們稱其為一般函數，而函數也可回傳指標，原形為```int *add(int a, int b)```。另一個指向函數的指標則稱為**函數指標**(function pointer or pointer to function) ```int (*compare)(const void*, const void*)```，兩者在宣告上非常相似。

## 1. 函數回傳指標
如上所述，其原型為 ```int *add(int a, int b)``` ```int* add(int a, int b)```，宣告方式與一般的指標宣告方式幾乎一樣。然而此種使用方式一般會以傳入指標或陣列取代，在此不多給範例說明。

## 2. 指標函數
如之前所提到，函數本身也佔有一記憶體位置與空間，故可以宣告一指標指向該函數的位置。
#### 1. 宣告
指標函數的宣告，除了與一般函數一樣需要回傳型別、函數名稱與傳入引數外，其名稱需括號並在前面加上星號代表該函數為指標函數
```C
int a, *b; // a 為一般變數，b 為指標變數
int func1(int, int); //一般函數
int (*func1Ptr)(int, int); //指標函數
```
#### 2. 規格書: http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf
```A function designator is an expression that has function type. Except when it is the operand of the sizeof operator54) or the unary & operator,afunction designator with type ‘‘function returning type’’ is converted to an expression that has type ‘‘pointer to function returning type’’.```
