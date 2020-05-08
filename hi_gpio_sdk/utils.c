#include "stdio.h"

int execmd(char* cmd,char* result) {
    char buffer[128];                         //定义缓冲区
    FILE* pipe = popen(cmd, "r");            //打开管道，并执行命令
    if (!pipe)
          return 0;                      //返回0表示运行失败

    while(!feof(pipe)) {
    if(fgets(buffer, 128, pipe)){             //将管道输出到result中
            strcat(result,buffer);
        }
    }
    pclose(pipe);                            //关闭管道
    return 1;                                 //返回1表示运行成功
}
