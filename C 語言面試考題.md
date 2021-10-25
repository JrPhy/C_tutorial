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

