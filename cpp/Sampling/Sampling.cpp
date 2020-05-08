#include "Sampling.h"
using namespace std;
#include "cmyglobal.h"

extern CMyGlobal MyGlobal;
void CSampling::StartSampling()//创建采样线程
{
//    Linux内核的三种调度策略：
//    1，SCHED_OTHER 分时调度策略，
//    2，SCHED_FIFO实时调度策略，先到先服务。一旦占用cpu则一直运行。一直运行直到有更高优先级任务到达或自己放弃
//    3，SCHED_RR实时调度策略，时间片轮转。当进程的时间片用完，系统将重新分配时间片，并置于就绪队列尾。放在队列尾保证了所有具有相同优先级的RR任务的调度公平
//    Linux线程优先级设置
//    首先，可以通过以下两个函数来获得线程可以设置的最高和最低优先级，函数中的策略即上述三种策略的宏定义：
//    int sched_get_priority_max(int policy);
//    int sched_get_priority_min(int policy);
//    SCHED_OTHER是不支持优先级使用的，而SCHED_FIFO和SCHED_RR支持优先级的使用，他们分别为1和99，数值越大优先级越高。

    pthread_t SampID,ProcessID1,ProcessID2,ProcessID3,ProcessID4,MonitorID,ProcessID5,ProcessID6;

    struct sched_param param;

    pthread_attr_t attr1,attr2,attr3;

    pthread_attr_init(&attr1);
    pthread_attr_init(&attr2);
    pthread_attr_init(&attr3);

    param.sched_priority = 99;
    pthread_attr_setschedpolicy(&attr1,SCHED_FIFO);
    pthread_attr_setschedparam(&attr1,&param);
    pthread_attr_setinheritsched(&attr1,PTHREAD_EXPLICIT_SCHED);//要使优先级其作用必须要有这句话

    param.sched_priority = 80;
    pthread_attr_setschedpolicy(&attr2,SCHED_FIFO);
    pthread_attr_setschedparam(&attr2,&param);
    pthread_attr_setinheritsched(&attr2,PTHREAD_EXPLICIT_SCHED);

    param.sched_priority = 70;
    pthread_attr_setschedpolicy(&attr3,SCHED_RR);
    pthread_attr_setschedparam(&attr3,&param);
    pthread_attr_setinheritsched(&attr3,PTHREAD_EXPLICIT_SCHED);


    //1实时线程：优先级 99 -- 98 -- 97
    pthread_create(&SampID,&attr1,SamplingThread,(void*)this);
    pthread_create(&ProcessID1,&attr2,ProcessingThread1,(void*)this);//视频
    pthread_create(&ProcessID2,NULL,ProcessingThread2,(void*)this);//音频

    //非实时线程：报文-- NTP&SNMP -- 上位机下位机通信 -- 内存监视 -- 其他线程
    pthread_create(&ProcessID3,NULL,ProcessingThread3,(void*)this);//报文
    pthread_create(&ProcessID4,NULL,ProcessingThread4,(void*)this);//NTP服务
    pthread_create(&ProcessID5,NULL,ProcessingThread5,(void*)this);//上位机与下位机通信模块
    pthread_create(&ProcessID6,NULL,ProcessingThread6,(void*)this);//电台服务（都需要定时发送）
    pthread_create(&MonitorID,NULL,BufferMonitorThread,(void*)this);//内存监视服务
}

void CSampling::StopSampling()
{
	m_StopSamp = 1;
    while(!(m_isSampThreadEnd && m_isProcessThread1End && m_isProcessThread2End && m_isProcessThread3End && m_isProcessThread4End && m_isProcessThread5End && m_isProcessThread6End && m_isMonitorThreadEnd) )
        sleep(1);
}
//0采集线程
void* CSampling::SamplingThread(void* lpParam)//采集网卡监听数据，保存到缓存中
{
	CSampling* pSamp = (CSampling*)lpParam;
    BYTE PacketBuffer[5000];
    int PacketLen;

    struct timeval timevalue;

    CBufferManagement::_tPackMsg PackMsg;
    int EtherToUdpLen = ProtoType::GetEtherToUDPLen();

    //<2.从数据库中读取策略  到策略变量>
    MyGlobal.m_Strategy.ReadSamplingPolicyFromDB();
    MyGlobal.m_Strategy.ReadStoragePolicyFromDB();


    BYTE SampStrategy = MyGlobal.m_Strategy.m_SamplingStrategy;
    //上位机设置停止采集参数 MyGlobal.m_Strategy.m_IsSamplingStoped;
    bool UpperSetSampStop = MyGlobal.m_Strategy.m_IsSamplingStoped;
	//<!初始化策略变量完成>

    //开始采集
    while(!pSamp->m_StopSamp)
	{
        //更新采集策、上位机发送的停止采集变量
        SampStrategy = MyGlobal.m_Strategy.m_SamplingStrategy;
        UpperSetSampStop = MyGlobal.m_Strategy.m_IsSamplingStoped;

        //阻塞式采集socket数据
        pSamp->m_GetPacket.GetPacket(PacketBuffer,&PacketLen);
        if(PacketLen>0)
        {
            pSamp->m_DataRecvCnt += PacketLen;
            //1--UDP报文
            if(ProtoType::IsUDP(PacketBuffer,PacketLen))
            {
                //get system local tim
                timevalue = CBufferManagement::GetLocalTime();

                //1.上位机与下位机通信类
                if(ProtoType::IsUpperToLowerCmd(PacketBuffer,PacketLen))
                {
                    PackMsg.SrcIP = ProtoType::GetSrcIP(PacketBuffer,PacketLen);
                    PackMsg.DestIP = ProtoType::GetDestIP(PacketBuffer,PacketLen);
                    PackMsg.Type = 1;
                    PackMsg.Length = PacketLen - EtherToUdpLen;
                    PackMsg.tv = timevalue;

                    //获取数据与辅助信息
                    pSamp->m_UpperLowerCom.Add(PacketBuffer + EtherToUdpLen, PacketLen - EtherToUdpLen);
                    if(!pSamp->m_UpperLowerCom.IsFull())
                        pSamp->m_UpperLowerCom.AddMsg(PackMsg);
                }
                
            }
		}
	}

    pSamp->m_isSampThreadEnd = 1;
    cout<<"SamplingThread Exit"<<endl;
    return 0;
}


void* CSampling::ProcessingThread3(void* lpParam)//从缓存中获取数据，解码并分类保存
{
    CSampling* pSamp = (CSampling*)lpParam;
    BYTE TmpBuf[5000];//解码用的缓冲区
    pSamp->m_Msg.m_CmdCtlCom.SendConfigToModule();

    while(!pSamp->m_StopSamp)
    {
        //1<报文解码模块>
        pSamp->m_Msg.Processing(TmpBuf,5000);
        //<!--报文解码模块>
        usleep(100);
    }
    pSamp->m_isProcessThread3End = 1;
    cout<<"ProcessingThread3 Exit"<<endl;
    return 0;
}

//5上位机与下位机通信模块
void* CSampling::ProcessingThread5(void* lpParam)
{
	CSampling* pSamp = (CSampling*)lpParam;

    BYTE TmpBuf[5000];//解码用的缓冲区

    while(!pSamp->m_StopSamp)
    {
        //2<上位机-下位机通信>
        pSamp->m_UpperLowerCom.Processing(TmpBuf,5000);

        usleep(1000);
	}
    pSamp->m_isProcessThread5End = 1;
    cout<<"ProcessingThread5 Exit"<<endl;
    return 0;
}


//内存监视模块
void* CSampling::BufferMonitorThread(void* lpParam)
{
    CSampling* pSamp = (CSampling*)lpParam;

    UINT dataLen[3];
    UINT TotalLen[3];
    while(!pSamp->m_StopSamp)
    {
        cout<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
        //1 采集速度
        cout<<"采集速度:"<<pSamp->m_DataRecvCnt*1.0/1024<<"KB/s"<<endl;
        pSamp->m_DataRecvCnt = 0;

        //4 Msg缓冲区使用情况
        dataLen[2] = pSamp->m_Msg.GetDataLen_Data();
        TotalLen[2] = pSamp->m_Msg.GetCapacity_Data();
        cout<<"报文缓冲区："<<dataLen[2]<<"("<<(float)(dataLen[2]*1.0/TotalLen[2])<<")"<<endl;
        cout<<endl;

        cout<<"----------------------------------------------------------"<<endl;


        if(0 == dataLen[0] && 0 == dataLen[1] && 0 == dataLen[2])
            MyGlobal.m_IsMsgVoiceVideoBufferEmpty = 1;
        else
            MyGlobal.m_IsMsgVoiceVideoBufferEmpty = 0;


        cout<<endl;
        sleep(1);
    }
    pSamp->m_isMonitorThreadEnd = 1;

    cout<<"BufferMonitorThread Exit"<<endl;
    return 0;
}










