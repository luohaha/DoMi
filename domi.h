#ifndef DOMI_H
#define DOMI_H

#include<stdio.h>
#include "DM_dev.h"
#include "DM.h"
#include "MEM.h"

/*
  ������������
*/
#define smaller(a, b) ((a) < (b) ? (a) : (b))
#define larger(a, b) ((a) > (b) ? (a) : (b))

#define MESSAGE_ARGUMENT_MAX    (256)
#define LINE_BUF_SIZE           (1024)
/*
 *����ʱ����Ĵ�������
 * */
typedef enum {
  /*��������*/
    PARSE_ERR = 1,
    
    CHARACTER_INVALID_ERR,
    /*�ظ����庯��*/
    FUNCTION_MULTIPLE_DEFINE_ERR,
    
    COMPILE_ERROR_COUNT_PLUS_1


} CompileError;


/*
 *����ʱ����Ĵ�������
 * */
typedef enum {
  /*����δ����*/
    VARIABLE_NOT_FOUND_ERR = 1,
    /*����δ����*/
    FUNCTION_NOT_FOUND_ERR,
    /*��������ʱ������Ĳ�������*/
    ARGUMENT_TOO_MANY_ERR,
    /*��������ʱ������Ĳ�������*/
    ARGUMENT_TOO_FEW_ERR,
    /*����boolean*/
    NOT_BOOLEAN_TYPE_ERR,
    /*��������*/
    MINUS_OPERAND_TYPE_ERR,
    
    BAD_OPERAND_TYPE_ERR,

    NOT_BOOLEAN_OPERATOR_ERR,
    /*�ļ���*/
    FOPEN_ARGUMENT_TYPE_ERR,

    FCLOSE_ARGUMENT_TYPE_ERR,

    FGETS_ARGUMENT_TYPE_ERR,

    FPUTS_ARGUMENT_TYPE_ERR,

    NOT_NULL_OPERATOR_ERR,
    /*����0�Ĵ���*/
    DIVISION_BY_ZERO_ERR,
    /*������Ϊȫ�ֱ�����ʱ�򣬱���δ����*/
    GLOBAL_VARIABLE_NOT_FOUND_ERR,
    
    GLOBAL_STATEMENT_IN_TOPLEVEL_ERR,

    BAD_OPERATOR_FOR_STRING_ERR,

    RUNTIME_ERROR_COUNT_PLUS_1


} RuntimeError;


/*
 *������Ǵ�����Ϣ�пɱ䲿�ֵ�����
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
  /*�ڽ���������ʱ�����ڴ棬����������ʱ����*/
    MEM_Storage         interpreter_storage;
  /*����ʱ�ڴ棬*/
    MEM_Storage         execute_storage;
  /*ȫ�ֱ�����ͷ��㣬ͨ�������Թ��������Ӷ��ҵ�������ȫ�ֱ���*/
    Variable            *variable;
  /*��¼��д�ĺ�������Ϊͷ��㣬�����ҵ���������*/
    FunctionDefinition  *function_list;
  /*�����������*/
    StatementList       *statement_list;
  /*ʹ�ö�̬������*/
  DM_LocalEnvironment *top_env;

    int                 current_line_number;
};

struct Variable_tag {
  /*�洢ȫ�ֱ����Ľ�㣬���Թ�������*/
  /*������*/
    char        *name;
  /*����ֵ*/
    DM_Value   value;
  /*��һ�������ڱ����б�*/
    struct Variable_tag *next;

};

typedef enum {
  /*�û������ĺ���*/
    DOMI_FUNCTION_DEFINITION = 1,
    /*�Դ�����*/
    NATIVE_FUNCTION_DEFINITION

} FunctionDefinitionType;

struct FunctionDefinition_tag {
  /*��¼�����Ľ�㣬������������*/
  /*������*/
    char                *name;
  /*��������*/
    FunctionDefinitionType      type;

    union {
      /*��Ӧ�Զ��庯��*/
        struct {
          /*�����Ķ���*/
            ParameterList       *parameter;
          /*��������*/
            Block               *block;

        } domi_f;
      /*��Ӧ�Դ�����*/
        struct {

            DM_NativeFunctionProc      *proc;

        } native_f;

    } u;
  /*��һ��*/
    struct FunctionDefinition_tag       *next;

};

struct ParameterList_tag {
  /*������������ϳɵ�����
    ������*/
    char        *name;
  /*�������õ�ָ��*/
    struct ParameterList_tag *next;

};

struct Block_tag {
  /*��������
    ��������ָ��ͷ���
    ���������ͷ��ʼִ��*/
    StatementList       *statement_list;

};

struct StatementList_tag {
  /*��������ṹ*/
    Statement   *statement;
  /*��һ��*/
    struct StatementList_tag    *next;

};
/*
statement(���)�����͵�ö��
*/
typedef enum {
  /*���ʽ����*/
    EXPRESSION_STATEMENT = 1,
    /*ȫ�ֱ������������*/
    GLOBAL_STATEMENT,
    /*if���*/
    IF_STATEMENT,
    /*while���*/
    WHILE_STATEMENT,
    /*for���*/
    FOR_STATEMENT,
    /*return���*/
    RETURN_STATEMENT,
    /*break*/
    BREAK_STATEMENT,
    /*continue*/
    CONTINUE_STATEMENT,
    /*++*/
    STATEMENT_TYPE_COUNT_PLUS_1

} StatementType;

/*
���
*/
/*
while���
*/

struct WhileStatement_tag{
  /*�������ʽ*/
    Expression  *condition;
  /*��ִ�п�*/
    Block       *block;

};
/*
if���
*/
struct IfStatement_tag {
  /*�������ʽ*/
    Expression  *condition;
  /*then��Ŀ�ִ�п�*/
    Block       *then_block;
    
    Elsif       *elsif_list;
  /*else��Ŀ�ִ�п�*/
    Block       *else_block;

};
/*
Elsif����ṹ
*/
struct Elsif_tag {
  /*elseif����������ʽ*/
    Expression  *condition;
  /*��ִ�п�*/
    Block       *block;
  /*��һ��*/
    struct Elsif_tag    *next;

};

/*
for���
*/
struct ForStatement_tag {
  /*��ʼ�����*/
    Expression  *init;
  /*�ж����*/
    Expression  *condition;
  /*������䣬����i++*/
    Expression  *post;
  /*�����*/
    Block       *block;

};
/*
return���
*/
struct ReturnStatement_tag {
  /*retrun������ŵı��ʽ*/
    Expression *return_value;

};
/*
global���
*/
struct GlobalStatement_tag {
  /*�������ı������б�*/
    IdentifierList      *identifier_list;

};

struct Statement_tag {
  /*����*/
    StatementType       type;

    int                 line_number;

    union {
      /*���ʽ���*/
        Expression      *expression_s;
      /*global���*/
        GlobalStatement global_s;
      /*if���*/
        IfStatement     if_s;
      /*while���*/
        WhileStatement  while_s;
      /*for���*/
        ForStatement    for_s;
      /*return���*/
        ReturnStatement return_s;

    } u;

};

/*
����
*/
struct IdentifierList_tag {
  /*����������*/
    char        *name;
  /*�����е���һ��*/
    struct IdentifierList_tag   *next;

};

/*
  expression������ö��
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
    /*����*/
    IDENTIFIER_EXPRESSION,
    /*��ֵ���ʽ*/
    ASSIGN_EXPRESSION,
    /*��+��||����Щ���ʽ��ֵ��ʹ��BinaryExpression���ʹ洢*/
    /*+*/
    ADD_EXPRESSION,
    /*-*/
    SUB_EXPRESSION,
    /***/
    MUL_EXPRESSION,
    /*  / */
    DIV_EXPRESSION,
    /*������ʽ*/
    MOD_EXPRESSION,
    /* ==*/
    EQ_EXPRESSION,
    /* ��=*/
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
    /*ȡ����*/
    MINUS_EXPRESSION,
    /*�������ñ��ʽ*/
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
�ѱ��ʽ���࣬����
1.��ѧ���ʽ
2.�Ƚϱ��ʽ
3.�߼�������ʽ
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
expression�Ķ��壬���ʽ
*/

/*
  ����Ԫ��
*/
struct IndexExpression_tag {
  /*ָ���ʾ����ı��ʽ*/
  Expression *array;
  /*������±�*/
  Expression *index;
};

/*
 �������Լ�
*/
struct IncrementOrDecrement_tag {
  Expression *operand;
};

/*
  ���ú���
*/
struct MethodCallExpression_tag {
  Expression *expression;
  char *identifier;
  ArgumentList *argument;
};

/*
��ֵ�����
*/
struct AssignExpression_tag {
  /*����*/
    char        *variable;
  /*=�ұߵı��ʽ*/
    Expression  *operand;

};
/*
�������õı��ʽ
*/
struct FunctionCallExpression_tag {
  /*��������*/
    char                *identifier;
  /*��������*/
    ArgumentList        *argument;

};
/*
�洢��Ԫ����������ݽṹ
*/
struct BinaryExpression_tag {

    Expression  *left;

    Expression  *right;

};


struct Expression_tag {
  /*expression������*/
    ExpressionType type;
  /*��Ӧ������*/
    int line_number;
  /*��Ӧ���͵Ĵ洢ֵ�ļ���*/
    union {
      /*boolean*/
        DM_Boolean             boolean_value;
      /*int*/
        int                     int_value;
      /*double*/
        double                  double_value;
      /*string*/
        char                    *string_value;
      /*����*/
        char                    *identifier;
      /*��ֵ���ʽ*/
        AssignExpression        assign_expression;
      /*�洢��Ԫ������ŵ�*/
        BinaryExpression        binary_expression;
      /*����*/
        Expression              *minus_expression;
      /*�������ñ��ʽ*/
        FunctionCallExpression  function_call_expression;

    } u;

};

/*
��������ʱ�Ĳ�������
*/
struct ArgumentList_tag {
  /*���ڵ��ú���ʱ�����������Ǳ��ʽ��������*/
    Expression *expression;
  /*��һ��*/
    struct ArgumentList_tag *next;

};


/*
����
*/
/*
ȫ�ֱ���
*/
struct GlobalVariableRef_tag {
    Variable    *variable;
    struct GlobalVariableRef_tag *next;
};

/*
���ر�����Ҳ�оֲ�����
*/
struct LocalEnvironment_tag {
   Variable    *variable;
   GlobalVariableRef   *global_variable;
   struct LocalEnvironment_tag *next;
};

/*
String���͵Ķ���
*/
struct DM_String_tag{
  int ref_count;
  char *string;
  DM_Boolean is_literal;
};

/*
string��
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
