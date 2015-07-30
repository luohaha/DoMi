#ifndef DM_DEV_H
#define DM_DEV_H

#include "DM.h"

/*
BOOLEAN类型
*/
typedef enum {

    DM_FALSE = 0,

    DM_TRUE = 1

} DM_Boolean;



typedef struct DM_String_tag DM_String;


/*
原生指针
*/
typedef struct {

    char        *name;

} DM_NativePointerInfo;


/*
内部类型，因为domi在使用的时候是不用预先声明类型的，所以需要内部类型来存储不同的值
*/
typedef enum {
    //boolean
    DM_BOOLEAN_VALUE = 1,
    //int
    DM_INT_VALUE,
    //double
    DM_DOUBLE_VALUE,
    //string
    DM_STRING_VALUE,
    //指针
    DM_NATIVE_POINTER_VALUE,
    //null
    DM_NULL_VALUE

} DM_ValueType;


/*
原生指针
*/
typedef struct {

    DM_NativePointerInfo       *info;

    void                        *pointer;

} DM_NativePointer;


/*
domi内部用来存储变量的数据结构
*/
typedef struct {

    DM_ValueType       type;

    union {

        CRB_Boolean     boolean_value;

        int             int_value;

        double          double_value;

        DM_String      *string_value;

        DM_NativePointer       native_pointer;

    } u;

} DM_Value;


/*
自带函数
*/
typedef CRB_Value CRB_NativeFunctionProc(CRB_Interpreter *interpreter, int arg_count, CRB_Value *args);



void CRB_add_native_function(CRB_Interpreter *interpreter,

                             char *name, CRB_NativeFunctionProc *proc);

void CRB_add_global_variable(CRB_Interpreter *inter,

                             char *identifier, CRB_Value *value);



#endif
