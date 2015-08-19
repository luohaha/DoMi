#include "MEM.h"

#include "DBG.h"

#include "domi.h"


/*
函数定义，identifier是函数名，paramter_list是参数链表，block是函数的主体部分
*/
void dm_function_define(char *identifier, ParameterList *parameter_list, Block *block)
{
    //函数的结构
    FunctionDefinition *f;
    //内部解释器
    DM_Interpreter *inter;

    if (dm_search_function(identifier)) {
    //如果发现重名的函数，则报错误
        crb_compile_error(FUNCTION_MULTIPLE_DEFINE_ERR,

                          STRING_MESSAGE_ARGUMENT, "name", identifier,

                          MESSAGE_ARGUMENT_END);

        return;

    }
    //获取当前的解释器
    inter = dm_get_current_interpreter();



    f = crb_malloc(sizeof(FunctionDefinition));
    //函数的名称
    f->name = identifier;
    //函数的类型为自定义
    f->type = DOMI_FUNCTION_DEFINITION;
    //保存参数链表
    f->u.crowbar_f.parameter = parameter_list;
    //保存函数的主体
    f->u.crowbar_f.block = block;
    //将当前的函数，插到解释器的函数链表的头部
    f->next = inter->function_list;
    inter->function_list = f;

}


/*
产生一个形式参数链表,只有一个结点
*/
ParameterList *dm_create_parameter(char *identifier)
{

    ParameterList       *p;

    //分配空间

    p = dm_malloc(sizeof(ParameterList));
    //名称
    p->name = identifier;
    //下一个
    p->next = NULL;



    return p;

}


/*
将新的参数结点，插入参数链表的尾部
*/
ParameterList *dm_chain_parameter(ParameterList *list, char *identifier)
{

    ParameterList *pos;


    //找到链表的尾部
    for (pos = list; pos->next; pos = pos->next)

        ;
    //插入尾部
    pos->next = crb_create_parameter(identifier);

    return list;

}


/*
创建只有一个结点的函数传入参数的链表
*/
ArgumentList *dm_create_argument_list(Expression *expression)
{

    ArgumentList *al;



    al = dm_malloc(sizeof(ArgumentList));

    al->expression = expression;

    al->next = NULL;



    return al;

}


/*
将函数传入的参数，插入函数传入参数的链表的尾部
*/
ArgumentList *dm_chain_argument_list(ArgumentList *list, Expression *expr)
{

    ArgumentList *pos;


    //找到尾部
    for (pos = list; pos->next; pos = pos->next)

        ;
    //插入
    pos->next = dm_create_argument_list(expr);



    return list;

}


/*
和上面两个类似，创建语句链表，只有一个结点
*/
StatementList *dm_create_statement_list(Statement *statement)
{

    StatementList *sl;



    sl = dm_malloc(sizeof(StatementList));

    sl->statement = statement;

    sl->next = NULL;



    return sl;

}


/*
创建语句链表，将新的语句插入到尾部
*/
StatementList *dm_chain_statement_list(StatementList *list, Statement *statement)
{

    StatementList *pos;



    if (list == NULL)

        return dm_create_statement_list(statement);


    //找到尾部
    for (pos = list; pos->next; pos = pos->next)

        ;
    //插入
    pos->next = dm_create_statement_list(statement);



    return list;

}


/*
产生新的表达式变量
*/
Expression *dm_alloc_expression(ExpressionType type)
{

    Expression  *exp;



    exp = dm_malloc(sizeof(Expression));
    //表达式类型
    exp->type = type;
    //所在的行号
    exp->line_number = dm_get_current_interpreter()->current_line_number;



    return exp;

}


/*
产生赋值表达式
*/
Expression *dm_create_assign_expression(char *variable, Expression *operand)
{

    Expression *exp;


    //类型为赋值表达式
    exp = dm_alloc_expression(ASSIGN_EXPRESSION);
    //左边的变量名
    exp->u.assign_expression.variable = variable;
    //右边的表达式
    exp->u.assign_expression.operand = operand;



    return exp;

}


/*
  将DM_VALUE转换为Expression，DM_Value可能是int,boolean或者double
*/
static Expression convert_value_to_expression(DM_Value *v)
{

    Expression  expr;



    if (v->type == DM_INT_VALUE) {

        expr.type = INT_EXPRESSION;

        expr.u.int_value = v->u.int_value;

    } else if (v->type == DM_DOUBLE_VALUE) {

        expr.type = DOUBLE_EXPRESSION;

        expr.u.double_value = v->u.double_value;

    } else {

        DBG_assert(v->type == DM_BOOLEAN_VALUE,

                   ("v->type..%d\n", v->type));

        expr.type = BOOLEAN_EXPRESSION;

        expr.u.boolean_value = v->u.boolean_value;

    }

    return expr;

}


/*
创造二元表达式
*/
Expression *dm_create_binary_expression(ExpressionType operator, Expression *left, Expression *right)
{
  /*
    如果左右两边是数值的话，要提前算出结果
    不是的话，简单滴赋值就行啦
   */
    if ((left->type == INT_EXPRESSION || left->type == DOUBLE_EXPRESSION)
        && (right->type == INT_EXPRESSION || right->type == DOUBLE_EXPRESSION)) {

        DM_Value v;

        /*
          这里做了一个提前计算，将二元表达式的最终值算出来，
          并且返回DM_Value
         */
        v = dm_eval_binary_expression(dm_get_current_interpreter(),

                                       NULL, operator, left, right);

        /* Overwriting left hand expression. */

        /*
          再将DM_Value转换为expression
         */
        *left = convert_value_to_expression(&v);



        return left;

    } else {

        Expression *exp;
        //获取一个空的expresson
        exp = dm_alloc_expression(operator);

        exp->u.binary_expression.left = left;

        exp->u.binary_expression.right = right;

        return exp;

    }

}


/*
 *
 * */
Expression *dm_create_minus_expression(Expression *operand)
{

  /*
    如果-号右边是数值，就调用eval.c中的函数，提前将它转换为DM_Value,然后再转换回expression
   */
    if (operand->type == INT_EXPRESSION

        || operand->type == DOUBLE_EXPRESSION) {

        CRB_Value       v;

        v = dm_eval_minus_expression(dm_get_current_interpreter(),

                                      NULL, operand);

        /* Notice! Overwriting operand expression. */

        *operand = convert_value_to_expression(&v);

        return operand;

    } else {

        Expression      *exp;

        exp = dm_alloc_expression(MINUS_EXPRESSION);

        exp->u.minus_expression = operand;

        return exp;

    }

}


/*
创建标识符链表，只有一个结点
*/
Expression *dm_create_identifier_expression(char *identifier)
{

    Expression  *exp;



    exp = dm_alloc_expression(IDENTIFIER_EXPRESSION);

    exp->u.identifier = identifier;



    return exp;

}


/*
创建函数调用表达式
*/
Expression *dm_create_function_call_expression(char *func_name, ArgumentList *argument)
{

    Expression  *exp;



    exp = dm_alloc_expression(FUNCTION_CALL_EXPRESSION);

    exp->u.function_call_expression.identifier = func_name;

    exp->u.function_call_expression.argument = argument;



    return exp;

}


/*
创建boolean表达式
*/
Expression *dm_create_boolean_expression(CRB_Boolean value)
{

    Expression *exp;



    exp = crb_alloc_expression(BOOLEAN_EXPRESSION);

    exp->u.boolean_value = value;



    return exp;

}


/*
创建null表达式
*/
Expression *crb_create_null_expression(void)
{

    Expression  *exp;



    exp = crb_alloc_expression(NULL_EXPRESSION);



    return exp;

}


/*
内部函数

返回特定类型和具体行号的statement（语句）
*/
static Statement *alloc_statement(StatementType type)
{

    Statement *st;



    st = dm_malloc(sizeof(Statement));

    st->type = type;

    st->line_number = dm_get_current_interpreter()->current_line_number;



    return st;

}


/*
创建global语句,使用标识符链表
*/
Statement *dm_create_global_statement(IdentifierList *identifier_list)
{

    Statement *st;



    st = alloc_statement(GLOBAL_STATEMENT);

    st->u.global_s.identifier_list = identifier_list;



    return st;

}


/*
创建单结点的标识符链表,
*/
IdentifierList *dm_create_global_identifier(char *identifier)
{

    IdentifierList      *i_list;



    i_list = dm_malloc(sizeof(IdentifierList));
    //标识符名字
    i_list->name = identifier;

    i_list->next = NULL;



    return i_list;

}


/*
在标识符链表的尾部，插入标识符
*/
IdentifierList *dm_chain_identifier(IdentifierList *list, char *identifier)
{

    IdentifierList *pos;


    //找到尾部
    for (pos = list; pos->next; pos = pos->next)

        ;
    //插入
    pos->next = dm_create_global_identifier(identifier);



    return list;

}


/*
if语句
*/
Statement *dm_create_if_statement(Expression *condition, Block *then_block, Elsif *elsif_list, Block *else_block)
{

    Statement *st;



    st = alloc_statement(IF_STATEMENT);

    st->u.if_s.condition = condition;

    st->u.if_s.then_block = then_block;

    st->u.if_s.elsif_list = elsif_list;

    st->u.if_s.else_block = else_block;



    return st;

}


/*
将elsif结点插入链表的尾部
*/
Elsif *dm_chain_elsif_list(Elsif *list, Elsif *add)
{

    Elsif *pos;


    //找到尾部
    for (pos = list; pos->next; pos = pos->next)

        ;
    //插入
    pos->next = add;



    return list;

}


/*
创建只有一个结点的elsif链表
*/
Elsif *dm_create_elsif(Expression *expr, Block *block)
{

    Elsif *ei;



    ei = dm_malloc(sizeof(Elsif));

    ei->condition = expr;

    ei->block = block;

    ei->next = NULL;



    return ei;

}


/*
创建while语句
*/
Statement *dm_create_while_statement(Expression *condition, Block *block)
{

    Statement *st;



    st = alloc_statement(WHILE_STATEMENT);

    st->u.while_s.condition = condition;

    st->u.while_s.block = block;



    return st;

}


/*
创建for语句
init->for第一个表达式
cond->for的判断式
post->for的后续操作
*/
Statement *dm_create_for_statement(Expression *init, Expression *cond, Expression *post, Block *block)
{

    Statement *st;


    //获取statement
    st = alloc_statement(FOR_STATEMENT);
    //赋值
    st->u.for_s.init = init;

    st->u.for_s.condition = cond;

    st->u.for_s.post = post;

    st->u.for_s.block = block;



    return st;

}


/*
创建block代码块,{ 与 }之间的部分
*/
Block *dm_create_block(StatementList *statement_list)
{

    Block *block;


    //获取内存
    block = crb_malloc(sizeof(Block));
    //赋值
    block->statement_list = statement_list;



    return block;

}


//创建只有一行表达式的语句（最简单的语句了!）,expression + ;
Statement *dm_create_expression_statement(Expression *expression)
{

    Statement *st;


    //创建并且赋值
    st = alloc_statement(EXPRESSION_STATEMENT);

    st->u.expression_s = expression;



    return st;

}


/*
创建返回语句
*/
Statement *dm_create_return_statement(Expression *expression)
{

    Statement *st;



    st = alloc_statement(RETURN_STATEMENT);
    //将return后的表达式赋值
    st->u.return_s.return_value = expression;



    return st;

}


/*
 *创建break语句
 * */
Statement *dm_create_break_statement(void)
{

    return alloc_statement(BREAK_STATEMENT);

}


/*
 *创建continue语句
 * */
Statement *dm_create_continue_statement(void)
{

    return alloc_statement(CONTINUE_STATEMENT);

}


