#include "MyHeader.h"
#include "cgetpacket.h"
#include "ProtoType.h"
#include "Sampling.h"
#include "BufferManagement.h"
#include "CCmdCtlCom.h"
#include "Message.h"
#include "crecord.h"
#include "DataCo.h"
#include "Strategy.h"
#include "cmyglobal.h"
#include "ExportToUDisk.h"
#include "Voice.h"

//全局对象，包括记录模块、串口通信模块、策略模块、缓冲区是否为空
CMyGlobal MyGlobal;



//1 启动jmRcorder音频服务    c2 报文服务-----------------------------------------------------------
//system("/home/jw/jw_zqd/jmRcorder &");
//sleep(3);
//system("/home/jw/jw_zqd/c2 &");
//sleep(3);


int main(void)
{
    //1 设置printf缓冲区为0
    setvbuf(stdout,(char*)NULL,_IONBF,0);

    //2 初始化串口、握手，设置镜像，取消告警，关闭串口-----------------------------------------
    CExportToUDisk::DeviceSelfCheckAndInit();

    //3 设置网卡为混杂模式----------------------------------------------------------------
    CUpperLowerCom::SetPromisc();

    //4 设置本进程优先级为RT--------------------------------------------------------------
    int xx = syscall(SYS_gettid);
    char buf[400];
    sprintf(buf,"renice -n -20 %d",xx);
    system(buf);
    sprintf(buf,"chrt -p -r 99 %d",xx);
    system(buf);

    //5 开始采集线程--------------------------------------------------------------------
    CSampling x;
    x.StartSampling();

    //6 启动记录模块线程-----------------------------------------------------------------
    MyGlobal.m_Record.StartRecordThread();

    //7 等待退出quit输入----------------------------------------------------------------
    string str;
    while(str != "quit")
        cin>>str;
    return 0;

}











