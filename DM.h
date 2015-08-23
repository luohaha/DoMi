#ifndef DM_H
#define DM_H
#include <stdio.h>

/*定义不完全类型*/
/*typedef struct DM_Interpreter_tag DM_Interpreter;*/
typedef struct Variable_tag Variable;
typedef struct Expression_tag Expression;
typedef struct FunctionDefinition_tag FunctionDefinition;
typedef struct StatementList_tag StatementList;
typedef struct Statement_tag Statement;
typedef struct ParameterList_tag ParameterList;
typedef struct Block_tag Block;
typedef struct MessageFormat_tag MessageFormat;
typedef struct DM_Interpreter_tag DM_Interpreter;
typedef struct WhileStatement_tag WhileStatement;
typedef struct IfStatement_tag IfStatement;
typedef struct Elsif_tag Elsif;
typedef struct ForStatement_tag ForStatement;
typedef struct ReturnStatement_tag ReturnStatement;
typedef struct GlobalStatement_tag GlobalStatement;
typedef struct IdentifierList_tag IdentifierList;
typedef struct BinaryExpression_tag BinaryExpression;
typedef struct AssignExpression_tag AssignExpression;
typedef struct FunctionCallExpression_tag FunctionCallExpression;
typedef struct ArgumentList_tag ArgumentList;
typedef struct GlobalVariableRef_tag GlobalVariableRef;
typedef struct LocalEnvironment_tag LocalEnvironment;
typedef struct StringPool_tag StringPool;
typedef struct StatementResult_tag StatementResult;
#endif/*#ifndef DM_H*/
