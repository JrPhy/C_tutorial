在每個系統與程式語言中，都有一套管理記憶體的方法。而越高階的語言(離硬體越遠)越不用煩惱記憶體，反之則要很小心。而 C 語言是非常靠近硬體的語言，在記憶體使用上的自由度非常高，但也會引發記憶體洩漏(Memory leak)的問題。\
在 C 語言中用範圍(scope)，連結(linkage)，持續時間(duration) 以及 static 和 extern 關鍵字來控制變數的生命周即與可視範圍。
## 1. 範圍 scope
C 語言中的範圍總共有以下四個
#### 1. 大括號   {}
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
{
    printf("%d\n", k);
}

void test1(int k)
{
    printf("%d\n", k);
}

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
