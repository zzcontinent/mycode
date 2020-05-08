
#include "ProtoType.h"
namespace ProtoType
{
	int GetEtherToUDPLen()
	{
		return  sizeof(ETHERNET_HEADER) + sizeof(IP_HEADER) + sizeof(UDP_HEADER);
	}
	int GetEtherToTCPLen()
	{
		return  sizeof(ETHERNET_HEADER) + sizeof(IP_HEADER) + sizeof(TCP_HEADER);
	}
	int GetEtherToIPLen()
	{
		return  sizeof(ETHERNET_HEADER) + sizeof(IP_HEADER);
	}
	int GetEtherLen()
	{
		return  sizeof(ETHERNET_HEADER);
	}

	bool IsArp(const BYTE* pBuf,UINT len)//从以太网帧判断
	{
		if(len < sizeof(ETHERNET_HEADER))
			return 0;
		else
		{
			ETHERNET_HEADER* pEthHeader = (ETHERNET_HEADER*)pBuf;
			if(::ntohs(pEthHeader->typeOrLen) == 0x0806)
				return 1;
			else 
				return 0;
		}
	}

    bool IsIP(const BYTE* pBuf,UINT len)
    {
        if(len < sizeof(ETHERNET_HEADER))
            return 0;
        else
        {
            ETHERNET_HEADER* pEthHeader = (ETHERNET_HEADER*)pBuf;
            if(::ntohs(pEthHeader->typeOrLen) == 0x0800)
                return 1;
            else
                return 0;
        }
    }

	bool IsICMP(const BYTE* pBuf,UINT len)
	{
		return 0;
	}

	bool IsTCP(const BYTE* pBuf,UINT len)
	{
		if(len < sizeof(ETHERNET_HEADER) + sizeof(IP_HEADER))
			return 0;
        if(IsIP(pBuf,len))//非ARP类型
		{
			IP_HEADER* pIpheader = (IP_HEADER*) (pBuf + sizeof(ETHERNET_HEADER));
			if(pIpheader->proto == 6)
				return 1;
			else 
				return 0;
		}
		else //返回
			return 0;
	}

	bool IsUDP(const BYTE* pBuf,UINT len)
	{
		if(len < sizeof(ETHERNET_HEADER) + sizeof(IP_HEADER))
			return 0;
        if(IsIP(pBuf,len))//IP类型
		{
			IP_HEADER* pIpheader = (IP_HEADER*) (pBuf + sizeof(ETHERNET_HEADER));
			if(pIpheader->proto == 17)
				return 1;
			else 
				return 0;
		}
		else //返回
			return 0;
	}

    //只接收应答报文HTTP协议
    bool IsHTTP(const BYTE* pBuf,UINT len)
    {
        int HdLen = GetEtherToTCPLen();
        string strHttp = (char*)(pBuf+HdLen);
        int pos0 = 0,pos1 = 0;
        if(IsTCP(pBuf,len))
        {
            //"GET /0/C/C/1_freetourw.jpg HTTP/1.1"
            pos1 = strHttp.find(' ',0);
            string str = strHttp.substr(pos0,pos1-pos0);
            if(str == "GET")
            {
                pos0 = strHttp.find(' ',pos1+1);
                pos1 = strHttp.find("\r\n",0);
                str = strHttp.substr(pos0+1,pos1-pos0-1);

                pos1 = str.find('/',0);
                str = str.substr(0,pos1);
                if(str == "HTTP")
                    return 1;
                else
                    return 0;
            }
            else
                return 0;
        }
        return 0;
    }


	bool IsFTP(const BYTE* pBuf,UINT len)
	{
		return 0;
	}
	bool IsSSH(const BYTE* pBuf,UINT len)
	{
		return 0;
	}
    UINT GetDestIP(const BYTE* pBuf,UINT len)
	{
			IP_HEADER* pIpheader = (IP_HEADER*) (pBuf + sizeof(ETHERNET_HEADER));
			return pIpheader->destIP;
	}
    UINT GetSrcIP(const BYTE* pBuf,UINT len)
	{
			IP_HEADER* pIpheader = (IP_HEADER*) (pBuf + sizeof(ETHERNET_HEADER));
			return pIpheader->sourceIP;
	}
	USHORT GetDestPort_UDP(const BYTE* pBuf,UINT len)
	{
			UDP_HEADER* pHd = (UDP_HEADER*) (pBuf + sizeof(ETHERNET_HEADER) + sizeof(IP_HEADER));
			return pHd->uh_dport;
	}
	USHORT GetSrcPort_UDP(const BYTE* pBuf,UINT len)
	{
			UDP_HEADER* pHd = (UDP_HEADER*) (pBuf + sizeof(ETHERNET_HEADER) + sizeof(IP_HEADER));
			return pHd->uh_sport;
	}
    USHORT GetDestPort_TCP(const BYTE* pBuf,UINT len)
    {
            TCP_HEADER* pHd = (TCP_HEADER*) (pBuf + sizeof(ETHERNET_HEADER) + sizeof(IP_HEADER));
            return pHd->th_dport;
    }
    USHORT GetSrcPort_TCP(const BYTE* pBuf,UINT len)
    {
            TCP_HEADER* pHd = (TCP_HEADER*) (pBuf + sizeof(ETHERNET_HEADER) + sizeof(IP_HEADER));
            return pHd->th_sport;
    }


	//<上位机与下位机自定义通信协议解析>
    bool IsUpperToLowerCmd(const BYTE* pBuf,UINT len)
	{
		UPPERLOWERCMD* pHd = (UPPERLOWERCMD*)(pBuf + GetEtherToUDPLen());
		int lenx = GetEtherToUDPLen();
		lenx += sizeof(UPPERLOWERCMD);
		
		if(pHd->DestID[1]==0x52 && pHd->DestID[0]==0x63 && pHd->SrcID[1]==0x56 &&pHd->SrcID[0]==0x65  && len == GetEtherToUDPLen() + sizeof(UPPERLOWERCMD))
				return 1;
			else
				return 0;
	}
	USHORT GetUpperLowerCmd_DestID(const BYTE* pBuf)
	{
		UPPERLOWERCMD* pHd = (UPPERLOWERCMD*)pBuf;
		return pHd->DestID[1]*256 + pHd->DestID[0];
	}
	USHORT GetUpperLowerCmd_SrcID(const BYTE* pBuf)
	{
		UPPERLOWERCMD* pHd = (UPPERLOWERCMD*)pBuf;
		return pHd->SrcID[1]*256 + pHd->SrcID[0];
	}
	BYTE GetUpperLowerCmd_Cmd(const BYTE* pBuf)
	{
		UPPERLOWERCMD* pHd = (UPPERLOWERCMD*)pBuf;
		return pHd->Cmd;
	}
	void GetUpperLowerCmd_Data(BYTE* pDest,const BYTE* pSrc)
	{
		UPPERLOWERCMD* pHd = (UPPERLOWERCMD*)pSrc;
        memcpy(pDest,pHd->Data,50);
	}
    void SetLowerToUpperCmd(UPPERLOWERCMD* pULC,BYTE Cmd,BYTE* pBuf,int len)
	{
		memset(pULC,0,sizeof(UPPERLOWERCMD));
		pULC->DestID[1] = 0x56;
		pULC->DestID[0] = 0x65;
		pULC->SrcID[1] = 0x52;
		pULC->SrcID[0] = 0x63;
		pULC->Cmd = Cmd;
		memcpy(pULC->Data,pBuf,len);
	}
	//<!上位机与下位机自定义通信协议解析>
	
	//<报文类型判断>
    bool IsMsg_PreTrans(const BYTE* pBuf,UINT len)
	{
        UINT IP4 = GetSrcIP(pBuf,len);
        BYTE tmpIP[4];
        tmpIP[0] = (IP4>>24)&0xff;
        tmpIP[1] = (IP4>>16)&0xff;
        tmpIP[2] = (IP4>>8)&0xff;
        tmpIP[3] = (IP4)&0xff;

        //过滤广播包地址，过滤方式:IP最后字节，是否等于(255-mask的最后字节)。
        if(IsBroadCast(pBuf,len,240) || IsBroadCast(pBuf,len,0))
            return 0;

        //过滤
        USHORT SrcPort = ntohs(GetSrcPort_UDP(pBuf,len));
        USHORT DestPort = ntohs(GetDestPort_UDP(pBuf,len));
        //if module send a message to me , save them for m_Msg.Processing() to handle with
        if(127 == tmpIP[3] && 0 == tmpIP[2] && 0 == tmpIP[1] && 1 == tmpIP[0])
        {
            if(5000 == SrcPort && 5005 == DestPort)
                return 1;
            else
                return 0;
        }
        else
            return 1;
	}

    typedef struct
    {
        unsigned short type:3;
        unsigned short reply:1;
        unsigned short pt_len:12;
    } stLenAndType;

	//NTP时间同步类
	bool IsNTP(const BYTE* pBuf,UINT len)
	{
		USHORT SrcPort = ::ntohs(ProtoType::GetSrcPort_UDP(pBuf,len));
		if( SrcPort == 123)
			return 1;
		else
			return 0;
    }
}
