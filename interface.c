#include "MEM.h"
#include "DBG.h"
#define GLOBAL_VARIABLE_DEFINE
#include "domi.h"

/*
  添加解释器自带的函数，一共有五个
  print：打印
  fopen：打开文件，返回标识符
  fclose：关闭文件
  fgets：使用文件的标识符，来获取数据
  fputs：写入数据
*/
static void add_native_functions(DM_Interpreter *inter)
{
    DM_add_native_function(inter, "print", dm_nv_print_proc);
    DM_add_native_function(inter, "fopen", dm_nv_fopen_proc);
    DM_add_native_function(inter, "fclose", dm_nv_fclose_proc);
    DM_add_native_function(inter, "fgets", dm_nv_fgets_proc);
    DM_add_native_function(inter, "fputs", dm_nv_fputs_proc);
}

/*
  创建解释器的数据结构
*/
DM_Interpreter *DM_create_interpreter(void)
{
    MEM_Storage storage;
    DM_Interpreter *interpreter;

    storage = MEM_open_storage(0);
    interpreter = MEM_storage_malloc(storage,
                                     sizeof(struct CRB_Interpreter_tag));
    interpreter->interpreter_storage = storage;
    interpreter->execute_storage = NULL;
    interpreter->variable = NULL;
    interpreter->function_list = NULL;
    interpreter->statement_list = NULL;
    interpreter->current_line_number = 1;

    dm_set_current_interpreter(interpreter);
    add_native_functions(interpreter);

    return interpreter;
}

/*
  设置解释器
*/
void DM_compile(CRB_Interpreter *interpreter, FILE *fp)
{
    extern int yyparse(void);
    extern FILE *yyin;

    dm_set_current_interpreter(interpreter);

    yyin = fp;
    if (yyparse()) {
        /* BUGBUG */
        fprintf(stderr, "Error ! Error ! Error !\n");
        exit(1);
    }
    dm_reset_string_literal_buffer();
}

void DM_interpret(DM_Interpreter *interpreter)
{
  /*
    创建远行时的内存
   */
    interpreter->execute_storage = MEM_open_storage(0);
    dm_add_std_fp(interpreter);
    dm_execute_statement_list(interpreter, NULL, interpreter->statement_list);
}

/*
  释放全局变量中的string类型
*/
static void release_global_strings(CRB_Interpreter *interpreter) {
    while (interpreter->variable) {
        Variable *temp = interpreter->variable;
        interpreter->variable = temp->next;
        if (temp->value.type == CRB_STRING_VALUE) {
          //判断是否是string类型
            dm_release_string(temp->value.u.string_value);
        }
    }
}

/*
  回收解释器的内存
*/
void DM_dispose_interpreter(CRB_Interpreter *interpreter)
{
    release_global_strings(interpreter);

    /*回收运行内存*/
    if (interpreter->execute_storage) {
        MEM_dispose_storage(interpreter->execute_storage);
    }

    /*
      回收解释器的内存
     */
    MEM_dispose_storage(interpreter->interpreter_storage);
}

/*
  在解释器的函数链表中，加入mative函数
*/
void DM_add_native_function(DM_Interpreter *interpreter,
                        char *name, DM_NativeFunctionProc *proc)
{
    FunctionDefinition *fd;

    /*新建函数类型*/
    fd = dm_malloc(sizeof(FunctionDefinition));
    fd->name = name;
    fd->type = NATIVE_FUNCTION_DEFINITION;
    fd->u.native_f.proc = proc;
    /*
      插入头结点
     */
    fd->next = interpreter->function_list;

    interpreter->function_list = fd;
}
