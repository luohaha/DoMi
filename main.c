#include "domi.h"
#include "y.tab.h"
/*
  author : Yixin Luo
  主函数
*/
Manager *manager;
Manager *whileManager;
ManagerLink *managerLink;
int main(int argc, char **argv) {
  FILE *fp;
  extern int yyparse(void);
  extern FILE *yyin;
  //获取全局变量链表的头部
  manager = (Manager*)malloc(sizeof(Manager));
  whileManager = (Manager*)malloc(sizeof(Manager));
  manager->varhead = init_var();
  manager->baghead = init_bag();
  whileManager->baghead = init_bag();
  /*
    函数的链表定义
   */
  managerLink = (ManagerLink*)malloc(sizeof(ManagerLink));
  managerLink->manager = NULL;
  managerLink->next = NULL;
  /*
    读取文件
   */
  
  if(argc != 2){
    fprintf(stderr,"usage:%s filename",argv[0]);
    exit(1);
  }
  fp = fopen(argv[1],"r");
  if(fp==NULL){
    fprintf(stderr,"%s not found.\n",argv[1]);
    exit(1);
  }
  yyin = fp;
  
  if (yyparse()) {
    fprintf(stderr, "parsing err!\n");
    exit(1);
  }
  exeBagLink(manager->baghead);
  
  return 0;
}
