#ifndef DOMI_H
#define DOMI_H

struct DM_Interpreter_tag {
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
    //编译时当前的行号，出错时可以提示行号
    int                 current_line_number;
};

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


#endif //#ifndef DOMI_H
