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
            token->kind = NUMBER_TOKEN;
            sscanf(token->str, "%lf", &token->value);//给value赋值
            return;//跳出while
        }
        if (isspace(current_char)) {
            if (current_char == '\n') {
                token->kind = END_OF_LINE_TOKEN;
                return;
            }
            st_line_pos++;
            continue;
        }

        if (out_pos >= MAX_TOKEN_SIZE-1) {
            fprintf(stderr, "token too long.\n");
            exit(1);
        }
        token->str[out_pos] = st_line[st_line_pos];
        st_line_pos++;
        out_pos++;
        token->str[out_pos] = '\0';

        if (current_char == '+') {
            token->kind = ADD_OPERATOR_TOKEN;
            return;
        } else if (current_char == '-') {
            token->kind = SUB_OPERATOR_TOKEN;
            return;
        } else if (current_char == '*') {
            token->kind = MUL_OPERATOR_TOKEN;
            return;
        } else if (current_char == '/') {
            token->kind = DIV_OPERATOR_TOKEN;
            return;
        } else if (current_char == '(') {
            token->kind = LEFT_PAREN_TOKEN;
            return;
        } else if (current_char == ')') {
            token->kind = RIGHT_PAREN_TOKEN;
            return;
        } else if (isdigit(current_char)) {
            if (status == INITIAL_STATUS) {
                status = IN_INT_PART_STATUS;
            } else if (status == DOT_STATUS) {
                status = IN_FRAC_PART_STATUS;
            }
        } else if (current_char == '.') {
            if (status == IN_INT_PART_STATUS) {
                status = DOT_STATUS;
            } else {
                fprintf(stderr, "syntax error.\n");
                exit(1);
            }
        }
    }
}

void
set_line(char *line)
{
    st_line = line;
    st_line_pos = 0;
}

#if 0
void
parse_line(void)
{
    Token token;
    st_line_pos = 0;

    for (;;) {
        get_token(&token);
        if (token.kind == END_OF_LINE_TOKEN) {
          break;
        } else {
            printf("kind..%d, str..%s\n", token.kind, token.str);
        }
    }
}

int
main(int argc, char **argv)
{
    while (fgets(st_line, LINE_BUF_SIZE, stdin) != NULL) {
        parse_line();
    }

    return 0;
}
#endif

