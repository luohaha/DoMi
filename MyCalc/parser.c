#include <stdio.h>
#include <stdlib.h>
#include "token.h"

//提前查看到的token
static Token st_look_ahead_token;
//如果提前查看到的token存在的话
static int st_look_ahead_token_exists;



static void my_get_token(Token *token)
{
    //获取token
    //notice：可能存在提前查看的token
    if (st_look_ahead_token_exists) {
    //如果存在提前查看的token，则使用它
        *token = st_look_ahead_token;
    //此时已经没有提前查看的token
        st_look_ahead_token_exists = 0;
    } else {
    //调用lex_ana.c中的词法分析器，获取新的token
        get_token(token);

    }

}



static void unget_token(Token *token)
{
    //将当前token标记为提前查看的token
    //也可以理解为，向后退一步
    st_look_ahead_token = *token;

    st_look_ahead_token_exists = 1;

}



double parse_expression(void);



static double parse_primary_expression()
{
//获取数字，这是优先级别最高的（乘除运算也得有数字才行啊！！！）
    Token token;

    double value = 0.0;

    //标记是否为负数
    int minus_flag = 0;



    my_get_token(&token);//获取token
    if (token.type == ERR_TOKEN) {
        printf("语法错误！");
    }
    if (token.type == SUB_OP_TOKEN) {
    //'-'符号
        minus_flag = 1;

    } else {
    //否则退一步
        unget_token(&token);

    }



    my_get_token(&token);

    if (token.type == NUM_TOKEN) {
        //如果获取到数字，赋值
        value = token.value;

    } else if (token.type == LEFT_PAREN_TOKEN) {
        //当遇到括号时，优先级改变，必须优先运行括号内的运算
        value = parse_expression();

        my_get_token(&token);

        if (token.type != RIGHT_PAREN_TOKEN) {
        //err
            fprintf(stderr, "missing ')' error.\n");

            exit(1);

        }

    } else {
        //如果不是数字，将其标记为提前查看token(退一步)
        unget_token(&token);
        //然后，报错，因为该出现数字的地方没出现
        printf("语法错误\n");
    }

    if (minus_flag) {
    //负数
        value = -value;

    }

    return value;

}



static double parse_term()
{
//乘除运算，优先级第二
    double v1;

    double v2;

    Token token;


    //获取数字
    v1 = parse_primary_expression();

    for (;;) {

        my_get_token(&token);

        if (token.type == ERR_TOKEN) {
            printf("语法错误！");
        }

        if (token.type != MUL_OP_TOKEN&& token.type != DIV_OP_TOKEN) {
        //如果不出现乘与除，退一步
            unget_token(&token);
        //跳出
            break;

        }

        v2 = parse_primary_expression();
        //计算
        if (token.type == MUL_OP_TOKEN) {

            v1 *= v2;

        } else if (token.type == DIV_OP_TOKEN) {

            v1 /= v2;

        }

    }

    return v1;

}



double parse_expression()
{
//加减运算，优先级第三
    double v1;

    double v2;

    Token token;





    v1 = parse_term();

    for (;;) {

        my_get_token(&token);

        if (token.type == ERR_TOKEN) {
            printf("语法错误！");
        }

        if (token.type != ADD_OP_TOKEN 

            && token.type != SUB_OP_TOKEN) {
        //如果没有加减，跳出，退一步
            unget_token(&token);

            break;

        }

        v2 = parse_term();
        //计算
        if (token.type == ADD_OP_TOKEN) {

            v1 += v2;

        } else if (token.type == SUB_OP_TOKEN) {

            v1 -= v2;

        }

    }

    return v1;

}



double parse_line(void)
{

    double value;



    st_look_ahead_token_exists = 0;
    //调用递归下降分析法的最外层
    value = parse_expression();



    return value;

}



int main(int argc, char **argv)

{

    char line[LINE_BUF_SIZE];

    double value;



    while (fgets(line, LINE_BUF_SIZE, stdin) != NULL) {

        set_line(line);

        value = parse_line();

        printf(">>%f\n", value);

    }



    return 0;

}
