#include <stdio.h>
#include "CRB.h"
#include "MEM.h"

int
main(int argc, char **argv)
{
    /* 分别是解释器和待读入的源程序 */
    CRB_Interpreter     *interpreter;
    FILE *fp;

    /* 参数检查 */
    if (argc != 2) {
        fprintf(stderr, "usage:%s filename", argv[0]);
        exit(1);
    }

    /* 源程序检查 */
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "%s not found.\n", argv[1]);
        exit(1);
    }

    interpreter = CRB_create_interpreter(); /* 创建解释器 */
    CRB_compile(interpreter, fp);           /* 使用解释器对源程序进行处理 */
    CRB_interpret(interpreter);             /* 执行 */
    CRB_dispose_interpreter(interpreter);   /* 释放 */

    MEM_dump_blocks(stdout);                /* 输出结果 */

    return 0;
}
