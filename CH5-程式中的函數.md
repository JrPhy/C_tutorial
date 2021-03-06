一般初學者在寫程式時通常會把所有要用的東西都放在 main 函式中，雖然程式可以正常運作，但是可讀性非常差且會讓程式碼非常冗長，故會將重複使用到的部分提出 
main 以外，需要呼叫時就可以重複使用。
## 1. 程式中函數的位置
在 C 語言中將函式提到 main 外面的寫法有以下兩種。假設我們要寫一函數找出一陣列中的最大值，因為程式碼是從上面往下走，我們可以將所要使用到的函數寫在 
main 函數之前
```C
#include<stdio.h>
int max(int a, int b)
{
    if(a >= b) return a;
    else return b;
}
 
int main()
{
    int array[] = {6, 4, 9, -1, 5, 8, 11, -3};
    int value = -9999, i;
    for(i = 0; i < 8; i++)
    value = max(value, array[i]);
    printf("%d\n", value);
    return 0;
}
```
也可將函數原型先宣告於 main 之前，在將函數的詳細內容寫在 main 之後。此時當 main 函式中呼叫到你所宣告的函數名稱時，就會去往下找他的實做方式
```C
#include<stdio.h>
int max(int a, int b);

int main()
{
    int array[] = {6, 4, 9, -1, 5, 8, 11, -3};
    int value = -9999, i;
    for(i = 0; i < 8; i++)
    value = max(value, array[i]);
    printf("%d\n", value);
    return 0;
}

int max(int a, int b)
{
    if(a >= b) return a;
    else return b;
}
```
在多人開發或大型程式開發實，會建議先將函數原型宣告出來，並在 main 函數後實作，因為有可能函數 A 需要呼叫到函數 B，但是 B 函數的實作在 A 之前，
若是用第一種方式則會報錯，而且也不清楚這一支程式中有那些函式。因此若是將函式的原型先宣告在 main 之前除了無關順序外，也可以讓讀者容易了解這隻
程式使用到了哪些函數及其原型。

## 2. 傳值入函數
函數若要傳值的話需要有引數，如果想要在函數內交換兩個變數的值，通常初學者會寫以下函數
```C
#include<stdio.h>
int swap(int a, int b);

int main()
{
    int a = 5, b = 10;
    swap(a, b);
    printf("%d\t%d\n", a, b);
    return 0;
}

int swap(int a, int b)
{
    int temp = a;
    a = b;
    b = temp;
}
```
但是結果不如預期把兩變數數值交換，原因是這種行為在程式裡面會先將 a, b 的值複製一份出來，**傳入複製的那份進函數裡面**，所以雖然在函數內的寫法沒錯，但是實際上並沒有真的將值交換。
