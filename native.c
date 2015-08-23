#include <stdio.h>
#include <string.h>
#include "MEM.h"
#include "DBG.h"
#include "DM_dev.h"
#include "domi.h"

#define NATIVE_LIB_NAME "domi.lang.file"

static DM_NativePointerInfo st_native_lib_info = {
    NATIVE_LIB_NAME
};

/*
  解释器自带函数print
*/
DM_Value dm_nv_print_proc(DM_Interpreter *interpreter,
                            int arg_count, DM_Value *args)
{
    DM_Value value;

    value.type = DM_NULL_VALUE;

    if (arg_count < 1) {
        dm_runtime_error(0, ARGUMENT_TOO_FEW_ERR,
                          MESSAGE_ARGUMENT_END);
    } else if (arg_count > 1) {
        dm_runtime_error(0, ARGUMENT_TOO_MANY_ERR,
                          MESSAGE_ARGUMENT_END);
    }
    switch (args[0].type) {
    case DM_BOOLEAN_VALUE:
        if (args[0].u.boolean_value) {
            printf("true");
        } else {
            printf("false");
        }
        break;
    case DM_INT_VALUE:
        printf("%d", args[0].u.int_value);
        break;
    case DM_DOUBLE_VALUE:
        printf("%f", args[0].u.double_value);
        break;
    case DM_STRING_VALUE:
        printf("%s", args[0].u.string_value->string);
        break;
    case DM_NATIVE_POINTER_VALUE:
        printf("(%s:%p)",
               args[0].u.native_pointer.info->name,
               args[0].u.native_pointer.pointer);
        break;
    case DM_NULL_VALUE:
        printf("null");
        break;
    }

    return value;
}

/*
  解释器自带的函数fopen
*/
DM_Value dm_nv_fopen_proc(DM_Interpreter *interpreter,
                            int arg_count, DM_Value *args)
{
    DM_Value value;
    FILE *fp;

    if (arg_count < 2) {
        dm_runtime_error(0, ARGUMENT_TOO_FEW_ERR,
                          MESSAGE_ARGUMENT_END);
    } else if (arg_count > 2) {
        dm_runtime_error(0, ARGUMENT_TOO_MANY_ERR,
                          MESSAGE_ARGUMENT_END);
    }
    /*
      args符合条件
     */
    if (args[0].type != DM_STRING_VALUE
        || args[1].type != DM_STRING_VALUE) {
        dm_runtime_error(0, FOPEN_ARGUMENT_TYPE_ERR,
                          MESSAGE_ARGUMENT_END);
    }
    
    /*
      args[0]包含文件名字
      args[1]包含打开的模式(读写)
     */
    fp = fopen(args[0].u.string_value->string,
               args[1].u.string_value->string);
    if (fp == NULL) {
        value.type = DM_NULL_VALUE;
    } else {
      /*
        打开成功
       */
        value.type = DM_NATIVE_POINTER_VALUE;
        value.u.native_pointer.info = &st_native_lib_info;
        value.u.native_pointer.pointer = fp;
    }

    return value;
}

/*
  检查native pointer
*/
static DM_Boolean check_native_pointer(DM_Value *value)
{
    return value->u.native_pointer.info == &st_native_lib_info;
}

/*
  关闭文件fclose
*/
DM_Value dm_nv_fclose_proc(DM_Interpreter *interpreter,
                             int arg_count, DM_Value *args)
{
    DM_Value value;
    FILE *fp;

    value.type = DM_NULL_VALUE;
    if (arg_count < 1) {
        dm_runtime_error(0, ARGUMENT_TOO_FEW_ERR,
                          MESSAGE_ARGUMENT_END);
    } else if (arg_count > 1) {
        dm_runtime_error(0, ARGUMENT_TOO_MANY_ERR,
                          MESSAGE_ARGUMENT_END);
    }
    if (args[0].type != DM_NATIVE_POINTER_VALUE
        || !check_native_pointer(&args[0])) {
        dm_runtime_error(0, FCLOSE_ARGUMENT_TYPE_ERR,
                          MESSAGE_ARGUMENT_END);
    }
    /*
      关闭
     */
    fp = args[0].u.native_pointer.pointer;
    fclose(fp);

    return value;
}

/*
  fgets()
*/
DM_Value dm_nv_fgets_proc(DM_Interpreter *interpreter,
                            int arg_count, DM_Value *args)
{
    DM_Value value;
    FILE *fp;
    char buf[LINE_BUF_SIZE];
    char *ret_buf = NULL;
    int ret_len = 0;

    /*
      检查
     */
    if (arg_count < 1) {
        dm_runtime_error(0, ARGUMENT_TOO_FEW_ERR,
                          MESSAGE_ARGUMENT_END);
    } else if (arg_count > 1) {
        dm_runtime_error(0, ARGUMENT_TOO_MANY_ERR,
                          MESSAGE_ARGUMENT_END);
    }
    if (args[0].type != DM_NATIVE_POINTER_VALUE
        || !check_native_pointer(&args[0])) {
        dm_runtime_error(0, FGETS_ARGUMENT_TYPE_ERR,
                          MESSAGE_ARGUMENT_END);
    }
    fp = args[0].u.native_pointer.pointer;

    /*
      不断地从fp中读取行
     */
    while (fgets(buf, LINE_BUF_SIZE, fp)) {
        int new_len;
        /*更新buf的长度*/
        new_len = ret_len + strlen(buf);
        ret_buf = MEM_realloc(ret_buf, new_len + 1);
        if (ret_len == 0) {
            strcpy(ret_buf, buf);
        } else {
          /*
            将buf复制到ret_buf的后面
           */
            strcat(ret_buf, buf);
        }
        ret_len = new_len;
        if (ret_buf[ret_len-1] == '\n')
            break;
    }
    /*
      如果读取到数据
     */
    if (ret_len > 0) {
        value.type = DM_STRING_VALUE;
        value.u.string_value = dm_create_domi_string(interpreter, ret_buf);
    } else {
        value.type = DM_NULL_VALUE;
    }

    return value;
}

/*
  fputs()
*/
DM_Value dm_nv_fputs_proc(DM_Interpreter *interpreter,
                            int arg_count, DM_Value *args)
{
    DM_Value value;
    FILE *fp;

    value.type = DM_NULL_VALUE;
    if (arg_count < 2) {
        dm_runtime_error(0, ARGUMENT_TOO_FEW_ERR,
                          MESSAGE_ARGUMENT_END);
    } else if (arg_count > 2) {
        dm_runtime_error(0, ARGUMENT_TOO_MANY_ERR,
                          MESSAGE_ARGUMENT_END);
    }
    if (args[0].type != DM_STRING_VALUE
        || (args[1].type != DM_NATIVE_POINTER_VALUE
            || !check_native_pointer(&args[1]))) {
        dm_runtime_error(0, FPUTS_ARGUMENT_TYPE_ERR,
                          MESSAGE_ARGUMENT_END);
    }
    /*
      将args[0]里的string写入args[1]对应的文件中
     */
    fp = args[1].u.native_pointer.pointer;

    fputs(args[0].u.string_value->string, fp);

    return value;
}

/*
  将stdin stdout stderr 加入
*/
void dm_add_std_fp(DM_Interpreter *inter)
{
    DM_Value fp_value;

    fp_value.type = DM_NATIVE_POINTER_VALUE;
    fp_value.u.native_pointer.info = &st_native_lib_info;

    fp_value.u.native_pointer.pointer = stdin;
    DM_add_global_variable(inter, "STDIN", &fp_value);

    fp_value.u.native_pointer.pointer = stdout;
    DM_add_global_variable(inter, "STDOUT", &fp_value);

    fp_value.u.native_pointer.pointer = stderr;
    DM_add_global_variable(inter, "STDERR", &fp_value);
}
