/*
词法分析器
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "token.h"

static char *st_line;//读到的字符串
static int st_line_pos;//分析当前字符串，分析到的位置

typedef enum {
    INITIAL_STATUS,//开始token分析
    IN_INT_PART_STATUS,//当读到数字时，在dot（.）字符出现之前的状态
    DOT_STATUS,//小数点
    IN_FRAC_PART_STATUS//当读到数字时，在小数点后
} LexerStatus;



void get_token(Token *token)
{
    int out_pos = 0;//当前要得到的token的字符串，读到的位置
    LexerStatus status = INITIAL_STATUS;
    char current_char;//记录当前读到的字符

    token->type = ERR_TOKEN;//在不匹配任何类型之前，是错误的
    while (st_line[st_line_pos] != '\0') {
        current_char = st_line[st_line_pos];
        if ((status == IN_INT_PART_STATUS || status == IN_FRAC_PART_STATUS)
            && !isdigit(current_char) && current_char != '.') {
            //1.当读到非数字和小数点
            //2.但之前是数字
            //-》所以读取数字完成
            token->type = NUM_TOKEN;
            sscanf(token->str, "%lf", &token->value);//给value赋值
            break;//跳出while
        }
        if (isspace(current_char)) {
        //如果当前的字符是空白字符
            if (current_char == '\n') {
            //回车符号，输入完成，标记line完成符号
                token->type = END_TOKEN;
                return;
            }
            st_line_pos++;
            continue;
        }

        if (out_pos >= MAX_TOKEN_SIZE-1) {
            fprintf(stderr, "token too long.\n");
            exit(1);
        }
        //将新读取的字符，放入token中
        //因为判断字符串是否为数字，需要提前读取一个字符，
        //因此，将是否为数字的判断操作，放在将当前字符写入token的前面
        token->str[out_pos] = st_line[st_line_pos];
        st_line_pos++;
        out_pos++;
        token->str[out_pos] = '\0';

        //标记token，得到后返回
        if (current_char == '+') {
            token->type = ADD_OP_TOKEN;
            return;
        } else if (current_char == '-') {
            token->type = SUB_OP_TOKEN;
            return;
        } else if (current_char == '*') {
            token->type = MUL_OP_TOKEN;
            return;
        } else if (current_char == '/') {
            token->type = DIV_OP_TOKEN;
            return;
        } else if (current_char == '(') {
            token->type = LEFT_PAREN_TOKEN;
            return;
        } else if (current_char == ')') {
            token->type = RIGHT_PAREN_TOKEN;
            return;
        } else if (isdigit(current_char)) {
            //当读入数字时，判断在小数点前面，或后面
            if (status == INITIAL_STATUS) {
                status = IN_INT_PART_STATUS;
            } else if (status == DOT_STATUS) {
                status = IN_FRAC_PART_STATUS;
            }
        } else if (current_char == '.') {
            //读入小数点
            if (status == IN_INT_PART_STATUS) {
                status = DOT_STATUS;
            } else {
                //不符合小数的规范写法，报错
                fprintf(stderr, "syntax error.\n");
                exit(1);
            }
        }
    }
}



void set_line(char *line)//初始化要分析的字符串
{
    st_line = line;
    st_line_pos = 0;
}
/*测试代码
void parse_line(char *buf)
{
    Token token;
    //将得到的buf赋值
    set_line(buf);

    for (;;) {
        get_token(&token);
        if (token.type == END_TOKEN || token.type == ERR_TOKEN) {
          if (token.type == ERR_TOKEN) {
              printf("err: 未识别的语句!\n");
          }
          break;
        } else {
            printf("type->%d, str->%s\n", token.type, token.str);
        }
    }
}

int main(int argc, char **argv)
{
    char buf[1024];
    while (fgets(buf, LINE_BUF_SIZE, stdin) != NULL) {
        parse_line(buf);
    }

    return 0;
}
*/
