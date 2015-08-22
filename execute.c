#include <math.h>
#include <string.h>
#include "MEM.h"
#include "DBG.h"
#include "domi.h"

static StatementResult execute_statement(DM_Interpreter *inter, LocalEnvironment *env,
                  Statement *statement);


/*
  表达式语句执行
*/
static StatementResult execute_expression_statement(DM_Interpreter *inter, LocalEnvironment *env,
                             Statement *statement)
{
    StatementResult result;
    DM_Value v;

    result.type = NORMAL_STATEMENT_RESULT;

    /*
      进行表达式评估
     */
    v = dm_eval_expression(inter, env, statement->u.expression_s);
    if (v.type == DM_STRING_VALUE) {
        dm_release_string(v.u.string_value);
    }

    return result;
}

/*
  global语句
 */
static StatementResult execute_global_statement(CRB_Interpreter *inter, LocalEnvironment *env,
                         Statement *statement)
{
    IdentifierList *pos;
    StatementResult result;

    result.type = NORMAL_STATEMENT_RESULT;

    /*
      env保存当前运行函数的局部变量，所以在声明global时,
      不能在函数外使用global，env==null时，在函数外
     */
    if (env == NULL) {
        dm_runtime_error(statement->line_number,
                          GLOBAL_STATEMENT_IN_TOPLEVEL_ERR,
                          MESSAGE_ARGUMENT_END);
    }
    for (pos = statement->u.global_s.identifier_list; pos; pos = pos->next) {
        GlobalVariableRef *ref_pos;
        GlobalVariableRef *new_ref;
        Variable *variable;
        for (ref_pos = env->global_variable; ref_pos;
             ref_pos = ref_pos->next) {
          /*当前的变量在全局变量已经找到，goto继续查找下一个*/
            if (!strcmp(ref_pos->variable->name, pos->name))
                goto NEXT_IDENTIFIER;
        }
        /*
          如果找不到全局变量，再来查找全局变量
          ???
         */
        variable = dm_search_global_variable(inter, pos->name);
        if (variable == NULL) {
          /*
            报错
           */
            dm_runtime_error(statement->line_number,
                              GLOBAL_VARIABLE_NOT_FOUND_ERR,
                              STRING_MESSAGE_ARGUMENT, "name", pos->name,
                              MESSAGE_ARGUMENT_END);
        }
        /*
          将其插入env的全局链表的头部
         */
        new_ref = MEM_malloc(sizeof(GlobalVariableRef));
        new_ref->variable = variable;
        new_ref->next = env->global_variable;
        env->global_variable = new_ref;
      NEXT_IDENTIFIER:
        ;
    }

    return result;
}

/*
  else if 的执行
*/
static StatementResult execute_elsif(DM_Interpreter *inter, LocalEnvironment *env,
              Elsif *elsif_list, DM_Boolean *executed)
{
    StatementResult result;
    DM_Value   cond;
    Elsif *pos;

    *executed = DM_FALSE;
    result.type = NORMAL_STATEMENT_RESULT;
    for (pos = elsif_list; pos; pos = pos->next) {
      /*
        计算出else if中的判断语句
       */
        cond = dm_eval_expression(inter, env, pos->condition);
        if (cond.type != DM_BOOLEAN_VALUE) {
            dm_runtime_error(pos->condition->line_number,
                              NOT_BOOLEAN_TYPE_ERR, MESSAGE_ARGUMENT_END);
        }
        if (cond.u.boolean_value) {
            result = dm_execute_statement_list(inter, env,
                                                pos->block->statement_list);
            /*
              执行成功
             */
            *executed = DM_TRUE;
            if (result.type != NORMAL_STATEMENT_RESULT)
                goto FUNC_END;
        }
    }

  FUNC_END:
    return result;
}

/*
  if语句的执行
*/
static StatementResult execute_if_statement(DM_Interpreter *inter, LocalEnvironment *env,
                     Statement *statement)
{
    StatementResult result;
    DM_Value   cond;

    result.type = NORMAL_STATEMENT_RESULT;
    /*
      计算出if中的条件判断
     */
    cond = dm_eval_expression(inter, env, statement->u.if_s.condition);
    if (cond.type != DM_BOOLEAN_VALUE) {
        dm_runtime_error(statement->u.if_s.condition->line_number,
                          NOT_BOOLEAN_TYPE_ERR, MESSAGE_ARGUMENT_END);
    }
    DBG_assert(cond.type == DM_BOOLEAN_VALUE, ("cond.type..%d", cond.type));

    /*
      if判断为true
     */
    if (cond.u.boolean_value) {
      /*
        执行then后面的语句
       */
        result = dm_execute_statement_list(inter, env,
                                            statement->u.if_s.then_block->statement_list);
    } else {
        DM_Boolean elsif_executed;
        result = execute_elsif(inter, env, statement->u.if_s.elsif_list,
                               &elsif_executed);
        if (result.type != NORMAL_STATEMENT_RESULT)
            goto FUNC_END;
        /*
          如果else if没有被执行，再来执行else里面的block
         */
        if (!elsif_executed && statement->u.if_s.else_block) {
            result = dm_execute_statement_list(inter, env,
                                                statement->u.if_s.else_block->statement_list);
        }
    }

  FUNC_END:
    return result;
}

/*
  执行while语句
*/
static StatementResult execute_while_statement(DM_Interpreter *inter, LocalEnvironment *env,
                        Statement *statement)
{
    StatementResult result;
    /*
      while中的条件判断
     */
    DM_Value   cond;

    result.type = NORMAL_STATEMENT_RESULT;
    for (;;) {
      /*
        获取条件判断的值
       */
        cond = dm_eval_expression(inter, env, statement->u.while_s.condition);
        if (cond.type != DM_BOOLEAN_VALUE) {
            dm_runtime_error(statement->u.while_s.condition->line_number,
                              NOT_BOOLEAN_TYPE_ERR, MESSAGE_ARGUMENT_END);
        }
        DBG_assert(cond.type == DM_BOOLEAN_VALUE,
                   ("cond.type..%d", cond.type));
        /*
          while中条件不正确，跳出while
         */
        if (!cond.u.boolean_value)
            break;

        /*
          执行while内部语句
         */
        result = dm_execute_statement_list(inter, env,
                                            statement->u.while_s.block
                                            ->statement_list);
        if (result.type == RETURN_STATEMENT_RESULT) {
            break;
        } else if (result.type == BREAK_STATEMENT_RESULT) {
          /*
            使用了return
           */
            result.type = NORMAL_STATEMENT_RESULT;
            break;
        }
    }

    return result;
}

/*
  for语句执行
*/
static StatementResult execute_for_statement(DM_Interpreter *inter, LocalEnvironment *env,
                      Statement *statement)
{
    StatementResult result;
    DM_Value   cond;

    result.type = NORMAL_STATEMENT_RESULT;

    if (statement->u.for_s.init) {
        dm_eval_expression(inter, env, statement->u.for_s.init);
    }
    for (;;) {
        if (statement->u.for_s.condition) {
            cond = dm_eval_expression(inter, env,
                                       statement->u.for_s.condition);
            if (cond.type != DM_BOOLEAN_VALUE) {
                dm_runtime_error(statement->u.for_s.condition->line_number,
                                  NOT_BOOLEAN_TYPE_ERR, MESSAGE_ARGUMENT_END);
            }
            DBG_assert(cond.type == DM_BOOLEAN_VALUE,
                       ("cond.type..%d", cond.type));
            if (!cond.u.boolean_value)
                break;
        }
        /*
          执行for里面的block的statementlist
         */
        result = dm_execute_statement_list(inter, env,
                                            statement->u.for_s.block->statement_list);
        if (result.type == RETURN_STATEMENT_RESULT) {
            break;
        } else if (result.type == BREAK_STATEMENT_RESULT) {
            result.type = NORMAL_STATEMENT_RESULT;
            break;
        }

        /*
          执行for的第三个段
         */
        if (statement->u.for_s.post) {
            dm_eval_expression(inter, env, statement->u.for_s.post);
        }
    }

    return result;
}


/*
  result语句
 */
static StatementResult execute_return_statement(DM_Interpreter *inter, LocalEnvironment *env,
                         Statement *statement)
{
    StatementResult result;

    result.type = RETURN_STATEMENT_RESULT;
    if (statement->u.return_s.return_value) {
      /*
        如果return后面有语句
       */
        result.u.return_value
            = dm_eval_expression(inter, env,
                                  statement->u.return_s.return_value);
    } else {
        result.u.return_value.type = DM_NULL_VALUE;
    }

    return result;
}

/*
  break语句
*/
static StatementResult execute_break_statement(CRB_Interpreter *inter, LocalEnvironment *env,
                        Statement *statement)
{
    StatementResult result;

    result.type = BREAK_STATEMENT_RESULT;

    return result;
}

/*
  continue
*/
static StatementResult execute_continue_statement(CRB_Interpreter *inter, LocalEnvironment *env,
                           Statement *statement)
{
    StatementResult result;

    result.type = CONTINUE_STATEMENT_RESULT;

    return result;
}

/*
  执行statement
*/
static StatementResult execute_statement(CRB_Interpreter *inter, LocalEnvironment *env,
                  Statement *statement)
{
    StatementResult result;

    result.type = NORMAL_STATEMENT_RESULT;

    /*
      根据不同类型执行
     */
    switch (statement->type) {
    case EXPRESSION_STATEMENT:
        result = execute_expression_statement(inter, env, statement);
        break;
    case GLOBAL_STATEMENT:
        result = execute_global_statement(inter, env, statement);
        break;
    case IF_STATEMENT:
        result = execute_if_statement(inter, env, statement);
        break;
    case WHILE_STATEMENT:
        result = execute_while_statement(inter, env, statement);
        break;
    case FOR_STATEMENT:
        result = execute_for_statement(inter, env, statement);
        break;
    case RETURN_STATEMENT:
        result = execute_return_statement(inter, env, statement);
        break;
    case BREAK_STATEMENT:
        result = execute_break_statement(inter, env, statement);
        break;
    case CONTINUE_STATEMENT:
        result = execute_continue_statement(inter, env, statement);
        break;
    case STATEMENT_TYPE_COUNT_PLUS_1:   /* FALLTHRU */
    default:
        DBG_panic(("bad case...%d", statement->type));
    }

    return result;
}

/*
  按照statement的list顺序来执行statement
*/
StatementResult dm_execute_statement_list(DM_Interpreter *inter, LocalEnvironment *env,
                           StatementList *list)
{
    StatementList *pos;
    StatementResult result;

    result.type = NORMAL_STATEMENT_RESULT;
    /*
      顺序执行statementlist上的statement
     */
    for (pos = list; pos; pos = pos->next) {
      /*
        执行
       */
        result = execute_statement(inter, env, pos->statement);
        if (result.type != NORMAL_STATEMENT_RESULT)
            goto FUNC_END;
    }

  FUNC_END:
    return result;
}
