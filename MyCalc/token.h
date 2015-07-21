/*
  定义记号的头文件
*/
#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    //枚举类型，定义token的种类
    ERR_TOKEN,
    NUM_TOKEN,
    ADD_OP_TOKEN,//+,-,*,/
    SUB_OP_TOKEN,
    MUL_OP_TOKEN,
    DIV_OP_TOKEN,
    LEFT_PAREN_TOKEN,//左括号
    RIGHT_PAREN_TOKEN,//右
    END_TOKEN
} TokenType;

#define MAX_TOKEN_SIZE (100) //一个token最长为100个字符
#define LINE_BUF_SIZE (1024) //读入的行的最大空间

typedef struct {
    TokenType type;//类型
    double value;//如果是数字，有数值
    char str[MAX_TOKEN_SIZE];//存放token
} Token;

void set_line(char *line);//将要解析的字符串放入
void get_token(Token *token);//获得token
//int isspace(char c);
//int isdigit(char c);
#endif //#ifndef TOKEN_H
