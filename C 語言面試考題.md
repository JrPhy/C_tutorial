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
