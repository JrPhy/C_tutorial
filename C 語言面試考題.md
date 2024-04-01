## 1. [指標](https://github.com/JrPhy/C_tutorial/blob/main/CH5-%E6%8C%87%E6%A8%99%E8%88%87%E5%AD%97%E4%B8%B2.md)
```C
char s[]="0113256";
char *p=s;           // p 指向 s[0]
printf("%c",*p++);   // 印出 s[0] 的值後移向 s[1]
printf("%c",*(p++)); // 此時 p 指向 s[1]，印出 s[1] 的值後移向 s[2]
printf("%c",(*p)++); // 此時 p 指向 s[2]，印出 s[2] 的值後將 s[2] 的值 +1，此時 s[2] = 2
printf("%c",*++p);   // 此時 p 指向 s[2]，移向 s[3] 後印出 s[3] 的值
printf("%c",*(++p)); // 此時 p 指向 s[3]，移向 s[4] 後印出 s[4] 的值
printf("%c",++*p);   // 此時 p 指向 s[4]，s[4] 的值 +1 後印出 s[4] 的值，此時 s[4] = 3
printf("%c",++(*p)); // 此時 p 指向 s[4]，s[4] 的值 +1 後印出 s[4] 的值，此時 s[4] = 4
printf("\n%s",s);    // 0123456
```

```c
char *str[] = {
    {"MediaTekOnlineTesting"},
    {"WelcomeToHere"},
    {"Hello"},
    {"EverydayGenius"},
    {"HopeEverythingGood"}
};// 長度為 [5] 的字串陣列

char* m = str[4] + 4;
char* n = str[1];
char* p = *(str+2) + 4;
printf("1. %s\n", *(str+1));  //相當於str[1]
printf("2. %s\n", (str[3]+8));  //str[3] 的第八個元素
printf("3. %c\n", *m);  //str[4] 的第四個元素
printf("4. %c\n", *(n+3));  //str[1] 的第三個元素
printf("5. %c\n", *p + 1);  //str[2] 的第四個元素的值 + 1，也就是 o 的下一個字母 p
```
## 2. [位元運算](https://github.com/JrPhy/C_tutorial/blob/main/CH9-%E4%BD%8D%E5%85%83%E9%81%8B%E7%AE%97.md)
#### 1. [Leetcode Single Number](https://leetcode.com/problems/single-number/)
[利用 NOR 運算及其性質來找出陣列中唯一不重複的元素](https://github.com/JrPhy/C_tutorial/blob/main/CH9-%E4%BD%8D%E5%85%83%E9%81%8B%E7%AE%97.md#4-%E6%89%BE%E5%87%BA%E9%99%A3%E5%88%97%E4%B8%AD%E5%94%AF%E4%B8%80%E4%B8%8D%E9%87%8D%E8%A4%87%E7%9A%84%E5%85%83%E7%B4%A0)

#### 2. 

## 3. 進位至轉換
#### 1. 十進位轉十六進位
```c
#include <stdio.h>
void dec2hex(int num, char hex[])
{
    if (!num) {
        hex[0] = '0';
        hex[1] = '\0';
        return;
    }
    char arr[] = "0123456789abcdef";
    int i = 0;
    unsigned int a = num;
    while (a)
    {
        hex[i++] = arr[a & (15)];
        a = a >> 4;
    }
    hex[i] ='\0';
    for (int k = 0, j = i - 1; k <= j; k++, j--) {
        // swapping characters
        char c = hex[k];
        hex[k] = hex[j];
        hex[j] = c;
    }
}
int main()
{
    int num = 255;
    char hex[16];
    dec2hex(num, hex);
    printf("dec %d to hex is: %s", num, hex);
    //sprintf(hex, "%x", num);
    return 0;
}
```

## 4. MAP
#### 1. 找出 < k 以內的質數和
可以先建立一個大小為 k+1 的陣列並都設為 0，然後從 2 開始找，把 index 為 2 的倍數的都設為 1，之後只須看陣列值不為 0 的即可，每個都這樣設，直到 i <= sqrt(k) 為止，因為 k 的最大因數為 sqrt(k)，最後把陣列值為 0 的 index 加起來即可
```cpp
int sumPrimes(int n) {
    if(n < 2) return 0;
    int sum = 0;
    int primeMap[100] = {0};
    // if i is NOT prime, then set index = 1
    for(int i = 2; i* i <= n; i++) {
        if(primeMap[i] == 0) {
            for(int j = i*i; j <= n; j += i) {
                primeMap[j] = 1;
            }
        }
    }
    // sum the prime
    for(int i = 2; i < n+1; i++)
        if(primeMap[i] == 0)
            sum = sum+i;
    return sum;
}
```
