#pragma once
#include "MyHeader.h"
#include "BufferManagement.h"
#include "ProtoType.h"
#include "TCP_UDP.h"
#include "Strategy.h"
#include "ExportToUDisk.h"
#include "cmyglobal.h"

class CUpperLowerCom : public CBufferManagement
{
public:
    CUpperLowerCom(void): CBufferManagement(10000000)
    {
       
        char ip[50];
        char mask[50];
        char ethname[50];
        GetLocalIP(ethname,ip,mask);
        if(strcmp(ip,"10.28.2.4") == 0)
        {
            m_TargetRadioIP = "10.28.2.5";
        }
        else if(strcmp(ip,"10.28.1.4") == 0)
        {
            m_TargetRadioIP = "10.28.1.5";
        }

        m_TargetNTPServerIP = "10.28.1.2";
	}

	~CUpperLowerCom(void)
    {

	}

    //处理线程循环调用，用于上下位机数据的编解码
    int Processing(BYTE* pBuf,UINT len);//pBuf为外部提供的缓冲区，len 为缓冲区大小，以防循环调用不断销毁创建缓冲区

public:

    //1.设置IP，获取IP，获取硬盘使用信息
    void ConfigLocalIP(char* pSetIP,char* pSetMask, char* pSetGateway, char*pUpperIP);
    void UploadLocalIP_DiskInfo(char* pUpperIP);
    void UploadDiskInfo(char* pUpperIP,char* pPath);

    
    void UpdateSamplingPolicy(char* pUpperIP,BYTE* pData)
    {
        //返回上位机信息
        ProtoType::UPPERLOWERCMD cmdSend;
        memset(&cmdSend,0,sizeof(cmdSend));
        BYTE cmdData[50];
        memset(cmdData,0,50);
        //如果设置采集策略成功
        if(ret == 1)
            cmdData[0] = 1;
        else
            cmdData[0] = 2;

        ProtoType::SetLowerToUpperCmd(&cmdSend,0xA5,(BYTE*)cmdData,sizeof(cmdData));//IP:可以不加，IP协议头信息已经包含源IP
        m_Tcp_Udp.SendUDP(pUpperIP,6666,(const BYTE*)&cmdSend,sizeof(ProtoType::UPPERLOWERCMD));
        cout<<"已回复更新采集策略命令"<<endl;
    }

    
    //7.设置停止采集,10s等待缓冲区空
    void SetSamplingStop(bool tf,char* pUpperIP)
    {
        BYTE cmdData[50];
        memset(cmdData,0,50);
        ProtoType::UPPERLOWERCMD cmdSend;
        memset(&cmdSend,0,sizeof(cmdSend));

        MyGlobal.m_Strategy.m_IsSamplingStoped = tf;
        if(tf)//如果是停止采集
        {
            int i=0;
            while(i++ < 10)
            {
                if(!MyGlobal.m_IsMsgVoiceVideoBufferEmpty)
                {
                    sleep(1);
                }
                else
                    break;
            }

            //返回上位机信息
            if(MyGlobal.m_IsMsgVoiceVideoBufferEmpty)
            {
                cmdData[0] = 1;
            }
            else
            {
                cmdData[0] = 2;
            }
        }
        //如果是恢复采集
        else
            cmdData[0] = 1;

        ProtoType::SetLowerToUpperCmd(&cmdSend,0xAF,(BYTE*)cmdData,sizeof(cmdData));//IP:可以不加，IP协议头信息已经包含源IP
        m_Tcp_Udp.SendUDP(pUpperIP,6666,(const BYTE*)&cmdSend,sizeof(ProtoType::UPPERLOWERCMD));
    }

   

private:

    //获取硬盘信息
    static char* shellcmd(char* cmd, char* buff, int size);
    static int FindStrBetweenBlanks(const char* pSrc, char pDest[][100] );
    static void GetDiskInfo(const char* path,uint64_t* total, uint64_t* used, uint64_t* avalible);


public:
    static bool SetPromisc();
    static bool GetLocalIP(char* ethName,char* ipout,char* mask);
    //"192.168.1.1"-->192(ipOut[3])   168(ipOut[2])   1(1)  1(0)
    static void StrIPToNumIP(const char* ipIn,BYTE* ipOut);
    // 设置本机IP地址
    static bool SetIPandMask(const char* ip, const char* mask,const char* gateway);


private:
	CTCP_UDP  m_Tcp_Udp;

public:
    string m_TargetNTPServerIP;
    string m_TargetRadioIP;

private:

    //usb受控导出
    CExportToUDisk m_ExportToUDisk;


};

