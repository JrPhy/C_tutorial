C 語言是一種編譯語言，也就是程式設計師寫完程式後需要經過編譯器，將所寫的程式轉成電腦看得懂的語言，可以算是將人類的語言轉譯。而編譯流程主要分以下四個階段：
1. 預處理
   將以下關鍵字做字串取代或是邏輯分析\
   #include, #define, #undef, #if, #elif, #else, #endif, #ifdef, #ifndef, #error, #warning, #pragma. 
2. 彙編\
   做詞意分析、語意分析、語法分析，中間程式碼產生，最佳化程式碼。不同的編譯器會有不同的邏輯，錯的理由也不盡相同。
3. 組語\
   轉成組合語言並生成 .o
4. 連結/建立\
   將所有的 .o 檔連接起來成為一個 .dll/.so/.exe
   
在程式中編譯器遇到以上預處理的字串，會先對那些字串做展開或條件編譯。

## 1. #define/#undef
在 C 語言中使用 #define 來取代程式中所有相同的文字，若是相同預取代字串出現兩次，則以後面的為主，且最後不用分號。\
#define 預處理字串 要取代成的字串
```C
#include<stdio.h>
#define SIZE 8
#define SIZE 10
int main()
{
   printf("SIZE = %d", SIZE); 
   return 0;
}
```
而 #define 也很適合拿來寫一些非常短的函數，例如
```C
#define CUBIC(x) (x)*(x)*(x) 
```
因為此僅為字串取代，所以並沒有限定傳入的變數型別，因此可以利用 #define 來實現 C 語言中的泛型函數。\
若是要利用 #define 來寫函數，則每個變數盡量都要括號，不然會因算子的優先序而跑出非預期結果。
```C
#define CUBIC1(x) (x)*(x)*(x) 
#define CUBIC2(x) x*x*x
#include<stdio.h>
int main()
{
   printf("CUBIC1 = %d, CUBIC2 = %d", CUBIC1(1+1), CUBIC2(1+1)); 
   return 0;
}
```
雖然兩個在預想中都是期待輸出某數的立方，但是經展開後會是以下結果\
CUBIC1(1+1) = (1+1)* (1+1)* (1+1) = 8\
CUBIC2(1+1) = 1+1* 1+1* 1+1 = 4

因為 #define 是做取代，故在程式執行上速度會稍微快，但是程式碼也會相對大，故必須自己做取捨。
若是程式執行到某一段不想用該 #define，則可以使用 #undef 來取消
```C
#include <stdio.h>  
#define PI 3.1415926  
int square=number*number;  
#undef PI  
int main() {  
   printf("%d",PI);  //這邊編譯器就會報錯
}
```

## 2. #include
對大部分初學者來說，在 C 語言的第一隻程式應該就是在螢幕上印出 hello world，在此最上面就會使用到此預處理功能。
```C
#include<stdio.h>

int main()
{
   printf("hello world");
}
```
在大型程式開發時會將函數的宣告與實現方式分開，或是將變數的宣告與初始化方式分開，使得整體架構變得好管理。通常宣告都是寫在另外的 .h 中，時做與初始化則是寫在 .c 中，當在 .c 中 #include 某個 .h，則會將 .h 內的東西直接展開。
a.h
```C
#ifndef A_H_
#define A_H_

int add(int a, int b);
struct data {
   int HR;
   float sp;
   int age;
} userInfo;
#endif
```
a.c
```C
#include <stdio.h>
#include "a.h"
int add(int a, int b)
{
   printf("%d", a);
   return (a+b);
}
struct data userObject ={165, 4.35, 23};
```
當引入了 a.h 後，實際上在 a.c 的程式碼如下
```C
#include <stdio.h>

int add(int a, int b);
struct data {
   int HR;
   float sp;
   int age;
} userInfo;

int add(int a, int b)
{
   printf("%d", a);
   return (a+b);
}
struct data userObject ={165, 4.35, 23};
```
在上述程式碼中有用 "" 及 <>，"" 為自定義的 .h，若與 .c 不在同一個資料夾中則需另外給明確路徑，<> 為標準函式庫的 .h，通常有預設路徑。

## 3. 條件編譯
在前一節中看到了 #ifndef A_H_ 與 #endif，此即告訴編譯器若沒有 A_H_ (即a.h) 這東西，則定義 a.h。好處是同一個 .h 不會被重複引用。而條件編譯包含了以下幾個
```C
#if, #elif, #else, #endif, #ifdef, #ifndef
```
前四個類似於程式中的 if-else 流程，而在預編譯中使用 #endif 來告訴編譯器條件編譯到這裡結束。而 #ifdef 即為 #if define，#ifndef 即為 #if !define。\
條件編譯的方法除了前面所提到的 .h 保護外，對於版本控制也是很方便的做法。
```C
#define VERSION 1

#if VERSION == 1
...
#elif VERSION == 2
...
#else 
...
#endif
```

## 4. #pragma
此指示是告訴編譯器作特殊的事情，如果此編譯器可以做到此功能就會做，如果不能就忽略，在跨平台移植時很好用。但因為功能太多所以不一一介紹，在此只介紹 pack，其餘可參考以下網址 https://www.itread01.com/content/1526053212.html 。\
#pragma pack(n), n 屬於 2<sup>m</sup> (m = 0, 1, 2, 3, 4, 5, 6)。原本的 struct 會預設對齊，也就是一次要移動幾個 bit 的意思，這樣程式執行速度會比較快但是比較耗費記憶體。當編譯器看到這條指令時，就會根據所設定的數字移動，如此一來就可以省下一些記憶體，在 MCU 上是需要拿時間換空間的。
