#include <stdio.h>
#include "DM.h"
#include "MEM.h"
#include "domi.h"

int main(int argc, char **argv)
{
    DM_Interpreter     *interpreter;
    FILE *fp;

    if (argc != 2) {
        fprintf(stderr, "usage:%s filename", argv[0]);
        exit(1);
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "%s not found.\n", argv[1]);
        exit(1);
    }
    interpreter = DM_create_interpreter();
    DM_compile(interpreter, fp);
    DM_interpret(interpreter);
    DM_dispose_interpreter(interpreter);

    MEM_dump_blocks(stdout);

    return 0;
}
