在每個系統與程式語言中，都有一套管理記憶體的方法。而越高階的語言(離硬體越遠)越不用煩惱記憶體，反之則要很小心。而 C 語言是非常靠近硬體的語言，在記憶體使用上的自由度非常高，但也會引發記憶體洩漏(Memory leak)的問題。\
在 C 語言中用範圍(scope)，連結(linkage)，持續時間(duration) 以及 static 和 extern 關鍵字來控制變數的生命周即與可視範圍。
## 1. 範圍 scope
C 語言中的範圍總共有以下四個
#### 1. 大括號 {}
#### 2. 函數引數 int add(int a, int b)
#### 3. 函數內容 int add(int a, int b) { ... }
#### 4. .c檔案
若一個變數在函數以外，稱為全域變數(global variable)，反之則稱為區域變數(local variable)
```C
int a; // 全域變數
swapi(int *a, int *b)
{
    int temp = *a; //區域變數
    *a = *b;
    *b = temp;
}

swapf(float *a, float *b)
{
    float temp = *a; //區域變數
    *a = *b;
    *b = temp;
}
```
由上例子可知，若是在不同的範圍內變數名稱是可以一樣的。若是在同一個函數但是被大括號分成不同的範圍，則以同一個範圍內的名稱優先序最高
```C
int a = 4, b = 4; //全域變數
printf("%d, %d\n", a, b); //4, 4
int main()
{
    int a = 1, b = 1; //區域變數
    {
        int b = 2; //區域變數
        {
            int a = 3; //區域變數
            printf("%d, %d\n", a, b); // 3, 2
        }
        printf("%d, %d\n", a, b); // 1, 2
    }
    printf("%d, %d\n", a, b); // 1, 1
    return 0;
}
```
若是有相同的變數名稱 k，且當函數使用了該變數，則以變數傳入的範圍為主
```C
int k = 5; //全域變數
void test(void)
{ printf("%d\n", k); }

void test1(int k)
{ printf("%d\n", k); }

int main(){
    int k = 0; //區域變數
    test();   // 5
    test1(k); // 0
    printf("%d\n", k); // 5
    return 0;
}
```
上述的例子中 test() 並無需要引數，且此函數寫在 main() 外面，所以會使用到全域變數。而 test1(k) 是傳入區域變數。\
雖然 C 語言以大括號來區分範圍，但是在非常短的敘述內(單行字數不超過 80 字)是可以省略大括號的，這是 C99 後加入的新特性。
```C
int sum = 0;
for (int i = 0; i < 10; i++) sum += i;

for (int i = 0; i < 10; i++) 
{
    if (i == 5) printf("%d\n", i);
    else printf("%d ", i);
}
```

## 2. 連結 Linkage
由前可知 C 語言利用括號將變數分開，稱為區塊(block)，但若變數想要與別的範圍共用也是可以的。C 語言提供了 static 與 extern 兩個關鍵字將不同範圍做連結。\
若是使用到大型專案，擇一個專案中會有許多個 .c 檔，且會希望某些函數只能在該檔案中使用，有些則希望能夠共用。
#### 1. 無連結 no linkage
一般若無特別下這兩個關鍵字，則都是預設。
#### 2. 內部連結 internal linkage
當使用了 static 關鍵字後，該變數或函數就只能在**同一個**檔案內被使用。
#### 3. 外部連結 external linkage
當使用了 extern 關鍵字後，該變數或函數能在**不同**檔案內被使用。

## 3. 儲存持續時間 Storage Duration
若是該變數沒有與其他範圍做連結，則當該範圍結束後就會自動釋放該變數以便騰出記憶體空間。在 C 語言中的持續時間分成下列四個
#### 1. 自動 auto
一般若無特別下其他關鍵字，則都預設為自動持續時間。\
註：C 語言中有 auto 關鍵字，但 C99 之後都是預設 auto，跟 C++ 中的 auto 完全不同意義。
#### 2. 靜態 static
這邊的 static 指的是變數的生命週期而非關鍵字。意思是指當**區域變數**使用了 static 做修飾，則該變數會一直存在，直到程式結束。
#### 3. 分配 allocated
在 C 語言中提供了 malloc, calloc, realloc, free 四組函數來上程式設計者自行決定什麼時候要該塊記憶體並釋放。
#### 4. 執行緒
C11 中提供了 Thread.h 這個標頭檔讓程式設計者可以更方便的使用多執行緒，並以 **__thread** 修飾變數。\
當該執行緒結束之後此變數就被釋放掉。

以下做個整理：
| 儲存類別 | 持續時間 | 範圍 | 連結 | 如何宣告 |
| --- | --- | --- | --- | --- |
| 自動 | 自動 | 區塊 | 無 | 預設，不用特別加關鍵字 |
| 靜態外連結 | 靜態 | 檔案 | 外 | 在函數外面(建議用 extern) |
| 靜態內連結 | 靜態 | 檔案 | 內 | 在函數外且前面加 static |
| 靜態無連結 | 靜態 | 區塊 | 無 | 在區域內且前面加 static |

## 4. 關鍵字 static/extern
這兩個關鍵字除了讓變數的可視範圍不同外，其餘都一樣，使用這兩個修飾變數都需要給**初始值**，且生命週期都是到程式結束。
#### 1. static
由上可知 static 關鍵字除了改變區域變數的生命週期外，還將全域變數或是函數的可視範圍限縮在同一個檔案內。
```C
int a = 5; //預設是 extern，但若是要與其他檔案共用，建議還是加上 extern 關鍵字
static int b = 3; //只能在此檔案中使用
static int count(int a, int b); //只能在此檔案中使用
int main()
{
      ...
}
static int count(int a, int b)
{
    static int c = 0; //生命週期與 a, b 相同
    return c;
}
```
#### 2. extern
若有其他的 .c 檔或是在不同 scope 中想要共用同一個變數或函數，則編譯器看到此關鍵字後會使用某一塊記憶體空間，並讓其他的 .o 檔來使用同一個變數或函數。當一變數使用 extern 修飾，則該變數**不能**初始化。
a.c
```C
int count = 10;
```
main.c
```C
int main()
{
    extern int count; //告訴編譯器在其他 .c 檔案中有相同名稱的變數且使用那個
    count++;
    return 0
}
```
#### 3. extern "C"
在許多 C++ 的檔案中，如果使用 IDE 開啟專案，就會在 .h 中看到這行字，這是因為在 C++ 中支援多載函式，也就是同一個函數名可以回傳不同型別的值。然而編譯器其實會自行產生許多個函數，但是 C 就不支援。所以當同一個檔案可能會給 C 和 C++ 使用時，這行字就會派上用場。

## 5. 關鍵字 const
此關鍵字與 constant 縮寫完全一致，但意義為「唯讀 read-only」。當變數使用此關鍵字修飾，則在其生命周期間該數值都**不能被程式碼修改**。\
此關鍵字可與 static/extern 關鍵字一起使用，即可達到 #define 的效果。
當此關鍵字修飾指標值，則表示該指標位置不能被改變。而指標也可以取值，所以此關鍵字的用法如下
```C
static const int c_vacuum = 299792458; // m/s
int main()
{
    const int d = 10; //此變數的值在其生命週期內不能改變
    c_vacuum = 3e8; //會報錯
    int a = 10, b = 15;
    const int *p = &a;
    // *p = 12 --> error; p = &b --> ok
    //因為 *p 是取該指標的值，所以用 const 修飾後意思就是該指標的值不能改變。
    int* const q = &a;
    // *q = 12 --> ok; q = &b --> error
    //因為 q 是取該指標的位置，所以用 const 修飾後意思就是該指標的位置不能改變。
    const int* const r = &a;
    //此種用法及指標的值和位置都不能改變。
    return 0;
}
```
#### 1. 與 #define 的差異
const 變數在變編譯後會確實生成一個位置，也會占用記憶體。而 #define 則是會直接把該字段取代，並不會占用記憶體。
| 程式碼 | 組語 | 
| --- | --- |
| #define a 10 | pushq   %rbp |
| int b, c; | movq    %rsp, %rbp |
| const int d = 5; | movq    %rsp, %rbp |
| int main(void) { | movl    $10, b(%rip) |
|     b = a; | movl    $10, b(%rip) |
|     c = d; | movl    $5, %eax |
|            | movl    %eax, c(%rip) |
| } | popq    %rbp |

可以看到若用 #define，a 會直接被改成 10。而 d 還是會占用一個空間，然後再把 d 的值搬到一個暫存器，會有兩個指令。所以在一些很低階的 MCU 中，#define 會比 const 常用
