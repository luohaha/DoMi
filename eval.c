#include <math.h>
#include <string.h>
#include "MEM.h"
#include "DBG.h"
#include "domi.h"

/*
 *将boolean类型，转换为DM——Value
 * */
static DM_Value eval_boolean_expression(CRB_Boolean boolean_value)
{
    DM_Value   v;

    v.type = DM_BOOLEAN_VALUE;
    v.u.boolean_value = boolean_value;

    return v;
}


/*
 *将int转换为DM_VALUE
 * */
static DM_Value eval_int_expression(int int_value)
{
    DM_Value   v;

    v.type = DM_INT_VALUE;
    v.u.int_value = int_value;

    return v;
}

/*
 *将double类型转换为DM_Value
 * */
static DM_Value eval_double_expression(double double_value)
{
    DM_Value   v;

    v.type = DM_DOUBLE_VALUE;
    v.u.double_value = double_value;

    return v;
}

/*
 *将string转化为DM_Value
 * */
static DM_Value eval_string_expression(DM_Interpreter *inter, char *string_value)
{
    DM_Value   v;

    v.type = CRB_STRING_VALUE;
    v.u.string_value = dm_literal_to_dm_string(inter, string_value);

    return v;
}

/*
 *null转化为DM_Value
 * */
static DM_Value eval_null_expression(void)
{
    CRB_Value   v;

    v.type = CRB_NULL_VALUE;

    return v;
}

/*
判断是不是string，string的引用计数加一
*/
static void refer_if_string(DM_Value *v)
{
    if (v->type == DM_STRING_VALUE) {
        dm_refer_string(v->u.string_value);
    }
}

/*
判断是不是string，string的引用计数减一
*/
static void release_if_string(CRB_Value *v)
{
    if (v->type == CRB_STRING_VALUE) {
        crb_release_string(v->u.string_value);
    }
}

/*
从本地环境中查找全局变量,找不到，再去解释器中找
*/
static Variable *search_global_variable_from_env(DM_Interpreter *inter,
                                LocalEnvironment *env, char *name)
{
    GlobalVariableRef *pos;

    if (env == NULL) {
      //但本地环境中找不到时，去解释器中找
        return dm_search_global_variable(inter, name);
    }

    //查找
    for (pos = env->global_variable; pos; pos = pos->next) {
        if (!strcmp(pos->variable->name, name)) {
            return pos->variable;
        }
    }

    return NULL;
}

/*
查找相应的标识符，对应的DM_Value
*/
static DM_Value eval_identifier_expression(CRB_Interpreter *inter,
                           LocalEnvironment *env, Expression *expr)
{
    DM_Value   v;
    Variable    *vp;

    //查找本地的变量
    vp = crb_search_local_variable(env, expr->u.identifier);
    if (vp != NULL) {
      //找到
        v = vp->value;
    } else {
      //如果没有找到，查找全局变量
        vp = search_global_variable_from_env(inter, env, expr->u.identifier);
        if (vp != NULL) {
            v = vp->value;
        } else {
          //如果找不到，报错
            crb_runtime_error(expr->line_number, VARIABLE_NOT_FOUND_ERR,
                              STRING_MESSAGE_ARGUMENT,
                              "name", expr->u.identifier,
                              MESSAGE_ARGUMENT_END);
        }
    }
    //这里需要判断是否是DM_String,如果是,则计数应用加一
    refer_if_string(&v);

    return v;
}

static DM_Value eval_expression(DM_Interpreter *inter, LocalEnvironment *env,
                                 Expression *expr);

/*
赋值运算评估
identifier是左边的变量名
expression是右边的表达式
*/
static DM_Value eval_assign_expression(DM_Interpreter *inter, LocalEnvironment *env,
                       char *identifier, Expression *expression)
{
    DM_Value   v;
    Variable    *left;

    //求出右边表达式的DM_Value
    v = eval_expression(inter, env, expression);

    //查找左边的变量是否在局部变量中
    left = dm_search_local_variable(env, identifier);
    if (left == NULL) {
      //如果不在局部变量
        left = search_global_variable_from_env(inter, env, identifier);
    }
    if (left != NULL) {
      //如果之前已经存在这个变量
      //先把之前的string引用回收
        release_if_string(&left->value);
        left->value = v;
        //然后如果是string，还要增加引用
        refer_if_string(&v);
    } else {
        if (env != NULL) {
          //添加到局部变量
            dm_add_local_variable(env, identifier, &v);
        } else {
          //添加到全局变量
            DM_add_global_variable(inter, identifier, &v);
        }
        refer_if_string(&v);
    }

    return v;
}
/*
 *左右两个都为boolean，只有== != 这两个运算
 * */
static CRB_Boolean eval_binary_boolean(CRB_Interpreter *inter, ExpressionType operator,
                    CRB_Boolean left, CRB_Boolean right, int line_number)
{
    CRB_Boolean result;

    if (operator == EQ_EXPRESSION) {
        result = left == right;
    } else if (operator == NE_EXPRESSION) {
        result = left != right;
    } else {
        //不符合boolean之间的运算,报错
        char *op_str = crb_get_operator_string(operator);
        crb_runtime_error(line_number, NOT_BOOLEAN_OPERATOR_ERR,
                          STRING_MESSAGE_ARGUMENT, "operator", op_str,
                          MESSAGE_ARGUMENT_END);
    }

    return result;
}
/*
 *二元运算求值的函数，其中左右变量都为int
 * */
static void eval_binary_int(DM_Interpreter *inter, ExpressionType operator,
                int left, int right,
                DM_Value *result, int line_number)
{
    if (dkc_is_math_operator(operator)) {
        //如果为运算操作
        result->type = DM_INT_VALUE;
    } else if (dkc_is_compare_operator(operator)) {
        //如果为比较操作
        result->type = DM_BOOLEAN_VALUE;
    } else {
        DBG_panic(("operator..%d\n", operator));
    }

    switch (operator) {
    case BOOLEAN_EXPRESSION:    /* FALLTHRU */
    case INT_EXPRESSION:        /* FALLTHRU */
    case DOUBLE_EXPRESSION:     /* FALLTHRU */
    case STRING_EXPRESSION:     /* FALLTHRU */
    case IDENTIFIER_EXPRESSION: /* FALLTHRU */
    case ASSIGN_EXPRESSION:
        DBG_panic(("bad case...%d", operator));
        break;
    case ADD_EXPRESSION:
        result->u.int_value = left + right;
        break;
    case SUB_EXPRESSION:
        result->u.int_value = left - right;
        break;
    case MUL_EXPRESSION:
        result->u.int_value = left * right;
        break;
    case DIV_EXPRESSION:
        result->u.int_value = left / right;
        break;
    case MOD_EXPRESSION:
        result->u.int_value = left % right;
        break;
    case LOGICAL_AND_EXPRESSION:        /* FALLTHRU */
    case LOGICAL_OR_EXPRESSION:
        DBG_panic(("bad case...%d", operator));
        break;
    case EQ_EXPRESSION:
        result->u.boolean_value = left == right;
        break;
    case NE_EXPRESSION:
        result->u.boolean_value = left != right;
        break;
    case GT_EXPRESSION:
        result->u.boolean_value = left > right;
        break;
    case GE_EXPRESSION:
        result->u.boolean_value = left >= right;
        break;
    case LT_EXPRESSION:
        result->u.boolean_value = left < right;
        break;
    case LE_EXPRESSION:
        result->u.boolean_value = left <= right;
        break;
    case MINUS_EXPRESSION:              /* FALLTHRU */
    case FUNCTION_CALL_EXPRESSION:      /* FALLTHRU */
    case NULL_EXPRESSION:               /* FALLTHRU */
    case EXPRESSION_TYPE_COUNT_PLUS_1:  /* FALLTHRU */
    default:
        DBG_panic(("bad case...%d", operator));
    }
}

static void eval_binary_double(CRB_Interpreter *inter, ExpressionType operator,
                   double left, double right,
                   CRB_Value *result, int line_number)
{
    if (dkc_is_math_operator(operator)) {
        result->type = CRB_DOUBLE_VALUE;
    } else if (dkc_is_compare_operator(operator)) {
        result->type = CRB_BOOLEAN_VALUE;
    } else {
        DBG_panic(("operator..%d\n", operator));
    }

    switch (operator) {
    case BOOLEAN_EXPRESSION:    /* FALLTHRU */
    case INT_EXPRESSION:        /* FALLTHRU */
    case DOUBLE_EXPRESSION:     /* FALLTHRU */
    case STRING_EXPRESSION:     /* FALLTHRU */
    case IDENTIFIER_EXPRESSION: /* FALLTHRU */
    case ASSIGN_EXPRESSION:
        DBG_panic(("bad case...%d", operator));
        break;
    case ADD_EXPRESSION:
        result->u.double_value = left + right;
        break;
    case SUB_EXPRESSION:
        result->u.double_value = left - right;
        break;
    case MUL_EXPRESSION:
        result->u.double_value = left * right;
        break;
    case DIV_EXPRESSION:
        result->u.double_value = left / right;
        break;
    case MOD_EXPRESSION:
        result->u.double_value = fmod(left, right);
        break;
    case LOGICAL_AND_EXPRESSION:        /* FALLTHRU */
    case LOGICAL_OR_EXPRESSION:
        DBG_panic(("bad case...%d", operator));
        break;
    case EQ_EXPRESSION:
        result->u.int_value = left == right;
        break;
    case NE_EXPRESSION:
        result->u.int_value = left != right;
        break;
    case GT_EXPRESSION:
        result->u.int_value = left > right;
        break;
    case GE_EXPRESSION:
        result->u.int_value = left >= right;
        break;
    case LT_EXPRESSION:
        result->u.int_value = left < right;
        break;
    case LE_EXPRESSION:
        result->u.int_value = left <= right;
        break;
    case MINUS_EXPRESSION:              /* FALLTHRU */
    case FUNCTION_CALL_EXPRESSION:      /* FALLTHRU */
    case NULL_EXPRESSION:               /* FALLTHRU */
    case EXPRESSION_TYPE_COUNT_PLUS_1:  /* FALLTHRU */
    default:
        DBG_panic(("bad default...%d", operator));
    }
}
/*
对于不同的字符串之间的比较操作，返回DM_Boolean
*/
static DM_Boolean eval_compare_string(ExpressionType operator,
                    CRB_Value *left, CRB_Value *right, int line_number)
{
    DM_Boolean result;
    int cmp;

    //比较大小
    cmp = strcmp(left->u.string_value->string, right->u.string_value->string);

    if (operator == EQ_EXPRESSION) {
        result = (cmp == 0);
    } else if (operator == NE_EXPRESSION) {
        result = (cmp != 0);
    } else if (operator == GT_EXPRESSION) {
        result = (cmp > 0);
    } else if (operator == GE_EXPRESSION) {
        result = (cmp >= 0);
    } else if (operator == LT_EXPRESSION) {
        result = (cmp < 0);
    } else if (operator == LE_EXPRESSION) {
        result = (cmp <= 0);
    } else {
        char *op_str = dm_get_operator_string(operator);
        dm_runtime_error(line_number, BAD_OPERATOR_FOR_STRING_ERR,
                          STRING_MESSAGE_ARGUMENT, "operator", op_str,
                          MESSAGE_ARGUMENT_END);
    }
    dm_release_string(left->u.string_value);
    dm_release_string(right->u.string_value);

    return result;
}

/*

*/
static DM_Boolean eval_binary_null(CRB_Interpreter *inter, ExpressionType operator,
                 CRB_Value *left, CRB_Value *right, int line_number)
{
    CRB_Boolean result;

    if (operator == EQ_EXPRESSION) {
        result = left->type == CRB_NULL_VALUE && right->type == CRB_NULL_VALUE;
    } else if (operator == NE_EXPRESSION) {
        result =  !(left->type == CRB_NULL_VALUE
                    && right->type == CRB_NULL_VALUE);
    } else {
        char *op_str = crb_get_operator_string(operator);
        crb_runtime_error(line_number, NOT_NULL_OPERATOR_ERR,
                          STRING_MESSAGE_ARGUMENT, "operator", op_str,
                          MESSAGE_ARGUMENT_END);
    }
    release_if_string(left);
    release_if_string(right);

    return result;
}

CRB_String *
chain_string(CRB_Interpreter *inter, CRB_String *left, CRB_String *right)
{
    int len;
    char *str;
    CRB_String *ret;

    len = strlen(left->string) + strlen(right->string);
    str = MEM_malloc(len + 1);
    strcpy(str, left->string);
    strcat(str, right->string);
    ret = crb_create_crowbar_string(inter, str);
    crb_release_string(left);
    crb_release_string(right);

    return ret;
}
/*
 *所有的二元表达式的获取value的总函数
 *
 * */
DM_Value dm_eval_binary_expression(DM_Interpreter *inter, LocalEnvironment *env,
                           ExpressionType operator,
                           Expression *left, Expression *right)
{
    CRB_Value   left_val;
    CRB_Value   right_val;
    CRB_Value   result;

    //递归地调用，求出左右两边的最终数值
    left_val = eval_expression(inter, env, left);
    right_val = eval_expression(inter, env, right);

    if (left_val.type == DM_INT_VALUE
        && right_val.type == DM_INT_VALUE) {
        //当左右的表达式都是int时,调用计算
        eval_binary_int(inter, operator,
                        left_val.u.int_value, right_val.u.int_value,
                        &result, left->line_number);
    }
    /*
     *当其中有一个为double时，进行doubel间的运算
     * */
    else if (left_val.type == DM_DOUBLE_VALUE
               && right_val.type == DM_DOUBLE_VALUE) {

        eval_binary_double(inter, operator,
                           left_val.u.double_value, right_val.u.double_value,
                           &result, left->line_number);
    } else if (left_val.type == DM_INT_VALUE
               && right_val.type == DM_DOUBLE_VALUE) {
        left_val.u.double_value = left_val.u.int_value;
        eval_binary_double(inter, operator,
                           left_val.u.double_value, right_val.u.double_value,
                           &result, left->line_number);
    } else if (left_val.type == DM_DOUBLE_VALUE
               && right_val.type == DM_INT_VALUE) {
        right_val.u.double_value = right_val.u.int_value;
        eval_binary_double(inter, operator,
                           left_val.u.double_value, right_val.u.double_value,
                           &result, left->line_number);
    } else if (left_val.type == CRB_BOOLEAN_VALUE
               && right_val.type == CRB_BOOLEAN_VALUE) {
        result.type = CRB_BOOLEAN_VALUE;
        //boolean之间的操作，返回boolean
        result.u.boolean_value
            = eval_binary_boolean(inter, operator,
                                  left_val.u.boolean_value,
                                  right_val.u.boolean_value,
                                  left->line_number);
    } else if (left_val.type == CRB_STRING_VALUE
               && operator == ADD_EXPRESSION) {
        //当左边是string，而且是加法
        char    buf[LINE_BUF_SIZE];
        DM_String *right_str;

        if (right_val.type == DM_INT_VALUE) {
            //当右边为int时，数值转换，将int转换为string。使用sprintf的方法，将int存储在buf中
            sprintf(buf, "%d", right_val.u.int_value);
            //得到右边
            right_str = dm_create_domi_string(inter, MEM_strdup(buf));
        } else if (right_val.type == DM_DOUBLE_VALUE) {
            //与上述方法类似
            sprintf(buf, "%f", right_val.u.double_value);
            right_str = dm_create_domi_string(inter, MEM_strdup(buf));
        } else if (right_val.type == DM_BOOLEAN_VALUE) {
            //将boolean类型的变量转换为true或者false
            if (right_val.u.boolean_value) {
                right_str = dm_create_crowbar_string(inter, MEM_strdup("true"));
            } else {
                right_str = dm_create_crowbar_string(inter, MEM_strdup("false"));
            }
        } else if (right_val.type == DM_STRING_VALUE) {
            //右边已经是string类型
            right_str = right_val.u.string_value;
        } else if (right_val.type == DM_NATIVE_POINTER_VALUE) {
            sprintf(buf, "(%s:%p)",
                    right_val.u.native_pointer.info->name,
                    right_val.u.native_pointer.pointer);
            right_str = crb_create_crowbar_string(inter, MEM_strdup(buf));
        } else if (right_val.type == DM_NULL_VALUE) {
            right_str = dm_create_crowbar_string(inter, MEM_strdup("null"));
        }
        result.type = DM_STRING_VALUE;
        //将左右两边连接
        result.u.string_value = chain_string(inter,
                                             left_val.u.string_value,
                                             right_str);
    } else if (left_val.type == DM_STRING_VALUE
               && right_val.type == DM_STRING_VALUE) {
        //当左边和右边都为string时,而且此时符号也不会为+
        result.type = DM_BOOLEAN_VALUE;
       
        result.u.boolean_value
            = eval_compare_string(operator, &left_val, &right_val,
                                  left->line_number);
    } else if (left_val.type == CRB_NULL_VALUE
               || right_val.type == CRB_NULL_VALUE) {
      //当左右都为null
        result.type = CRB_BOOLEAN_VALUE;
        //判断等与不等
        result.u.boolean_value
            = eval_binary_null(inter, operator, &left_val, &right_val,
                               left->line_number);
    } else {
      /*
        如果最后都没有匹配上，报错
       */
        char *op_str = crb_get_operator_string(operator);
        crb_runtime_error(left->line_number, BAD_OPERAND_TYPE_ERR,
                          STRING_MESSAGE_ARGUMENT, "operator", op_str,
                          MESSAGE_ARGUMENT_END);
    }

    return result;
}

/*
将逻辑表达式变为DM_Value
*/
static DM_Value eval_logical_and_or_expression(CRB_Interpreter *inter,
                               LocalEnvironment *env,
                               ExpressionType operator,
                               Expression *left, Expression *right)
{
    CRB_Value   left_val;
    CRB_Value   right_val;
    CRB_Value   result;

    result.type = CRB_BOOLEAN_VALUE;
    //先获取左边
    left_val = eval_expression(inter, env, left);

    if (left_val.type != CRB_BOOLEAN_VALUE) {
      //左右必须为DM_Boolean
        crb_runtime_error(left->line_number, NOT_BOOLEAN_TYPE_ERR,
                          MESSAGE_ARGUMENT_END);
    }
    if (operator == LOGICAL_AND_EXPRESSION) {
      //逻辑与
        if (!left_val.u.boolean_value) {
          //当左边为false时，整个为false（短路）
            result.u.boolean_value = CRB_FALSE;
            return result;
        }
    } else if (operator == LOGICAL_OR_EXPRESSION) {
        if (left_val.u.boolean_value) {
          //逻辑或，当左边为true时，整个为true（短路）
            result.u.boolean_value = CRB_TRUE;
            return result;
        }
    } else {
        DBG_panic(("bad operator..%d\n", operator));
    }

    /*
      不符合短路，再来取得右边
     */
    right_val = eval_expression(inter, env, right);
    if (right_val.type != CRB_BOOLEAN_VALUE) {
      //右边必须为DM_Boolean
        crb_runtime_error(right->line_number, NOT_BOOLEAN_TYPE_ERR,
                          MESSAGE_ARGUMENT_END);
    }
    /*
      此时，整个的true和false都由右边决定
     */
    result.u.boolean_value = right_val.u.boolean_value;

    return result;
}

/*
返回取负数之后的值
*/
DM_Value dm_eval_minus_expression(DM_Interpreter *inter, LocalEnvironment *env,
                          Expression *operand)
{
    DM_Value   operand_val;
    DM_Value   result;

    operand_val = eval_expression(inter, env, operand);
    if (operand_val.type == DM_INT_VALUE) {
        result.type = DM_INT_VALUE;
        result.u.int_value = -operand_val.u.int_value;
    } else if (operand_val.type == DM_DOUBLE_VALUE) {
        result.type = DM_DOUBLE_VALUE;
        result.u.double_value = -operand_val.u.double_value;
    } else {
      /*
        只能是int和double
       */
        dm_runtime_error(operand->line_number, MINUS_OPERAND_TYPE_ERR,
                          MESSAGE_ARGUMENT_END);
    }
    return result;
}
/*
分配局部变量的存储空间，并且返回
*/
static LocalEnvironment *alloc_local_environment()
{
    LocalEnvironment *ret;

    ret = MEM_malloc(sizeof(LocalEnvironment));
    ret->variable = NULL;
    ret->global_variable = NULL;

    return ret;
}

/*
回收本地变量
*/
static void dispose_local_environment(DM_Interpreter *inter, LocalEnvironment *env)
{
    while (env->variable) {
      //回收局部变量
        Variable        *temp;
        temp = env->variable;
        if (env->variable->value.type == CRB_STRING_VALUE) {
          //因为string可能存在多个引用
            crb_release_string(env->variable->value.u.string_value);
        }
        env->variable = temp->next;
        MEM_free(temp);
    }
    while (env->global_variable) {
      //回收全局变量
        GlobalVariableRef *ref;
        ref = env->global_variable;
        env->global_variable = ref->next;
        MEM_free(ref);
    }

    MEM_free(env);
}

/*
调用原生函数
*/
static DM_Value call_native_function(CRB_Interpreter *inter, LocalEnvironment *env,
                     Expression *expr, CRB_NativeFunctionProc *proc)
{
    CRB_Value   value;
    int         arg_count;
    ArgumentList        *arg_p;
    CRB_Value   *args;
    int         i;

    //求出实际参数的个数
    for (arg_count = 0, arg_p = expr->u.function_call_expression.argument;
         arg_p; arg_p = arg_p->next) {
        arg_count++;
    }

    //分配空间
    args = MEM_malloc(sizeof(CRB_Value) * arg_count);

    for (arg_p = expr->u.function_call_expression.argument, i = 0;
         arg_p; arg_p = arg_p->next, i++) {
      /*
        求出DM_Value
       */
        args[i] = eval_expression(inter, env, arg_p->expression);
    }
    //调用函数，求得最后的结果
    value = proc(inter, arg_count, args);
    /*
      该释放的释放掉
     */
    for (i = 0; i < arg_count; i++) {
        release_if_string(&args[i]);
    }
    MEM_free(args);

    return value;
}
/*
调用自定义的函数
*/
static DM_Value call_domi_function(DM_Interpreter *inter, LocalEnvironment *env,
                      Expression *expr, FunctionDefinition *func)
{
    DM_Value   value;
    StatementResult     result;
    //实际参数
    ArgumentList        *arg_p;
    //函数定义的形参
    ParameterList       *param_p;
    LocalEnvironment    *local_env;

    /*
      在调用每一个函数时，函数的内部都有自己的环境
     */
    //获取环境
    local_env = alloc_local_environment();
    /*
      找到函数调用时的实参和函数定义时的形参
      for循环
     */
    for (arg_p = expr->u.function_call_expression.argument,
             param_p = func->u.crowbar_f.parameter;
         arg_p;//当没有实际参数时跳出循环
         arg_p = arg_p->next, param_p = param_p->next) {
        CRB_Value arg_val;
      
        if (param_p == NULL) {
          //此时存在实参，但是没有形参，报错
            dm_runtime_error(expr->line_number, ARGUMENT_TOO_MANY_ERR,
                              MESSAGE_ARGUMENT_END);
        }
        /*
          因为实际参数可以是表达式，所以需要先计算出DM_Value值
         */
        arg_val = eval_expression(inter, env, arg_p->expression);
        /*
          在函数调用时，实际参数是当作局部变量来对待的
          名字是param_p->name
          值为  arg_val
         */
        dm_add_local_variable(local_env, param_p->name, &arg_val);
    }
    if (param_p) {
      //此时实际参数比形式参数少，报错
        crb_runtime_error(expr->line_number, ARGUMENT_TOO_FEW_ERR,
                          MESSAGE_ARGUMENT_END);
    }
    result = crb_execute_statement_list(inter, local_env,
                                        func->u.crowbar_f.block
                                        ->statement_list);
    if (result.type == RETURN_STATEMENT_RESULT) {
        value = result.u.return_value;
    } else {
        value.type = CRB_NULL_VALUE;
    }
    dispose_local_environment(inter, local_env);

    return value;
}
/*
在进行函数调用的表达式评估
*/
static CRB_Value eval_function_call_expression(CRB_Interpreter *inter, LocalEnvironment *env,
                              Expression *expr)
{
    CRB_Value           value;
    FunctionDefinition  *func;

    //获取函数名
    char *identifier = expr->u.function_call_expression.identifier;

    func = dm_search_function(identifier);
    if (func == NULL) {
      //当找不到函数的时候
        crb_runtime_error(expr->line_number, FUNCTION_NOT_FOUND_ERR,
                          STRING_MESSAGE_ARGUMENT, "name", identifier,
                          MESSAGE_ARGUMENT_END);
    }
    switch (func->type) {
    case CROWBAR_FUNCTION_DEFINITION:
      //调用自定义的函数
        value = call_domi_function(inter, env, expr, func);
        break;
    case NATIVE_FUNCTION_DEFINITION:
        value = call_native_function(inter, env, expr, func->u.native_f.proc);
        break;
    default:
        DBG_panic(("bad case..%d\n", func->type));
    }

    return value;
}
/*
 *表达式评估，执行函数
 返回得到的DM_vALUE
 * */
static DM_Value eval_expression(CRB_Interpreter *inter, LocalEnvironment *env, Expression *expr)
{
    CRB_Value   v;
    switch (expr->type) {
    case BOOLEAN_EXPRESSION:
        v = eval_boolean_expression(expr->u.boolean_value);
        break;
    case INT_EXPRESSION:
        v = eval_int_expression(expr->u.int_value);
        break;
    case DOUBLE_EXPRESSION:
        v = eval_double_expression(expr->u.double_value);
        break;
    case STRING_EXPRESSION:
        v = eval_string_expression(inter, expr->u.string_value);
        break;
    case IDENTIFIER_EXPRESSION:
        v = eval_identifier_expression(inter, env, expr);
        break;
    case ASSIGN_EXPRESSION:
        v = eval_assign_expression(inter, env,
                                   expr->u.assign_expression.variable,
                                   expr->u.assign_expression.operand);
        break;
    /*
     *大部分的二元运算符，使用dm_eval_binary_expression
     *
     * */
    case ADD_EXPRESSION:        /* FALLTHRU */
    case SUB_EXPRESSION:        /* FALLTHRU */
    case MUL_EXPRESSION:        /* FALLTHRU */
    case DIV_EXPRESSION:        /* FALLTHRU */
    case MOD_EXPRESSION:        /* FALLTHRU */
    case EQ_EXPRESSION: /* FALLTHRU */
    case NE_EXPRESSION: /* FALLTHRU */
    case GT_EXPRESSION: /* FALLTHRU */
    case GE_EXPRESSION: /* FALLTHRU */
    case LT_EXPRESSION: /* FALLTHRU */
    case LE_EXPRESSION:
        v = crb_eval_binary_expression(inter, env,
                                       expr->type,
                                       expr->u.binary_expression.left,
                                       expr->u.binary_expression.right);
        break;
    case LOGICAL_AND_EXPRESSION:/* FALLTHRU */
    case LOGICAL_OR_EXPRESSION:
        v = eval_logical_and_or_expression(inter, env, expr->type,
                                           expr->u.binary_expression.left,
                                           expr->u.binary_expression.right);
        break;
    case MINUS_EXPRESSION:
        v = crb_eval_minus_expression(inter, env, expr->u.minus_expression);
        break;
    case FUNCTION_CALL_EXPRESSION:
        v = eval_function_call_expression(inter, env, expr);
        break;
    case NULL_EXPRESSION:
        v = eval_null_expression();
        break;
    case EXPRESSION_TYPE_COUNT_PLUS_1:  /* FALLTHRU */
    default:
        DBG_panic(("bad case. type..%d\n", expr->type));
    }
    return v;
}

DM_Value dm_eval_expression(CRB_Interpreter *inter, LocalEnvironment *env,
                    Expression *expr)
{
    return eval_expression(inter, env, expr);
}
