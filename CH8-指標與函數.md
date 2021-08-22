我們從前面幾章已知變數有一般變數 ```int a;``` 與指標變數 ```int *a;```，而函數本身若有回傳值，因其在被呼叫時佔有記憶體位置與值，故也可當成變數 ```int add(int a, int b)```在此我們稱其為一般函數，而函數也可回傳指標，原形為```int *add(int a, int b)```。另一個指向函數的指標則稱為**指標函數** (A pointer points to function) ```int (*add)(int a, int b)``` ```int (*compare)(const void*, const void*)```，本質上仍為指標，兩者在宣告上非常相似。因為指標函數會牽涉到許多括號與星號，故建議先看以下網址再看範例  https://magicjackting.pixnet.net/blog/post/60889356 

## 1. 只有 call by value
在 C 語言中常常會聽到函數有 call by value, call by address, 甚至 call by reference(C++ 才有)。而在規格書[1]中提到
```A pointer type may be derived from a function type, an object type, or an incomplete type, called the referenced type. A pointer type describes an object whose value provides a reference to an entity of the referenced type. A pointer type derived from the referenced type T is sometimes called ‘‘pointer to T’’. The construction of a pointer type from a referenced type is called ‘‘pointer type derivation’’. ```\
所以只有 call by value。而傳入物件時都會先複製一份該物件，然後再把**複製的那份傳入函數中**，所以在寫 swap 函數時是傳入 addrres，把地址複製一份後傳入函數中，裡面的指標就會指向傳入的物件位置。
```C
#include<stdio.h>
void swap1(int a, int b) {int temp = a; a = b, b = temp;} 
void swap2(int *a, int *b) {int temp = *a; *a = *b, *b = temp;}
int main()
{
	int a = 1, b = 2, *pa = &a, *pb = &b;
	printf("%d, %d, %d, %d\n", a, b, *pa, *pb);
	swap1(a, b);    //不改變 :)
	printf("%d, %d, %d, %d\n", a, b, *pa, *pb);
	swap2(&a, &b);  //a, b 值會改變
	printf("%d, %d, %d, %d\n", a, b, *pa, *pb);
	return 0;
}
```
依照此想法，若想改變指標的指向該物件的值，那麼可以傳入一個指標的指標。
```C
#include<stdio.h>
void swap(int**p1 ,int **p2) {int* temp; temp = *p1; *p1 = *p2; *p2 = temp;}
int main()
{
	int a = 1, b = 2, *pa = &a, *pb = &b;
	printf("%d, %d, %d, %d\n", a, b, *pa, *pb);
	swap(&pa, &pb);//pa, pb 值會改變，但 a, b 值不會改變
	printf("%d, %d, %d, %d\n", a, b, *pa, *pb);
	return 0;
}
```

## 2. 函數回傳指標
如上所述，其原型為 ```int *add(int a, int b)```或```int* add(int a, int b)```，宣告方式與一般的指標宣告方式幾乎一樣。然而此種使用方式一般會以傳入指標或陣列取代，在此不多給範例說明。\
不過須注意如果回傳一陣列，若非用動態宣告而是靜態宣告，則因該宣告是 local variable，故回傳會有錯誤。

## 3. 指標函數
如之前所提到，函數本身也佔有一記憶體位置與空間，故可以宣告一指標指向該函數的位置，指標函數本身仍為指標。
#### 1. 宣告
指標函數的宣告，除了與一般函數一樣需要回傳型別、函數名稱與傳入引數外，其名稱需括號並在前面加上星號代表該函數為指標函數，因指標宣告的星號 * 運算優先次序比用為函數參數列的小括弧為低。
```C
int a, *b; // a 為一般變數，b 為指標變數
int func1(int, int); //一般函數
int (*func1Ptr)(int, int); //指標函數
```
其中 int (* )(int, int) 為指標函數的型別，func1Ptr 為指標函數的名稱。
#### 2. 算符優先序
由前面所提到的 add 函數可知，```int *add(int a, int b)```是回傳整數指標的函數，```int (*add)(int a, int b)```是回傳整數的指標函數，這是因為函數的 () 優先序高於指標的 * ，所以若是要宣告指標函數，則需要將 * 與函數名稱括號起來才是指標函數。

#### 3. 規格書: http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf
```A function designator is an expression that has function type. Except when it is the operand of the sizeof operator or the unary & operator, a function designator with type ‘‘function returning type’’ is converted to an expression that has type ‘‘pointer to function returning type’’.```\
function designator 就是 function 名稱，除了作為 sizeof 或取位址 & 的運算元，函式指示符在表達式中自動轉換為函式指標類型 (為一個不佔有記憶體位置的值)。例如
```C
int func1(int, int); //一般函數，回傳值為整數
int *func(int, int); //一般函數，回傳值為指向整數的指標
int (*func1Ptr)(int, int); //指標函數，回傳值為整數
int *(*func2Ptr)(int, int); //指標函數，回傳值為指向整數的指標
func1Ptr = &func1;   //與下面意思一樣
func1Ptr = func1;    //與上面意思一樣
```
當指標函數指向一函數後，即可像使用原函數一樣使用。須注意指標的型別，在此須多注意傳入的引數順序、引數型別與引數個數皆要與原函數相同。

#### 3. 指標函數當作引數
C 語言雖然不是物件導向的語言(Object-Oriented Programming Language, OOPL)，但可利用指標來達成物件導向的寫法做到物件導向(OOP)的功能。在此雖然看起來像傳入一個函數，但是實際上是傳入一個指標。例如有一個函數 A，引數包含一個指標函數且指向 B，指標傳入後這個指標會指向 B 函數，所以傳入之後就能在 A 函數裡面指向 B 函數。在之前已經有了一個例子，例如在標準函式庫中的 qsort 與 bsearch
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

若是 qsort 函數原型寫成以下形式且用以下使用方式，則為傳入一個整數而非函數。
```C
int compare(const void * a, const void * b)
{
    return *(int *)a > *(int *)b ? 1 : -1;
}

void qsort(void* base, size_t nitems, size_t size, int compare);
qsort(base, nitems, size, compare(const void * a, const void * b));
//等同於 qsort(base, nitems, size, 1); or qsort(base, nitems, size, -1); 
```
雖然看起來像是傳入函數，但實際上 base 裡面的物件丟進 compare 函數裡面，可能回傳 1 也可能是 -1，此種寫法只能在 qsort 函數外面做比較，而非在 qsort 函數裡面做比較。\
因為函數引數裡面也是一個 scope，所以會先將 compare 的回傳值放入 int compare 中，再將此值傳入函數中，所以實際上此種寫法跟傳入 1 or -1 是一樣的。

#### 4. 指標函數與 typedef
typedef 這個關鍵字可以將那些很長的關鍵字或是型別縮短，但無法將與儲存類別有關的關鍵字(static, extern, auto, register)納入縮寫，例如
```C
typedef 型別 別名;
typedef unsigned int uint;//把無號整數的這個型別給了一個別名叫做 uint
typedef struct data {
   int id;
   float score;
} result;                //把這個結構體的型別給了一個別名叫做 result
typedef static int sint; //會報錯
typedef const int* rv;   //把這個唯讀整數指標的型別給了一個別名叫做 rv
typedef const int rp;    //把這個唯讀整數位置的型別給了一個別名叫做 rp
typedef int* iptr;       //把這個整數指標的型別給了一個別名叫做 iptr

uint id;   //height 為無號整數型別
data peter;//peter 為 data 型別
rv b, c;       //== const int* b, c; b 指標，c 為變數 (等同 rp c;)，其值皆不可改變
iptr d, e;     //== int *d, *e;
const iptr f;  //== const int f; 在此先宣告了一個整數指標 f，並將此指標位置設為唯讀
```
同樣的，在指標函數這種很長的宣告我們也可以將其縮寫
```C
typedef int (*func1Ptr)(int, int); //把此指標函數的的這個型別給了一個別名叫做 func1Ptr
func1Ptr func; 
```
上述例子中 int (* )(int, int) 是一個型別，func1Ptr 是一個名稱，所以當指標函數搭配 typedef 使用後就可以讓整體程式碼看起來更簡潔。

## 4. 呼叫 dll (動態函式庫)
在 windows 很多應用程式中都會有包含 .dll 這個檔案，全名為 dynamic-link library。顧名思義當使用到的時候我們會去開一個記憶體位置，將該記憶體位置指向 .dll 裡面的函數拿出來使用，以下就給一個 C 語言呼叫 .dll 的方法。
```C
#include <stdio.h>
#include <windows.h>  //此為呼叫 .dll 所需之標頭檔
typedef struct
{
    int x;
    int y;
} Point;
typedef void (*DLLshow_point)(Point point);
typedef void (*DLLmove_point)(Point point);
typedef void (*DLLmove_point_by_ref)(Point *point);
typedef Point (*DLLget_point)(void);
//在此我們必須先知道 .dll 裡面的函數原型長得如何，當然也可以不用 typedef，只是寫起來就比較冗長不易讀。
HINSTANCE hDll;  //此為需要讀取 .dll 檔案的型別
DLLshow_point show_point;
//在此宣告了一個指標函數，其型別為 void (*)(Point point)，以下類推。
DLLmove_point move_point;
DLLmove_point_by_ref move_point_by_ref;
DLLget_point get_point;
int main()
{
    hDll = LoadLibrary("call struct by python.dll");
    if(hDll == NULL) printf("no dll");
    show_point = (DLLshow_point)GetProcAddress(hDll, "show_point");
    //這個意思是 .dll 裡面有個函數名稱為 show_point，然後放進 show_point 這個指標內。
    move_point = (DLLmove_point)GetProcAddress(hDll, "move_point");
    move_point_by_ref = (DLLmove_point_by_ref)GetProcAddress(hDll, "move_point_by_ref");
    Point point = {1, 2};
    show_point(point);
    move_point_by_ref(&point);
    show_point(point);
    FreeLibrary(hDll);
    //因為是動態宣告，所以記得最後要使用 FreeLibrary 函數 free 記憶體。
    return 0;
}
```

## 5. 善用指標函數減少程式碼
在某些例子中，我們會需要使用一個 FLAG 來決定需要呼叫哪支函數，例如
```C
typedef unsigned char uchar
void func0(...) {...};
void func1(...) {...};
void func2(...) {...};

bool OnStateChange(uchar flag) 
{
    if (flag == 0) func0(...);
    else if (flag == 1) func1(...);
    else if (flag == 2) func2(...);
    else 
    {
        printf("ERROR!\n");
        return false;
    }       
    return true;
}

int main()
{
    uchar flag = 1;
    OnStateChange(flag);
    return 0;
}
```
上述例子中若使用指標函數改寫則會變成簡潔許多。
```C
typedef unsigned char uchar
void func0(...) {...};
void func1(...) {...};
void func2(...) {...};

static void (*command[])(void) = {func0, func1, func2};

bool OnStateChange(uchar flag) 
{
    if (flag > 2 || flag < 0) 
    { 
        printf("ERROR!\n");
        return false;
    }

    command[flag]();
    return 0;
}

int main()
{
    uchar flag = 1;
    OnStateChange(flag);
    return 0;
}
```

## 6. 函數封裝
C 語言利用 struct 及 pointer 來達到封裝與繼承的效果，利用 void* 或 #define 來達到多型的效果，在此示範使用指標函數來達到物件導向內呼叫類別中函數的效果。
```C
#include<math.h>
#include<stdio.h>

double expo(double number) {return exp(number);}

double logr(double number) {return log(number);}

double mean1(double *series, int length)
{
    double a = 0.0;
    for(int i = 0; i < length; i++) a += series[i];
    return a/length;
}

double std1(double *series, int length) 
{
    double a = 0.0, b;
    for(int i = 0; i < length; i++) 
    {
        a += series[i]*series[i];
        b += series[i];
    }
    b /= length;
    return (a/length - b*b);
}

typedef struct _numpyFunc
{
    double (*exp1)(double number);
    double (*log1)(double number);
    double (*mean)(double *series, int length);
    double (*std)(double *series, int length);
}numpy;

int main()
{
    numpy np =
    {
        .exp1 = expo,
        .log1 = logr,
        .mean = mean1,
        .std = std1,
    };
    double a = 0.5, b[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6};
    printf("%f  %f  %f  %f  \n", np.exp1(a), np.log1(a), np.mean(b, 6), np.std(b, 6));
    return 0;
}
```

## 7. 一些指標函數原型解讀
指標在 C 語言中是一個非常強大的物件，但有些指標函數沒搭配 typedef 在閱讀起來不是那麼容易，以下為某個函數的原型
```C
extern void (*signal(int, void(*)(int)))(int);
```
我們可以先從最內層的括號來看\
```signal(int, void(* )(int))``` 表示 signal 一個函數，其引數為整數與一個指標函數，該指標函數的引數為整數且不回傳值\
```void (* signal(int, void(* )(int)))(int)```則表示這是一個指標函數，其引數為 int 且不回傳值。\
當這個函數搭配一個好的 typedef，則閱讀起來會容易許多
```C
typedef void (*SignalHandler)(int);
extern SignalHandler signal(int signum, SignalHandler handler);
```
https://stackoverflow.com/questions/1591361/understanding-typedefs-for-function-pointers-in-c \
https://stackoverflow.com/questions/15739500/how-to-read-this-prototype 


[1] http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf
