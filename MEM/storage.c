#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "memory.h"

/*
  一个内存单位
*/
typedef union {
    long        l_dummy;
    double      d_dummy;
    void        *p_dummy;
} Cell;

/*
内存单位的大小
*/
#define CELL_SIZE               (sizeof(Cell))
/*
默认的memorypage的cell的数量
*/
#define DEFAULT_PAGE_SIZE       (1024)  /* cell num */

typedef struct MemoryPage_tag MemoryPage;
/*
  memory page list 指向memory page
*/
typedef MemoryPage *MemoryPageList;

struct MemoryPage_tag {
  /*总共拥有的内存单元的数量*/
    int                 cell_num;
  /*已经使用的内存单元的数量*/
    int                 use_cell_num;
  /*下一个memorypage*/
    MemoryPageList      next;
  
    Cell                cell[1];
};

/*
  解释器的内存管理模块
*/
struct MEM_Storage_tag {
  /*
    page_list是头结点，通过它可以找到所有的MemoryPage
    current_page_size指的是MemoryPage的要分配的cell个数
   */
    MemoryPageList      page_list;
    int                 current_page_size;
};

#define larger(a, b) (((a) > (b)) ? (a) : (b))

/*
  新建一个storage，用于解释器的内存管理
*/
MEM_Storage MEM_open_storage_func(MEM_Controller controller,
                      char *filename, int line, int page_size)
{
    MEM_Storage storage;

    storage = MEM_malloc_func(controller, filename, line,
                              sizeof(struct MEM_Storage_tag));
    storage->page_list = NULL;
    assert(page_size >= 0);
    if (page_size > 0) {
        storage->current_page_size = page_size;
    } else {
        storage->current_page_size = DEFAULT_PAGE_SIZE;
    }

    return storage;
}

void* MEM_storage_malloc_func(MEM_Controller controller,
                        char *filename, int line, MEM_Storage storage,
                        size_t size)
{
    int                 cell_num;
    MemoryPage          *new_page;
    void                *p;

    /*
      所需要的内存单元的个数
     */
    cell_num = ((size - 1) / CELL_SIZE) + 1;

    /*
      如果storage的当前memorypage不为空，而且page内的cell的数量够用
     */
    if (storage->page_list != NULL
        && (storage->page_list->use_cell_num + cell_num
            < storage->page_list->cell_num)) {
      /*将可以使用的开始地址赋值给p*/
        p = &(storage->page_list->cell[storage->page_list->use_cell_num]);
        storage->page_list->use_cell_num += cell_num;
    } else {
      /*如果没有当前的memorypage，或者当前的memorypage的cell不够用*/
        int     alloc_cell_num;

        alloc_cell_num = larger(cell_num, storage->current_page_size);

        new_page = MEM_malloc_func(controller, filename, line,
                                   sizeof(MemoryPage)
                                   + CELL_SIZE * (alloc_cell_num - 1));
        /*新的memorypage插入page链表的头部*/
        new_page->next = storage->page_list;
        new_page->cell_num = alloc_cell_num;
        storage->page_list = new_page;

        p = &(new_page->cell[0]);
        new_page->use_cell_num = cell_num;
    }

    return p;
}

void MEM_dispose_storage_func(MEM_Controller controller, MEM_Storage storage)
{
    MemoryPage  *temp;

    /*
      将storage中的page链表的头部循环回收
     */
    while (storage->page_list) {
        temp = storage->page_list->next;
        MEM_free_func(controller, storage->page_list);
        storage->page_list = temp;
    }
    /*最后再回收整个storage*/
    MEM_free_func(controller, storage);
}
