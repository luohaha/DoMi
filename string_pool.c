#include <stdio.h>
#include <string.h>
#include "MEM.h"
#include "DBG.h"
#include "domi.h"

/*
分配DM_String
is_literal是字面常量：也就是被""包裹，出现在代码中的字符串
*/
static DM_String *alloc_dm_string(CRB_Interpreter *inter, char *str, DM_Boolean is_literal)
{
    DM_String *ret;

    ret = MEM_malloc(sizeof(DM_String));
    //引用数量
    ret->ref_count = 0;
    //是否为字面常量
    ret->is_literal = is_literal;
    //记录string
    ret->string = str;

    return ret;
}

/*
生成字符常量string
*/
DM_String *dm_literal_to_dm_string(CRB_Interpreter *inter, char *str)
{
    CRB_String *ret;

    ret = alloc_dm_string(inter, str, DM_TRUE);
    ret->ref_count = 1;

    return ret;
}

/*
引用计数自增
*/
void dm_refer_string(DM_String *str)
{
    str->ref_count++;
}

/*
引用计数自减
*/
void dm_release_string(DM_String *str)
{
    str->ref_count--;

    DBG_assert(str->ref_count >= 0, ("str->ref_count..%d\n",
                                     str->ref_count));
    if (str->ref_count == 0) {
      //当引用计数为0时，如果不为字符常量，回收
        if (!str->is_literal) {
            MEM_free(str->string);
        }
        MEM_free(str);
    }
}

/*
生成非字符常量的string
*/
DM_String *dm_create_domi_string(DM_Interpreter *inter, char *str)
{
    DM_String *ret = alloc_dm_string(inter, str, CRB_FALSE);
    ret->ref_count = 1;

    return ret;
}
