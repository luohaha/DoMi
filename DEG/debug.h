#ifndef PRIVATE_DBG_H_INCLUDED
#define PRIVATE_DBG_H_INCLUDED
#include <stdio.h>
#include "DBG.h"

/*
  debug控制器
*/
struct DBG_Controller_tag {
  /*
    debug写入的文件，和debug的等级
   */
    FILE        *debug_write_fp;
    int         current_debug_level;
};

#endif /* PRIVATE_DBG_H_INCLUDED */
