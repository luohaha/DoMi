#ifndef DOMI_H
#define DOMI_H

#include<stdio.h>
#include "DM_dev.h"
#include "DM.h"
#include "MEM.h"

/*
  基本常量定义
*/
#define smaller(a, b) ((a) < (b) ? (a) : (b))
#define larger(a, b) ((a) > (b) ? (a) : (b))

#define MESSAGE_ARGUMENT_MAX    (256)
#define LINE_BUF_SIZE           (1024)
/*
 *编译时错误的错误类型
 * */
typedef enum {
  /*解析错误*/
    PARSE_ERR = 1,
    
    CHARACTER_INVALID_ERR,
    /*重复定义函数*/
    FUNCTION_MULTIPLE_DEFINE_ERR,
    
    COMPILE_ERROR_COUNT_PLUS_1


} CompileError;


/*
 *运行时错误的错误类型
 * */
typedef enum {
  /*变量未发现*/
    VARIABLE_NOT_FOUND_ERR = 1,
    /*函数未发现*/
    FUNCTION_NOT_FOUND_ERR,
    /*函数调用时，传入的参数过多*/
    ARGUMENT_TOO_MANY_ERR,
    /*函数调用时，传入的参数过少*/
    ARGUMENT_TOO_FEW_ERR,
    /*不是boolean*/
    NOT_BOOLEAN_TYPE_ERR,
    /*减法错误*/
    MINUS_OPERAND_TYPE_ERR,
    
    BAD_OPERAND_TYPE_ERR,

    NOT_BOOLEAN_OPERATOR_ERR,
    /*文件打开*/
    FOPEN_ARGUMENT_TYPE_ERR,

    FCLOSE_ARGUMENT_TYPE_ERR,

    FGETS_ARGUMENT_TYPE_ERR,

    FPUTS_ARGUMENT_TYPE_ERR,

    NOT_NULL_OPERATOR_ERR,
    /*除以0的错误*/
    DIVISION_BY_ZERO_ERR,
    /*在声明为全局变量的时候，变量未发现*/
    GLOBAL_VARIABLE_NOT_FOUND_ERR,
    
    GLOBAL_STATEMENT_IN_TOPLEVEL_ERR,

    BAD_OPERATOR_FOR_STRING_ERR,

    RUNTIME_ERROR_COUNT_PLUS_1


} RuntimeError;


/*
 *用来标记错误信息中可变部分的类型
 * */
typedef enum {
    
    INT_MESSAGE_ARGUMENT = 1,

    DOUBLE_MESSAGE_ARGUMENT,

    STRING_MESSAGE_ARGUMENT,

    CHARACTER_MESSAGE_ARGUMENT,

    POINTER_MESSAGE_ARGUMENT,

    MESSAGE_ARGUMENT_END


} MessageArgumentType;



struct MessageFormat_tag {

    char *format;

};

struct DM_Interpreter_tag {
  /*在解释器生成时分配内存，解释器不用时回收*/
    MEM_Storage         interpreter_storage;
  /*运行时内存，*/
    MEM_Storage         execute_storage;
  /*全局变量的头结点，通过它可以构建链表，从而找到其他的全局变量*/
    Variable            *variable;
  /*记录编写的函数，作为头结点，可以找到其他函数*/
    FunctionDefinition  *function_list;
  /*保存具体的语句*/
    StatementList       *statement_list;
  /*使用动态作用域*/
  DM_LocalEnvironment *top_env;

    int                 current_line_number;
};

struct Variable_tag {
  /*存储全局变量的结点，可以构成链表*/
  /*变量名*/
    char        *name;
  /*变量值*/
    DM_Value   value;
  /*下一个，用于便利列表*/
    struct Variable_tag *next;

};

typedef enum {
  /*用户声明的函数*/
    DOMI_FUNCTION_DEFINITION = 1,
    /*自带函数*/
    NATIVE_FUNCTION_DEFINITION

} FunctionDefinitionType;

struct FunctionDefinition_tag {
  /*记录函数的结点，可以连成链表*/
  /*函数名*/
    char                *name;
  /*函数类型*/
    FunctionDefinitionType      type;

    union {
      /*对应自定义函数*/
        struct {
          /*参数的定义*/
            ParameterList       *parameter;
          /*函数主体*/
            Block               *block;

        } domi_f;
      /*对应自带函数*/
        struct {

            DM_NativeFunctionProc      *proc;

        } native_f;

    } u;
  /*下一个*/
    struct FunctionDefinition_tag       *next;

};

struct ParameterList_tag {
  /*函数参数，组合成的链表
    变量名*/
    char        *name;
  /*链表所用的指针*/
    struct ParameterList_tag *next;

};

struct Block_tag {
  /*函数主体
    语句的链表，指向头结点
    解析器会从头开始执行*/
    StatementList       *statement_list;

};

struct StatementList_tag {
  /*语句的链表结构*/
    Statement   *statement;
  /*下一个*/
    struct StatementList_tag    *next;

};
/*
statement(语句)的类型的枚举
*/
typedef enum {
  /*表达式声明*/
    EXPRESSION_STATEMENT = 1,
    /*全局变量声明的语句*/
    GLOBAL_STATEMENT,
    /*if语句*/
    IF_STATEMENT,
    /*while语句*/
    WHILE_STATEMENT,
    /*for语句*/
    FOR_STATEMENT,
    /*return语句*/
    RETURN_STATEMENT,
    /*break*/
    BREAK_STATEMENT,
    /*continue*/
    CONTINUE_STATEMENT,
    /*++*/
    STATEMENT_TYPE_COUNT_PLUS_1

} StatementType;

/*
语句
*/
/*
while语句
*/

struct WhileStatement_tag{
  /*条件表达式*/
    Expression  *condition;
  /*可执行块*/
    Block       *block;

};
/*
if语句
*/
struct IfStatement_tag {
  /*条件表达式*/
    Expression  *condition;
  /*then里的可执行块*/
    Block       *then_block;
    
    Elsif       *elsif_list;
  /*else里的可执行块*/
    Block       *else_block;

};
/*
Elsif链表结构
*/
struct Elsif_tag {
  /*elseif里的条件表达式*/
    Expression  *condition;
  /*可执行块*/
    Block       *block;
  /*下一个*/
    struct Elsif_tag    *next;

};

/*
for语句
*/
struct ForStatement_tag {
  /*初始化语句*/
    Expression  *init;
  /*判断语句*/
    Expression  *condition;
  /*后续语句，比如i++*/
    Expression  *post;
  /*代码块*/
    Block       *block;

};
/*
return语句
*/
struct ReturnStatement_tag {
  /*retrun后面跟着的表达式*/
    Expression *return_value;

};
/*
global语句
*/
struct GlobalStatement_tag {
  /*被声明的变量的列表*/
    IdentifierList      *identifier_list;

};

struct Statement_tag {
  /*类型*/
    StatementType       type;

    int                 line_number;

    union {
      /*表达式语句*/
        Expression      *expression_s;
      /*global语句*/
        GlobalStatement global_s;
      /*if语句*/
        IfStatement     if_s;
      /*while语句*/
        WhileStatement  while_s;
      /*for语句*/
        ForStatement    for_s;
      /*return语句*/
        ReturnStatement return_s;

    } u;

};

/*
变量
*/
struct IdentifierList_tag {
  /*变量的名字*/
    char        *name;
  /*链表中的下一个*/
    struct IdentifierList_tag   *next;

};

/*
  expression的类型枚举
*/
typedef enum {
  /*boolean*/
    BOOLEAN_EXPRESSION = 1,
    /*int*/
    INT_EXPRESSION,
    /*double*/
    DOUBLE_EXPRESSION,
    /*string*/
    STRING_EXPRESSION,
    /*变量*/
    IDENTIFIER_EXPRESSION,
    /*赋值表达式*/
    ASSIGN_EXPRESSION,
    /*从+到||，这些表达式的值都使用BinaryExpression类型存储*/
    /*+*/
    ADD_EXPRESSION,
    /*-*/
    SUB_EXPRESSION,
    /***/
    MUL_EXPRESSION,
    /*  / */
    DIV_EXPRESSION,
    /*求余表达式*/
    MOD_EXPRESSION,
    /* ==*/
    EQ_EXPRESSION,
    /* ！=*/
    NE_EXPRESSION,
    /* >*/
    GT_EXPRESSION,
    /* >=*/
    GE_EXPRESSION,
    /* <*/
    LT_EXPRESSION,
    /* <=*/
    LE_EXPRESSION,
    /* &&*/
    LOGICAL_AND_EXPRESSION,
    /* ||*/
    LOGICAL_OR_EXPRESSION,
    /*取负数*/
    MINUS_EXPRESSION,
    /*函数调用表达式*/
    FUNCTION_CALL_EXPRESSION,
    /* null*/
    NULL_EXPRESSION,

    EXPRESSION_TYPE_COUNT_PLUS_1

} ExpressionType;

typedef enum {
    NORMAL_STATEMENT_RESULT = 1,
    RETURN_STATEMENT_RESULT,
    BREAK_STATEMENT_RESULT,
    CONTINUE_STATEMENT_RESULT,
    STATEMENT_RESULT_TYPE_COUNT_PLUS_1
} StatementResultType;

typedef struct StatementResult_tag{
    StatementResultType type;
    union {
        DM_Value       return_value;
    } u;
};
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

/*
  数组元素
*/
struct IndexExpression_tag {
  /*指向表示数组的表达式*/
  Expression *array;
  /*数组的下标*/
  Expression *index;
};

/*
 自增与自减
*/
struct IncrementOrDecrement_tag {
  Expression *operand;
};

/*
  调用函数
*/
struct MethodCallExpression_tag {
  Expression *expression;
  char *identifier;
  ArgumentList *argument;
};

/*
赋值运算符
*/
struct AssignExpression_tag {
  /*变量*/
    char        *variable;
  /*=右边的表达式*/
    Expression  *operand;

};
/*
函数调用的表达式
*/
struct FunctionCallExpression_tag {
  /*函数名称*/
    char                *identifier;
  /*参数链表*/
    ArgumentList        *argument;

};
/*
存储二元运算符的数据结构
*/
struct BinaryExpression_tag {

    Expression  *left;

    Expression  *right;

};


struct Expression_tag {
  /*expression的类型*/
    ExpressionType type;
  /*对应的行数*/
    int line_number;
  /*对应类型的存储值的集合*/
    union {
      /*boolean*/
        DM_Boolean             boolean_value;
      /*int*/
        int                     int_value;
      /*double*/
        double                  double_value;
      /*string*/
        char                    *string_value;
      /*变量*/
        char                    *identifier;
      /*赋值表达式*/
        AssignExpression        assign_expression;
      /*存储二元运算符号滴*/
        BinaryExpression        binary_expression;
      /*负号*/
        Expression              *minus_expression;
      /*函数调用表达式*/
        FunctionCallExpression  function_call_expression;

    } u;

};

/*
函数调用时的参数链表
*/
struct ArgumentList_tag {
  /*（在调用函数时，参数可以是表达式！）参数*/
    Expression *expression;
  /*下一个*/
    struct ArgumentList_tag *next;

};


/*
变量
*/
/*
全局变量
*/
struct GlobalVariableRef_tag {
    Variable    *variable;
    struct GlobalVariableRef_tag *next;
};

/*
本地变量，也叫局部变量
*/
struct LocalEnvironment_tag {
   Variable    *variable;
   GlobalVariableRef   *global_variable;
   struct LocalEnvironment_tag *next;
};

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
struct StringPool_tag {
    DM_String  *strings;
};


/* create.c */
void dm_function_define(char *identifier, ParameterList *parameter_list,
                         Block *block);
ParameterList *dm_create_parameter(char *identifier);
ParameterList *dm_chain_parameter(ParameterList *list,
                                   char *identifier);
ArgumentList *dm_create_argument_list(Expression *expression);
ArgumentList *dm_chain_argument_list(ArgumentList *list, Expression *expr);
StatementList *dm_create_statement_list(Statement *statement);
StatementList *dm_chain_statement_list(StatementList *list,
                                        Statement *statement);
Expression *dm_alloc_expression(ExpressionType type);
Expression *dm_create_assign_expression(char *variable,
                                             Expression *operand);
Expression *dm_create_binary_expression(ExpressionType operator,
                                         Expression *left,
                                         Expression *right);
Expression *dm_create_minus_expression(Expression *operand);
Expression *dm_create_identifier_expression(char *identifier);
Expression *dm_create_function_call_expression(char *func_name,
                                                ArgumentList *argument);
Expression *dm_create_boolean_expression(DM_Boolean value);
Expression *dm_create_null_expression(void);
Statement *dm_create_global_statement(IdentifierList *identifier_list);
IdentifierList *dm_create_global_identifier(char *identifier);
IdentifierList *dm_chain_identifier(IdentifierList *list, char *identifier);
Statement *dm_create_if_statement(Expression *condition,
                                    Block *then_block, Elsif *elsif_list,
                                    Block *else_block);
Elsif *dm_chain_elsif_list(Elsif *list, Elsif *add);
Elsif *dm_create_elsif(Expression *expr, Block *block);
Statement *dm_create_while_statement(Expression *condition, Block *block);
Statement *dm_create_for_statement(Expression *init, Expression *cond,
                                    Expression *post, Block *block);
Block *dm_create_block(StatementList *statement_list);
Statement *dm_create_expression_statement(Expression *expression);
Statement *dm_create_return_statement(Expression *expression);
Statement *dm_create_break_statement(void);
Statement *dm_create_continue_statement(void);

/* string.c */
char *dm_create_identifier(char *str);
void dm_open_string_literal(void);
void dm_add_string_literal(char letter);
void dm_reset_string_literal_buffer(void);
char *dm_close_string_literal(void);

/* execute.c */
StatementResult
dm_execute_statement_list(DM_Interpreter *inter,
                           LocalEnvironment *env, StatementList *list);

/* eval.c */
DM_Value dm_eval_binary_expression(DM_Interpreter *inter,
                                 LocalEnvironment *env,
                                 ExpressionType operator,
                                 Expression *left, Expression *right);
DM_Value dm_eval_minus_expression(DM_Interpreter *inter,
                                LocalEnvironment *env, Expression *operand);
DM_Value dm_eval_expression(DM_Interpreter *inter,
                          LocalEnvironment *env, Expression *expr);

/* string_pool.c */
DM_String *dm_literal_to_dm_string(DM_Interpreter *inter, char *str);
void dm_refer_string(DM_String *str);
void dm_release_string(DM_String *str);
DM_String *dm_search_dm_string(DM_Interpreter *inter, char *str);
DM_String *dm_create_crowbar_string(DM_Interpreter *inter, char *str);

/* util.c */
DM_Interpreter *dm_get_current_interpreter(void);
void dm_set_current_interpreter(DM_Interpreter *inter);
void *dm_malloc(size_t size);
void *dm_execute_malloc(DM_Interpreter *inter, size_t size);
Variable *dm_search_local_variable(LocalEnvironment *env,
                                    char *identifier);
Variable *
dm_search_global_variable(DM_Interpreter *inter, char *identifier);
void dm_add_local_variable(LocalEnvironment *env,
                            char *identifier, DM_Value *value);
DM_NativeFunctionProc *
dm_search_native_function(DM_Interpreter *inter, char *name);
FunctionDefinition *dm_search_function(char *name);
char *dm_get_operator_string(ExpressionType type);

/* error.c */
void dm_compile_error(CompileError id, ...);
void dm_runtime_error(int line_number, RuntimeError id, ...);

/* native.c */
DM_Value dm_nv_print_proc(DM_Interpreter *interpreter,
                            int arg_count, DM_Value *args);
DM_Value dm_nv_fopen_proc(DM_Interpreter *interpreter,
                            int arg_count, DM_Value *args);
DM_Value dm_nv_fclose_proc(DM_Interpreter *interpreter,
                             int arg_count, DM_Value *args);
DM_Value dm_nv_fgets_proc(DM_Interpreter *interpreter,
                            int arg_count, DM_Value *args);
DM_Value dm_nv_fputs_proc(DM_Interpreter *interpreter,
                            int arg_count, DM_Value *args);
void dm_add_std_fp(DM_Interpreter *inter);


#endif /*#ifndef DOMI_H*/
