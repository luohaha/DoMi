#一个自己实现的好玩的脚本语言#
*还在不断的完善的过程中*  
*domi来自海南话中“干什么”的音译*
###使用###
首先执行make，生成可执行文件domi，然后运行  
`./domi [filename]`
 解释后缀名为.dm的文件。

###实例:###
1.简单打印数字

```c
int a = 1;
int b = 2;
out(a+b);
```

结果：3  

2.while循环

```c
int b = 5;
while (b) {
   out(b);
   b = b - 1;
};
```

结果：5\n4\n3\n2\n1  

3.函数声明与调用

```c
int b = 12;
func sum(int d) {
     int a = 13;
     out(a + b + d)
}
sum(14);
```

结果：39
