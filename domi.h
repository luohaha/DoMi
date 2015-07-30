#ifndef DOMI_H
#define DOMI_H

#include<stdio.h>
#include "DM_DEV.h"
#include "DM.h"

/*
 *����ʱ����Ĵ�������
 * */
typedef enum {
    //��������
    PARSE_ERR = 1,
    //
    CHARACTER_INVALID_ERR,
    //�ظ����庯��
    FUNCTION_MULTIPLE_DEFINE_ERR,
    //
    COMPILE_ERROR_COUNT_PLUS_1


} CompileError;


/*
 *����ʱ����Ĵ�������
 * */
typedef enum {
    //����δ����
    VARIABLE_NOT_FOUND_ERR = 1,
    //����δ����
    FUNCTION_NOT_FOUND_ERR,
    //��������ʱ������Ĳ�������
    ARGUMENT_TOO_MANY_ERR,
    //��������ʱ������Ĳ�������
    ARGUMENT_TOO_FEW_ERR,
    //����boolean
    NOT_BOOLEAN_TYPE_ERR,
    //��������
    MINUS_OPERAND_TYPE_ERR,
    //
    BAD_OPERAND_TYPE_ERR,

    NOT_BOOLEAN_OPERATOR_ERR,
    //�ļ���
    FOPEN_ARGUMENT_TYPE_ERR,

    FCLOSE_ARGUMENT_TYPE_ERR,

    FGETS_ARGUMENT_TYPE_ERR,

    FPUTS_ARGUMENT_TYPE_ERR,

    NOT_NULL_OPERATOR_ERR,
    //����0�Ĵ���
    DIVISION_BY_ZERO_ERR,
    //������Ϊȫ�ֱ�����ʱ�򣬱���δ����
    GLOBAL_VARIABLE_NOT_FOUND_ERR,
    //
    GLOBAL_STATEMENT_IN_TOPLEVEL_ERR,

    BAD_OPERATOR_FOR_STRING_ERR,

    RUNTIME_ERROR_COUNT_PLUS_1


} RuntimeError;


/*
 *������Ǵ�����Ϣ�пɱ䲿�ֵ�����
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
    //�ڽ���������ʱ�����ڴ棬����������ʱ����
    MEM_Storage         interpreter_storage;
    //����ʱ�ڴ棬
    MEM_Storage         execute_storage;
    //ȫ�ֱ�����ͷ��㣬ͨ�������Թ��������Ӷ��ҵ�������ȫ�ֱ���
    Variable            *variable;
    //��¼��д�ĺ�������Ϊͷ��㣬�����ҵ���������
    FunctionDefinition  *function_list;
    //�����������
    StatementList       *statement_list;

    int                 current_line_number;
} DM_Interpreter;

typedef struct Variable_tag {
    //�洢ȫ�ֱ����Ľ�㣬���Թ�������
    //������
    char        *name;
    //����ֵ
    DM_Value   value;
    //��һ�������ڱ����б�
    struct Variable_tag *next;

} Variable;

typedef enum {
    //�û������ĺ���
    DOMI_FUNCTION_DEFINITION = 1,
    //�Դ�����
    NATIVE_FUNCTION_DEFINITION

} FunctionDefinitionType;

typedef struct FunctionDefinition_tag {
    //��¼�����Ľ�㣬������������
    //������
    char                *name;
    //��������
    FunctionDefinitionType      type;

    union {
        //��Ӧ�Զ��庯��
        struct {
            //�����Ķ���
            ParameterList       *parameter;
            //��������
            Block               *block;

        } domi_f;
        //��Ӧ�Դ�����
        struct {

            DM_NativeFunctionProc      *proc;

        } native_f;

    } u;
    //��һ��
    struct FunctionDefinition_tag       *next;

} FunctionDefinition;

typedef struct ParameterList_tag {
    //������������ϳɵ�����
    //������
    char        *name;
    //�������õ�ָ��
    struct ParameterList_tag *next;

} ParameterList;

typedef struct {
    //��������
    //��������ָ��ͷ���
    //���������ͷ��ʼִ��
    StatementList       *statement_list;

} Block;

typedef struct Statement_tag Statement;

typedef struct StatementList_tag {
    //��������ṹ
    Statement   *statement;
    //��һ��
    struct StatementList_tag    *next;

} StatementList;
/*
statement(���)�����͵�ö��
*/
typedef enum {
    //���ʽ����
    EXPRESSION_STATEMENT = 1,
    //ȫ�ֱ������������
    GLOBAL_STATEMENT,
    //if���
    IF_STATEMENT,
    //while���
    WHILE_STATEMENT,
    //for���
    FOR_STATEMENT,
    //return���
    RETURN_STATEMENT,
    //break
    BREAK_STATEMENT,
    //continue
    CONTINUE_STATEMENT,
    //++
    STATEMENT_TYPE_COUNT_PLUS_1

} StatementType;

/*
���
*/

struct Statement_tag {
    //����
    StatementType       type;

    int                 line_number;

    union {
        //���ʽ���
        Expression      *expression_s;
        //global���
        GlobalStatement global_s;
        //if���
        IfStatement     if_s;
        //while���
        WhileStatement  while_s;
        //for���
        ForStatement    for_s;
        //return���
        ReturnStatement return_s;

    } u;

};
/*
while���
*/

typedef struct {
    //�������ʽ
    Expression  *condition;
    //��ִ�п�
    Block       *block;

} WhileStatement;
/*
if���
*/
typedef struct {
    //�������ʽ
    Expression  *condition;
    //then��Ŀ�ִ�п�
    Block       *then_block;
    //
    Elsif       *elsif_list;
    //else��Ŀ�ִ�п�
    Block       *else_block;

} IfStatement;
/*
Elsif����ṹ
*/
typedef struct Elsif_tag {
    //elseif����������ʽ
    Expression  *condition;
    //��ִ�п�
    Block       *block;
    //��һ��
    struct Elsif_tag    *next;

} Elsif;

/*
for���
*/
typedef struct {
    //��ʼ�����
    Expression  *init;
    //�ж����
    Expression  *condition;
    //������䣬����i++
    Expression  *post;
    //�����
    Block       *block;

} ForStatement;
/*
return���
*/
typedef struct {
    //retrun������ŵı��ʽ
    Expression *return_value;

} ReturnStatement;
/*
global���
*/
typedef struct {
    //�������ı������б�
    IdentifierList      *identifier_list;

} GlobalStatement;

/*
����
*/
typedef struct IdentifierList_tag {
    //����������
    char        *name;
    //�����е���һ��
    struct IdentifierList_tag   *next;

} IdentifierList;

/*
  expression������ö��
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
    //����
    IDENTIFIER_EXPRESSION,
    //��ֵ���ʽ
    ASSIGN_EXPRESSION,
    //��+��||����Щ���ʽ��ֵ��ʹ��BinaryExpression���ʹ洢
    //+
    ADD_EXPRESSION,
    //-
    SUB_EXPRESSION,
    //*
    MUL_EXPRESSION,
    //  /
    DIV_EXPRESSION,
    //������ʽ
    MOD_EXPRESSION,
    // ==
    EQ_EXPRESSION,
    // ��=
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
    //ȡ����
    MINUS_EXPRESSION,
    //�������ñ��ʽ
    FUNCTION_CALL_EXPRESSION,
    // null
    NULL_EXPRESSION,

    EXPRESSION_TYPE_COUNT_PLUS_1

} ExpressionType;
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
struct Expression_tag {
    //expression������
    ExpressionType type;
    //��Ӧ������
    int line_number;
    //��Ӧ���͵Ĵ洢ֵ�ļ���
    union {
        //boolean
        DM_Boolean             boolean_value;
        //int
        int                     int_value;
        //double
        double                  double_value;
        //string
        char                    *string_value;
        //����
        char                    *identifier;
        //��ֵ���ʽ
        AssignExpression        assign_expression;
        //�洢��Ԫ������ŵ�
        BinaryExpression        binary_expression;
        //����
        Expression              *minus_expression;
        //�������ñ��ʽ
        FunctionCallExpression  function_call_expression;

    } u;

};
/*
�洢��Ԫ����������ݽṹ
*/
typedef struct {

    Expression  *left;

    Expression  *right;

} BinaryExpression;
/*
��ֵ�����
*/
typedef struct {
    //����
    char        *variable;
    //=�ұߵı��ʽ
    Expression  *operand;

} AssignExpression;
/*
�������õı��ʽ
*/
typedef struct {
    //��������
    char                *identifier;
    //��������
    ArgumentList        *argument;

} FunctionCallExpression;
/*
��������ʱ�Ĳ�������
*/
typedef struct ArgumentList_tag {
    //���ڵ��ú���ʱ�����������Ǳ��ʽ��������
    Expression *expression;
    //��һ��
    struct ArgumentList_tag *next;

} ArgumentList;

/*
����
*/
/*
ȫ�ֱ���
*/
typedef struct GlobalVariableRef_tag {
    Variable    *variable;
    struct GlobalVariableRef_tag *next;
} GlobalVariableRef;

/*
���ر�����Ҳ�оֲ�����
*/
typedef struct {
    Variable    *variable;
    GlobalVariableRef   *global_variable;
} LocalEnvironment;

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
typedef struct {
    CRB_String  *strings;
} StringPool;



#endif //#ifndef DOMI_H
