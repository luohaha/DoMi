#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include "MEM.h"
#include "debug.h"

static DBG_Controller   st_current_controller;
static char     *st_current_file_name;
static int      st_current_line;
static char     *st_assert_expression;

static struct DBG_Controller_tag st_default_controller = {
    NULL, /*stderr,*/
    INT_MAX,
};
DBG_Controller  dbg_default_controller = &st_default_controller;

/*
  创建新的controller
 */
DBG_Controller DBG_create_controller_func(void)
{
    DBG_Controller      controller;

    controller = MEM_malloc(sizeof(struct DBG_Controller_tag));
    controller->debug_write_fp = NULL;
    controller->current_debug_level = INT_MAX;

    return controller;
}

/*
  设置
*/
void DBG_set_debug_level_func(DBG_Controller controller, int level)
{
    controller->current_debug_level = level;
}

void DBG_set_debug_write_fp_func(DBG_Controller controller, FILE *fp)
{
    controller->debug_write_fp = fp;
}

/*
  处理化debug输出为stderr
*/
static void initialize_debug_write_fp(void)
{
    if (st_default_controller.debug_write_fp == NULL) {
        st_default_controller.debug_write_fp = stderr;
    }
}

static void assert_func(FILE *fp, char *file, int line, char *expression,
            char *fmt,  va_list ap)
{
    fprintf(fp, "Assertion failure (%s) file..%s line..%d\n",
            expression, file, line);
    if (fmt) {
        vfprintf(fp, fmt, ap);
    }
}

/*
  设置debug错误输出
*/
void DBG_assert_func(char *fmt, ...)
{
    va_list     ap;

    va_start(ap, fmt);
    initialize_debug_write_fp();
    assert_func(st_current_controller->debug_write_fp,
                st_current_file_name, st_current_line,
                st_assert_expression, fmt, ap);
    /*
      每次都会将错误输出到stderr
     */
    assert_func(stderr,
                st_current_file_name, st_current_line,
                st_assert_expression, fmt, ap);
    va_end(ap);
    abort();
}

static void panic_func(FILE *fp, char *file, int line, char *fmt,  va_list ap)
{
    fprintf(fp, "Panic!! file..%s line..%d\n", file, line);
    if (fmt) {
        vfprintf(fp, fmt, ap);
    }
}

void DBG_panic_func(char *fmt, ...)
{
    va_list     ap;

    va_start(ap, fmt);
    initialize_debug_write_fp();
    panic_func(st_current_controller->debug_write_fp,
               st_current_file_name, st_current_line, fmt, ap);
    panic_func(stderr, st_current_file_name, st_current_line, fmt, ap);
    va_end(ap);
    abort();
}

/*
  根据debug的等级来确定是否要输出
*/
void DBG_debug_write_func(int level, char *fmt, ...)
{
    va_list     ap;

    if (level > 0 && level > st_current_controller->current_debug_level) {
      /*等级不够*/
        return;
    }
    va_start(ap, fmt);
    initialize_debug_write_fp();
    if (fmt) {
        vfprintf(st_current_controller->debug_write_fp, fmt, ap);
    }
    va_end(ap);
}

/*
  设置controller
*/
void DBG_set(DBG_Controller controller, char *file, int line)
{
    st_current_controller = controller;
    st_current_file_name = file;
    st_current_line = line;
}

/*
  设置
*/
void DBG_set_expression(char *expression)
{
    st_assert_expression = expression;
}
