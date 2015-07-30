#ifndef DOMI_H
#define DOMI_H

#include<stdio.h>
#include "DM_DEV.h"
#include "DM.h"

/*
 *编译时错误的错误类型
 * */
typedef enum {
    //解析错误
    PARSE_ERR = 1,
    //
    CHARACTER_INVALID_ERR,
    //重复定义函数
    FUNCTION_MULTIPLE_DEFINE_ERR,
    //
    COMPILE_ERROR_COUNT_PLUS_1


} CompileError;


/*
 *运行时错误的错误类型
 * */
typedef enum {
    //变量未发现
    VARIABLE_NOT_FOUND_ERR = 1,
    //函数未发现
    FUNCTION_NOT_FOUND_ERR,
    //函数调用时，传入的参数过多
    ARGUMENT_TOO_MANY_ERR,
    //函数调用时，传入的参数过少
    ARGUMENT_TOO_FEW_ERR,
    //不是boolean
    NOT_BOOLEAN_TYPE_ERR,
    //减法错误
    MINUS_OPERAND_TYPE_ERR,
    //
    BAD_OPERAND_TYPE_ERR,

    NOT_BOOLEAN_OPERATOR_ERR,
    //文件打开
    FOPEN_ARGUMENT_TYPE_ERR,

    FCLOSE_ARGUMENT_TYPE_ERR,

    FGETS_ARGUMENT_TYPE_ERR,

    FPUTS_ARGUMENT_TYPE_ERR,

    NOT_NULL_OPERATOR_ERR,
    //除以0的错误
    DIVISION_BY_ZERO_ERR,
    //在声明为全局变量的时候，变量未发现
    GLOBAL_VARIABLE_NOT_FOUND_ERR,
    //
    GLOBAL_STATEMENT_IN_TOPLEVEL_ERR,

    BAD_OPERATOR_FOR_STRING_ERR,

    RUNTIME_ERROR_COUNT_PLUS_1


} RuntimeError;


/*
 *用来标记错误信息中可变部分的类型
 * */
typedef enum {
    //
    INT_MESSAGE_ARGUMENT = 1,

    DOUBLE_MESSAGE_ARGUMENT,

    STRING_MESSAGE_ARGUMENT,

    CHARACTER_MESSAGE_ARGUMENT,

    POINTER_MESSAGE_ARGUMENT,

    MESSAGE_ARGUMENT_END


} MessageArgumentType;



typedef struct {

    char *format;

} MessageFormat;

typedef struct DM_Interpreter_tag {
    //在解释器生成时分配内存，解释器不用时回收
    MEM_Storage         interpreter_storage;
    //运行时内存，
    MEM_Storage         execute_storage;
    //全局变量的头结点，通过它可以构建链表，从而找到其他的全局变量
    Variable            *variable;
    //记录编写的函数，作为头结点，可以找到其他函数
    FunctionDefinition  *function_list;
    //保存具体的语句
    StatementList       *statement_list;

    int                 current_line_number;
} DM_Interpreter;

typedef struct Variable_tag {
    //存储全局变量的结点，可以构成链表
    //变量名
    char        *name;
    //变量值
    DM_Value   value;
    //下一个，用于便利列表
    struct Variable_tag *next;

} Variable;

typedef enum {
    //用户声明的函数
    DOMI_FUNCTION_DEFINITION = 1,
    //自带函数
    NATIVE_FUNCTION_DEFINITION

} FunctionDefinitionType;

typedef struct FunctionDefinition_tag {
    //记录函数的结点，可以连成链表
    //函数名
    char                *name;
    //函数类型
    FunctionDefinitionType      type;

    union {
        //对应自定义函数
        struct {
            //参数的定义
            ParameterList       *parameter;
            //函数主体
            Block               *block;

        } domi_f;
        //对应自带函数
        struct {

            DM_NativeFunctionProc      *proc;

        } native_f;

    } u;
    //下一个
    struct FunctionDefinition_tag       *next;

} FunctionDefinition;

typedef struct ParameterList_tag {
    //函数参数，组合成的链表
    //变量名
    char        *name;
    //链表所用的指针
    struct ParameterList_tag *next;

} ParameterList;

typedef struct {
    //函数主体
    //语句的链表，指向头结点
    //解析器会从头开始执行
    StatementList       *statement_list;

} Block;

typedef struct Statement_tag Statement;

typedef struct StatementList_tag {
    //语句的链表结构
    Statement   *statement;
    //下一个
    struct StatementList_tag    *next;

} StatementList;
/*
statement(语句)的类型的枚举
*/
typedef enum {
    //表达式声明
    EXPRESSION_STATEMENT = 1,
    //全局变量声明的语句
    GLOBAL_STATEMENT,
    //if语句
    IF_STATEMENT,
    //while语句
    WHILE_STATEMENT,
    //for语句
    FOR_STATEMENT,
    //return语句
    RETURN_STATEMENT,
    //break
    BREAK_STATEMENT,
    //continue
    CONTINUE_STATEMENT,
    //++
    STATEMENT_TYPE_COUNT_PLUS_1

} StatementType;

/*
语句
*/

struct Statement_tag {
    //类型
    StatementType       type;

    int                 line_number;

    union {
        //表达式语句
        Expression      *expression_s;
        //global语句
        GlobalStatement global_s;
        //if语句
        IfStatement     if_s;
        //while语句
        WhileStatement  while_s;
        //for语句
        ForStatement    for_s;
        //return语句
        ReturnStatement return_s;

    } u;

};
/*
while语句
*/

typedef struct {
    //条件表达式
    Expression  *condition;
    //可执行块
    Block       *block;

} WhileStatement;
/*
if语句
*/
typedef struct {
    //条件表达式
    Expression  *condition;
    //then里的可执行块
    Block       *then_block;
    //
    Elsif       *elsif_list;
    //else里的可执行块
    Block       *else_block;

} IfStatement;
/*
Elsif链表结构
*/
typedef struct Elsif_tag {
    //elseif里的条件表达式
    Expression  *condition;
    //可执行块
    Block       *block;
    //下一个
    struct Elsif_tag    *next;

} Elsif;

/*
for语句
*/
typedef struct {
    //初始化语句
    Expression  *init;
    //判断语句
    Expression  *condition;
    //后续语句，比如i++
    Expression  *post;
    //代码块
    Block       *block;

} ForStatement;
/*
return语句
*/
typedef struct {
    //retrun后面跟着的表达式
    Expression *return_value;

} ReturnStatement;
/*
global语句
*/
typedef struct {
    //被声明的变量的列表
    IdentifierList      *identifier_list;

} GlobalStatement;

/*
变量
*/
typedef struct IdentifierList_tag {
    //变量的名字
    char        *name;
    //链表中的下一个
    struct IdentifierList_tag   *next;

} IdentifierList;

/*
  expression的类型枚举
*/
typedef enum {
    //boolean
    BOOLEAN_EXPRESSION = 1,
    //int
    INT_EXPRESSION,
    //double
    DOUBLE_EXPRESSION,
    //string
    STRING_EXPRESSION,
    //变量
    IDENTIFIER_EXPRESSION,
    //赋值表达式
    ASSIGN_EXPRESSION,
    //从+到||，这些表达式的值都使用BinaryExpression类型存储
    //+
    ADD_EXPRESSION,
    //-
    SUB_EXPRESSION,
    //*
    MUL_EXPRESSION,
    //  /
    DIV_EXPRESSION,
    //求余表达式
    MOD_EXPRESSION,
    // ==
    EQ_EXPRESSION,
    // ！=
    NE_EXPRESSION,
    // >
    GT_EXPRESSION,
    // >=
    GE_EXPRESSION,
    // <
    LT_EXPRESSION,
    // <=
    LE_EXPRESSION,
    // &&
    LOGICAL_AND_EXPRESSION,
    // ||
    LOGICAL_OR_EXPRESSION,
    //取负数
    MINUS_EXPRESSION,
    //函数调用表达式
    FUNCTION_CALL_EXPRESSION,
    // null
    NULL_EXPRESSION,

    EXPRESSION_TYPE_COUNT_PLUS_1

} ExpressionType;
/*
把表达式归类，包括
1.数学表达式
2.比较表达式
3.逻辑运算表达式
*/
#define dkc_is_math_operator(operator) \
  ((operator) == ADD_EXPRESSION || (operator) == SUB_EXPRESSION\
   || (operator) == MUL_EXPRESSION || (operator) == DIV_EXPRESSION\
   || (operator) == MOD_EXPRESSION)

#define dkc_is_compare_operator(operator) \
  ((operator) == EQ_EXPRESSION || (operator) == NE_EXPRESSION\
   || (operator) == GT_EXPRESSION || (operator) == GE_EXPRESSION\
   || (operator) == LT_EXPRESSION || (operator) == LE_EXPRESSION)

#define dkc_is_logical_operator(operator) \
  ((operator) == LOGICAL_AND_EXPRESSION || (operator) == LOGICAL_OR_EXPRESSION)

/*
expression的定义，表达式
*/
struct Expression_tag {
    //expression的类型
    ExpressionType type;
    //对应的行数
    int line_number;
    //对应类型的存储值的集合
    union {
        //boolean
        DM_Boolean             boolean_value;
        //int
        int                     int_value;
        //double
        double                  double_value;
        //string
        char                    *string_value;
        //变量
        char                    *identifier;
        //赋值表达式
        AssignExpression        assign_expression;
        //存储二元运算符号滴
        BinaryExpression        binary_expression;
        //负号
        Expression              *minus_expression;
        //函数调用表达式
        FunctionCallExpression  function_call_expression;

    } u;

};
/*
存储二元运算符的数据结构
*/
typedef struct {

    Expression  *left;

    Expression  *right;

} BinaryExpression;
/*
赋值运算符
*/
typedef struct {
    //变量
    char        *variable;
    //=右边的表达式
    Expression  *operand;

} AssignExpression;
/*
函数调用的表达式
*/
typedef struct {
    //函数名称
    char                *identifier;
    //参数链表
    ArgumentList        *argument;

} FunctionCallExpression;
/*
函数调用时的参数链表
*/
typedef struct ArgumentList_tag {
    //（在调用函数时，参数可以是表达式！）参数
    Expression *expression;
    //下一个
    struct ArgumentList_tag *next;

} ArgumentList;

/*
变量
*/
/*
全局变量
*/
typedef struct GlobalVariableRef_tag {
    Variable    *variable;
    struct GlobalVariableRef_tag *next;
} GlobalVariableRef;

/*
本地变量，也叫局部变量
*/
typedef struct {
    Variable    *variable;
    GlobalVariableRef   *global_variable;
} LocalEnvironment;

/*
String类型的定义
*/
struct DM_String_tag{
  int ref_count;
  char *string;
  DM_Boolean is_literal;
};

/*
string池
*/
typedef struct {
    CRB_String  *strings;
} StringPool;



#endif //#ifndef DOMI_H
