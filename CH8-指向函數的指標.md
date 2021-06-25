我們從前面幾章已知變數有一般變數 ```(int a;)``` 與指標變數 ```(int *a;)```，而函數本身若有回傳值，因其佔有記憶體位置與值，故也可當成變數 ```(int add(int a, int b))```在此我們稱其為一般函數，而函數也可回傳指標，原形為```int *add(int a, int b)```。另一個指向函數的指標則稱為**指標函數**(A pointer points to function) ```int (*compare)(const void*, const void*)```，本質上仍為指標，兩者在宣告上非常相似。

## 1. 函數回傳指標
如上所述，其原型為 ```int *add(int a, int b)``` ```int* add(int a, int b)```，宣告方式與一般的指標宣告方式幾乎一樣。然而此種使用方式一般會以傳入指標或陣列取代，在此不多給範例說明。\
不過須注意如果回傳一陣列，若非用動態宣告而是靜態宣告，則因該宣告是 local variable，故回傳會有錯誤。

## 2. 指標函數
如之前所提到，函數本身也佔有一記憶體位置與空間，故可以宣告一指標指向該函數的位置，指標函數本身仍為指標。
#### 1. 宣告
指標函數的宣告，除了與一般函數一樣需要回傳型別、函數名稱與傳入引數外，其名稱需括號並在前面加上星號代表該函數為指標函數，因指標宣告的星號 * 運算優先次序比用為函數參數列的小括弧為低。
```C
int a, *b; // a 為一般變數，b 為指標變數
int func1(int, int); //一般函數
int (*func1Ptr)(int, int); //指標函數
```
#### 2. 規格書: http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf
```A function designator is an expression that has function type. Except when it is the operand of the sizeof operator or the unary & operator, a function designator with type ‘‘function returning type’’ is converted to an expression that has type ‘‘pointer to function returning type’’.```\
function designator 就是 function 名稱，除了作為 sizeof 或取位址 & 的運算元，函式指示符在表達式中自動轉換為函式指標類型右值 (為一個不佔有記憶體位置的值)。例如
```C
int func1(int, int); //一般函數
int *func(int, int); //一般函數，回傳值為指標
int (*func1Ptr)(int, int); //指標函數
func1Ptr = &func1
func1Ptr = func1
```
當指標函數指向一函數後，即可像使用原函數一樣使用。須注意指標的型別，在此須多注意傳入的引數順序、引數型別與引數個數皆要與原函數相同。

#### 3. 指標函數當作引數
C 語言雖然不是物件導向的語言，但是 C 語言可以利用指標來達成物件導向的寫法。但是實際上是傳入一個指標，例如有一個函數 A，引數包含一個指標函數且指向 B，指標傳入後這個指標會指向 B 函數，所以傳入之後就能在 A 函數裡面指向 B 函數。在之前已經有了一個例子，例如在標準函式庫中的 qsort 與 bsearch
```C
int compare(const void * a, const void * b)
{
    return *(int *)a > *(int *)b ? 1 : -1;
}

void qsort(void* base, size_t nitems, size_t size, int (*compare)(const void*, const void*));
// *base: an array waited for sorting.
// nitems: number of element in array.
// size: size of each element in the array.
// compare: > 0 for ascend, < 0 for descend.
// It’s an in-place version.

void* bsearch (const void* key, const void* base, size_t num, size_t size, int (*compare)(const void*,const void*));
// *key: the item you want to find
// *base: an array waited for searching.
// num: number of element in array.
// size: size of each element in the array.
// compare: > 0 for ascend, < 0 for descend.
// If the item is not in the array, then it return NULL.
```
在 qsort 與 bsearch 中的最後一個引數即為指標函數，這個指標函數是回傳一個整數，並傳入兩個唯讀的泛型指標。這個指標函數傳入之後，會把另外傳入的泛型指標 base 裡面的東西丟進去做比較。如同變數一樣，當我們宣告變數後，這個變數即有了名稱、位置與數值，所以雖然我們是宣告一個一般函數，還是可以傳入一個指標函數。

#### 4. 指標函數與 typedef
typedef 這個關鍵字可以將那些很長的關鍵字或是型別縮短，但無法將與儲存類別有關的關鍵字(static, extern, auto, register)納入縮寫，例如
```C
typedef 型別 別名;
typedef unsigned int uint;  //把無號整數的這個型別給了一個別名叫做 uint
typedef struct data {
   int HR;
   float sp;
   int age;
} userInfo; //把這個結構體的型別給了一個別名叫做 userInfo
typedef static int sint; //會報錯
typedef const int* rv;   //把這個唯讀整數指標的型別給了一個別名叫做 rv
typedef const int rp;    //把這個唯讀整數位置的型別給了一個別名叫做 rp
typedef int* iptr;       //把這個整數指標的型別給了一個別名叫做 iptr

uint height;   //== unsigned int height
userInfo peter;//== struct data peter
rv b, c;       //== const int* b, c; b 指標，c 為變數 (等同 rp c;)，其值皆不可改變
const iptr d;  //== const int d; 在此先宣告了一個整數指標 d，並將此指標位置設為唯讀
```
同樣的，在指標函數這種很長的宣告我們也可以將其縮寫
```C

```
