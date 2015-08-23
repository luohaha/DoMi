
#include <stdio.h>
#include <string.h>
#include "MEM.h"
#include "DBG.h"
#include "domi.h"

/*
定义只能在内部使用的解释器
*/
static DM_Interpreter *st_current_interpreter;

/*
获取当前的解释器
*/
DM_Interpreter *dm_get_current_interpreter(void)
{
    return st_current_interpreter;
}

/*
设置当前的解释器
*/
void dm_set_current_interpreter(DM_Interpreter *inter)
{
    st_current_interpreter = inter;
}

/* BUGBUG
DM_NativeFunctionProc *
dm_search_native_function(DM_Interpreter *inter, char *name)
{
    NativeFunction *pos;

    for (pos = inter->native_function; pos; pos = pos->next) {
        if (!strcmp(pos->name, name))
            break;
    }
    if (pos) {
        return pos->proc;
    } else {
        return NULL;
    }
}
*/
/*
根据函数的名字查找到函数定义的数据结构(FunctionDefinition)
*/
FunctionDefinition *dm_search_function(char *name)
{
    FunctionDefinition *pos;
    DM_Interpreter *inter;

    inter = dm_get_current_interpreter();
    for (pos = inter->function_list; pos; pos = pos->next) {
      if (!strcmp(pos->name, name))/*比较函数名,相等时break*/
            break;
    }
    return pos;
}

void *dm_malloc(size_t size)
{
    void *p;
    DM_Interpreter *inter;

    inter = dm_get_current_interpreter();
    p = MEM_storage_malloc(inter->interpreter_storage, size);

    return p;
}

void *
dm_execute_malloc(DM_Interpreter *inter, size_t size)
{
    void *p;

    p = MEM_storage_malloc(inter->execute_storage, size);

    return p;
}

/*
在本地环境中查找局部变量
*/
Variable *dm_search_local_variable(LocalEnvironment *env, char *identifier)
{
    Variable    *pos;

    if (env == NULL)
        return NULL;
    for (pos = env->variable; pos; pos = pos->next) {
        if (!strcmp(pos->name, identifier))
            break;
    }
    if (pos == NULL) {
        return NULL;
    } else {
        return pos;
    }
}

/*
去解释器中找全局变量
*/
Variable *dm_search_global_variable(DM_Interpreter *inter, char *identifier)
{
    Variable    *pos;

    for (pos = inter->variable; pos; pos = pos->next) {
        if (!strcmp(pos->name, identifier))
            return pos;
    }

    return NULL;
}
/*
添加局部变量
*/
void dm_add_local_variable(LocalEnvironment *env,
                       char *identifier, DM_Value *value)
{
    Variable    *new_variable;

    new_variable = MEM_malloc(sizeof(Variable));
    new_variable->name = identifier;
    new_variable->value = *value;
    /*将新的局部变量插入局部变量链表的头部*/
    new_variable->next = env->variable;
    env->variable = new_variable;
}

/*
将变量插到解释器全局变量链表的头部
*/
void DM_add_global_variable(DM_Interpreter *inter, char *identifier,
                        DM_Value *value)
{
    Variable    *new_variable;

    new_variable = dm_execute_malloc(inter, sizeof(Variable));
    new_variable->name = dm_execute_malloc(inter, strlen(identifier) + 1);
    strcpy(new_variable->name, identifier);
    new_variable->next = inter->variable;
    inter->variable = new_variable;
    new_variable->value = *value;
}

/*
  根据expression的类型来返回字符
*/
char *dm_get_operator_string(ExpressionType type)
{
    char        *str;

    switch (type) {
    case BOOLEAN_EXPRESSION:    /* FALLTHRU */
    case INT_EXPRESSION:        /* FALLTHRU */
    case DOUBLE_EXPRESSION:     /* FALLTHRU */
    case STRING_EXPRESSION:     /* FALLTHRU */
    case IDENTIFIER_EXPRESSION:
        DBG_panic(("bad expression type..%d\n", type));
        break;
    case ASSIGN_EXPRESSION:
        str = "=";
        break;
    case ADD_EXPRESSION:
        str = "+";
        break;
    case SUB_EXPRESSION:
        str = "-";
        break;
    case MUL_EXPRESSION:
        str = "*";
        break;
    case DIV_EXPRESSION:
        str = "/";
        break;
    case MOD_EXPRESSION:
        str = "%";
        break;
    case LOGICAL_AND_EXPRESSION:
        str = "&&";
        break;
    case LOGICAL_OR_EXPRESSION:
        str = "||";
        break;
    case EQ_EXPRESSION:
        str = "==";
        break;
    case NE_EXPRESSION:
        str = "!=";
        break;
    case GT_EXPRESSION:
        str = "<";
        break;
    case GE_EXPRESSION:
        str = "<=";
        break;
    case LT_EXPRESSION:
        str = ">";
        break;
    case LE_EXPRESSION:
        str = ">=";
        break;
    case MINUS_EXPRESSION:
        str = "-";
        break;
    case FUNCTION_CALL_EXPRESSION:  /* FALLTHRU */
    case NULL_EXPRESSION:  /* FALLTHRU */
    case EXPRESSION_TYPE_COUNT_PLUS_1:
    default:
        DBG_panic(("bad expression type..%d\n", type));
    }

    return str;
}
