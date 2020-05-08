
#include "UpperLowerCom.h"
int CUpperLowerCom::Processing(BYTE* pBuf,UINT len)
{
    if(GetDataLen_Msg() == 0)//如果缓冲区没有数据
        return -1;
    else
    {
        CBufferManagement::PackMessage PackMsg;
        ReadMsg(&PackMsg);//读取信息缓冲区一个PackMessage
        if(PackMsg.Length > len)//缓冲区不足，不能够提取出m_pComBuf的数据
            return -2;
        Read(pBuf,PackMsg.Length);//读出一帧对应长度的数据到pBuf中去


        //*********************************************************
        BYTE cmd;
        cmd = ProtoType::GetUpperLowerCmd_Cmd(pBuf);
        BYTE cmdData[50];
        ProtoType::GetUpperLowerCmd_Data(cmdData,pBuf);
        //*********************************************************
        UINT IP4;
        BYTE tmpIP[4];
        char UpperIP[50];
        char SetLocalIP[50];
        char SetLocalMask[50];
        char SetLocalGateway[50];
        UINT* pTime;

        switch(cmd)
        {
            //1:查询本地IP(0xA8--0xA9) && 获取硬盘使用情况(0xA2--0xA3) &&
            case 0xA8:

                IP4 = PackMsg.SrcIP;
                tmpIP[0] = (IP4>>24)&0xff;
                tmpIP[1] = (IP4>>16)&0xff;
                tmpIP[2] = (IP4>>8)&0xff;
                tmpIP[3] = (IP4)&0xff;
                sprintf(UpperIP,"%d.%d.%d.%d",tmpIP[3],tmpIP[2],tmpIP[1],tmpIP[0]);
                UploadLocalIP_DiskInfo(UpperIP);

            break;

            //2:修改本地IP(0xAA--0xAB)
            case 0xAA:
                //设置本地IP
                sprintf(SetLocalIP,"%d.%d.%d.%d",cmdData[0],cmdData[1],cmdData[2],cmdData[3]);
                sprintf(SetLocalMask,"%d.%d.%d.%d",cmdData[4],cmdData[5],cmdData[6],cmdData[7]);
                sprintf(SetLocalGateway,"%d.%d.%d.%d",cmdData[8],cmdData[9],cmdData[10],cmdData[11]);

                //获取远程IP  PORT
                IP4 = PackMsg.SrcIP;
                tmpIP[0] = (IP4>>24)&0xff;
                tmpIP[1] = (IP4>>16)&0xff;
                tmpIP[2] = (IP4>>8)&0xff;
                tmpIP[3] = (IP4)&0xff;
                sprintf(UpperIP,"%d.%d.%d.%d",tmpIP[3],tmpIP[2],tmpIP[1],tmpIP[0]);
                ConfigLocalIP(SetLocalIP,SetLocalMask,SetLocalGateway,UpperIP);
                break;

            

            //6.停止采集
            case 0xAE:
                //获取远程IP  PORT
                IP4 = PackMsg.SrcIP;
                tmpIP[0] = (IP4>>24)&0xff;
                tmpIP[1] = (IP4>>16)&0xff;
                tmpIP[2] = (IP4>>8)&0xff;
                tmpIP[3] = (IP4)&0xff;
                sprintf(UpperIP,"%d.%d.%d.%d",tmpIP[3],tmpIP[2],tmpIP[1],tmpIP[0]);

                if(1 == cmdData[0])//停止采集
                {
                    SetSamplingStop(1,UpperIP);
                }
                else if(2 == cmdData[0])//恢复采集
                {
                    SetSamplingStop(0,UpperIP);
                }

            break;

           

            default:
                break;
        }
        return 1;

    }
}

char* CUpperLowerCom::shellcmd(char* cmd, char* buff, int size)
{
  char temp[256];
  FILE* fp = NULL;
  int offset = 0;
  int len;

  fp = popen(cmd, "r");
  if(fp == NULL)
  {
    return NULL;
  }

  while(fgets(temp, sizeof(temp), fp) != NULL)
  {
    len = strlen(temp);
    if(offset + len < size)
    {
      strcpy(buff+offset, temp);
      offset += len;
    }
    else
    {
      buff[offset] = 0;
      break;
    }
  }

  if(fp != NULL)
  {
    pclose(fp);
  }

  return buff;
}

int CUpperLowerCom::FindStrBetweenBlanks(const char* pSrc, char pDest[][100] )
{
    string strSrc = pSrc;
    string strDest;
    int len = strlen(pSrc);
    int i =0;
    int cnt = 0;
    char xx;
    while(i<len)
    {
        xx = strSrc.at(i);
        if(' ' != strSrc.at(i))
        {
            strDest += strSrc.at(i);
        }
        else
        {
            if(strDest.length() > 0)
            {
                strcpy(pDest[cnt],strDest.data());
                strDest.clear();
                cnt++;
            }
        }
        i++;
    }
    return cnt;
}

void CUpperLowerCom::GetDiskInfo(const char* path,uint64_t* total, uint64_t* used, uint64_t* avalible)
{
    struct stat s_buf;
    stat(path,&s_buf);
    /*判断输入的文件路径是否目录，若是目录，则往下执行，分析目录下的文件*/
    if(!S_ISDIR(s_buf.st_mode))
        return ;

    char buf[300];
    memset(buf,0,300);
    char cmdTmp[100];
    sprintf(cmdTmp,"df %s",path);
    shellcmd(cmdTmp,buf,300);

    //0文件系统        1容量  2已用  3可用 4已用% 5挂载点
    char bufsep[6][100];

    string strin = buf;
    int pos0 = strin.find('\n');
    FindStrBetweenBlanks(buf+pos0,bufsep);
    *total = atol(bufsep[1]);
    *used = atol(bufsep[2]);
    *avalible = atol(bufsep[3]);

}

void CUpperLowerCom::UploadDiskInfo(char* pUpperIP,char* pPath)
{
    uint64_t KTotal = 0,KUsed = 0,KAvailable = 0;
    CUpperLowerCom::GetDiskInfo(pPath,&KTotal,&KUsed,&KAvailable);

    ProtoType::UPPERLOWERCMD cmdSend;
    memset(&cmdSend,0,sizeof(cmdSend));
    BYTE cmdData[50];
    memset(cmdData,0,50);
    uint64_t* pKTotoalPos = (uint64_t*)cmdData;
    uint64_t* pKUsedPos = pKTotoalPos + sizeof(uint64_t);
    uint64_t* pKAvailablePos = pKUsedPos + sizeof(uint64_t);

    *pKTotoalPos = KTotal;
    *pKUsedPos   = KUsed;
    *pKAvailablePos = KAvailable;

    ProtoType::SetLowerToUpperCmd(&cmdSend,0xA3,(BYTE*)cmdData,50);//IP:可以不加，IP协议头信息已经包含源IP

    m_Tcp_Udp.SendUDP(pUpperIP,6666,(const BYTE*)&cmdSend,sizeof(ProtoType::UPPERLOWERCMD));
    //printf("回复查询:%x %x,%x %x, %x,%d %d %d %d\n",cmdSend.DestID[1],cmdSend.DestID[0],cmdSend.SrcID[1],cmdSend.SrcID[0],cmdSend.Cmd,cmdSend.Data[0],cmdSend.Data[1],cmdSend.Data[2],cmdSend.Data[3]);
    cout<<"已回复硬盘使用信息命令"<<endl;

}


void CUpperLowerCom::ConfigLocalIP(char* pSetIP,char* pSetMask, char* pSetGateway, char*pUpperIP)
{
    char ip[50];
    char mask[50];
    char ethname[50];
    int IpModifySucceed = 0;

    ProtoType::UPPERLOWERCMD cmdSend;
    //如果IP与本机相同，则不修改
    GetLocalIP(ethname,ip,mask);
    //如果本机IP与设置IP不相同
    if(strcmp(pSetIP,ip) != 0 || strcmp(pSetMask,mask) != 0)
    {
        //1设置本地IP
        SetIPandMask(pSetIP,pSetMask,pSetGateway);

        //2等待IP设置是否完成
        int cnt2= 0 ;
        while(cnt2++<20)//15秒
        {
            sleep(1);
            if(GetLocalIP(ethname,ip,mask))//获取本地IP成功
            {
                //对比IP是否修改完成
                if(strcmp(pSetIP,ip) == 0)
                {
                    IpModifySucceed = 1;
                    break;
                }
                else
                    IpModifySucceed = 0;
            }
        }
    }
    else
        IpModifySucceed = 1;

    //3设置本机对应的SNMP服务器IP
    if(strcmp(pSetIP,"10.28.2.4") == 0)
    {
        m_TargetRadioIP = "10.28.2.5";
    }
    else if(strcmp(pSetIP,"10.28.1.4") == 0)
    {
        m_TargetRadioIP = "10.28.1.5";
    }

    //4回复上位机命令
    BYTE cmdData[20];
    memset(cmdData,0,20);
    if(IpModifySucceed)//如果修改成功
    {
        cmdData[0]= 0x01;
        ProtoType::SetLowerToUpperCmd(&cmdSend,0xAB,(BYTE*)cmdData,1);//IP:可以不加，IP协议头信息已经包含源IP
    }
    else//修改失败
    {
        cmdData[0]= 0x02;
        ProtoType::SetLowerToUpperCmd(&cmdSend,0xAB,(BYTE*)cmdData,1);//IP:可以不加，IP协议头信息已经包含源IP
    }

    m_Tcp_Udp.SendUDP(pUpperIP,6666,(const BYTE*)&cmdSend,sizeof(ProtoType::UPPERLOWERCMD));
    cout<<"回复修改IP命令"<<endl;
}

bool CUpperLowerCom::SetPromisc()
{
    char localip[50];
    char localname[50];
    char localmask[50];
    GetLocalIP(localname,localip,localmask);
    char cmd[100];
    sprintf(cmd,"/sbin/ifconfig %s promisc",localname);
    system(cmd);
    return true;
}

bool CUpperLowerCom::GetLocalIP(char* ethName,char* ipout,char* mask)
{
    struct ifaddrs *ifc, *ifc1;
    if(0 != getifaddrs(&ifc)) return false;
    ifc1 = ifc;

    //printf("iface\tIP address\tNetmask\n");
    for(; NULL != ifc; ifc = (*ifc).ifa_next)
    {
        //printf("%s", (*ifc).ifa_name);
        if(strcmp((*ifc).ifa_name,"lo") != 0 && (*ifc).ifa_addr->sa_family == AF_INET)
        {
            strcpy(ethName,(*ifc).ifa_name);
            if(NULL != (*ifc).ifa_addr)
            {
                inet_ntop(AF_INET, &(((struct sockaddr_in*)((*ifc).ifa_addr))->sin_addr), ipout, 64);

            }
            if(NULL != (*ifc).ifa_netmask)
            {
                inet_ntop(AF_INET, &(((struct sockaddr_in*)((*ifc).ifa_netmask))->sin_addr), mask, 64);
            }
            freeifaddrs(ifc1);
            return true;
        }
    }

    return false;
}

//"192.168.1.1"-->192(ipOut[3])   168(ipOut[2])   1(1)  1(0)
void CUpperLowerCom::StrIPToNumIP(const char* ipIn,BYTE* ipOut)
{
    string strip = (char*)ipIn;
    int pos0 = 0,pos1 = 0;
    int i=0;
    while(i<3)
    {
        pos1 = strip.find('.',pos0);
        ipOut[i++] = atoi(strip.substr(pos0,pos1).data());
        pos0 = pos1+1;
    }
    ipOut[i] = atoi(strip.substr(pos0).data());
}

// 设置本机IP地址
bool CUpperLowerCom::SetIPandMask(const char* ip, const char* mask, const char* gateway)
{
    char localip[50];
    char localname[50];
    char localmask[50];
    GetLocalIP(localname,localip,localmask);
    char cmd[200];
    sprintf(cmd,"echo auto %s > /etc/network/interfaces ",localname);
    system(cmd);
    sprintf(cmd,"echo iface %s inet static >> /etc/network/interfaces ",localname);
    system(cmd);
    sprintf(cmd,"echo address %s >> /etc/network/interfaces ",ip);
    system(cmd);
    sprintf(cmd,"echo netmask %s >> /etc/network/interfaces ",mask);
    system(cmd);
    sprintf(cmd,"echo gateway %s >> /etc/network/interfaces ",gateway);
    system(cmd);


    system("/etc/init.d/networking restart");
    sleep(1);
    sprintf(cmd,"/sbin/ifconfig %s %s netmask %s",localname,ip,mask);
    system(cmd);
    sleep(1);

    return true;
}





