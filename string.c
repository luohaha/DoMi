#include <stdio.h>
#include <string.h>
#include "MEM.h"
#include "domi.h"

/*每次拓展容量时，增加的大小*/
#define STRING_ALLOC_SIZE       (256)

/*
string的地址
*/
static char *st_string_literal_buffer = NULL;
/*当前已经使用*/
static int st_string_literal_buffer_size = 0;
/*当前分配*/
static int st_string_literal_buffer_alloc_size = 0;

/*
初始化string
*/
void dm_open_string_literal(void)
{
    st_string_literal_buffer_size = 0;
}

/*
添加letter,
*/
void dm_add_string_literal(char letter)
{
  /*
    当已经使用的内存，达到已经分配的大小时，在来分配更多
   */
    if (st_string_literal_buffer_size == st_string_literal_buffer_alloc_size) {
        st_string_literal_buffer_alloc_size += STRING_ALLOC_SIZE;
        st_string_literal_buffer
            = MEM_realloc(st_string_literal_buffer,
                          st_string_literal_buffer_alloc_size);
    }
    st_string_literal_buffer[st_string_literal_buffer_size] = letter;
    st_string_literal_buffer_size++;
}

/*
清空string
*/
void dm_reset_string_literal_buffer(void)
{
    MEM_free(st_string_literal_buffer);
    st_string_literal_buffer = NULL;
    st_string_literal_buffer_size = 0;
    st_string_literal_buffer_alloc_size = 0;
}

/*
将string转移到新的地址中，形成新的string
*/
char *dm_close_string_literal(void)
{
    char *new_str;

    new_str = dm_malloc(st_string_literal_buffer_size + 1);

    memcpy(new_str, st_string_literal_buffer, st_string_literal_buffer_size);
    new_str[st_string_literal_buffer_size] = '\0';

    return new_str;
}

char *dm_create_identifier(char *str)
{
    char *new_str;

    new_str = dm_malloc(strlen(str) + 1);


    strcpy(new_str, str);

    return new_str;
}
