#pragma once
#include "MyHeader.h"
#include "MBuffer.h"
class CBufferManagement
{
public:
    typedef struct PackMessage
    {
        UINT  Type;
        UINT  Length;
        UINT  DestIP;
        UINT  SrcIP;
        struct timeval tv;
    }_tPackMsg;

    CBufferManagement(UINT len)
    {
        m_pDataBuf = new CMBuffer<BYTE>(len);
        m_pMsgBuf = new CMBuffer<_tPackMsg>(len/sizeof(_tPackMsg));//暂定最大缓冲区包数据帧少于DataBuf
    }

    virtual ~CBufferManagement(void)
    {
        delete m_pDataBuf;
        delete m_pMsgBuf;
    }

    int Add(BYTE data) //如果缓冲区满，返回-1，否则返回1
    {
        return m_pDataBuf->Add(&data,1);
    }
    int Add(const BYTE* pSrc,UINT len)//返回实际添加的数据长度
    {
        return m_pDataBuf->Add(pSrc,len);
    }
    int Read(BYTE* data)
    {
        return m_pDataBuf->Read(data,1);
    }
    int Read(BYTE* pDest, UINT len)
    {
        return m_pDataBuf->Read(pDest,len);
    }

    UINT GetDataLen_Data()
    {
        return m_pDataBuf->GetDataLen();
    }
    UINT GetDataLen_Msg()
    {
        return m_pMsgBuf->GetDataLen();
    }
	
    UINT GetCapacity_Data()
    {
        return m_pDataBuf->GetCapacity();
    }
    UINT GetCapacity_MSG()
    {
        return m_pMsgBuf->GetCapacity();
    }

    int ReadMsg(_tPackMsg* pMsg)//保存解码后的报文辅助信息
    {
        if(m_pMsgBuf->Read(pMsg,1)==-1)
            return 0;
        return 1;
    }
    int AddMsg(_tPackMsg& msg)
    {
        //记录缓冲区解码的帧数长度、帧数
        return m_pMsgBuf->Add(&msg,1);
    }

    int IsEmpty()
    {
        return m_pDataBuf->IsEmpty();
    }
    int IsFull()
    {
        return m_pDataBuf->IsFull();
    }



    static void GetLocalTime(USHORT* time)
    {
        struct timeval tv;
        struct timezone tz;
        struct tm *tblock;

        gettimeofday(&tv,&tz);
        tblock = localtime(&tv.tv_sec);
        time[6] = tblock->tm_year + 1900;
        time[5] = tblock->tm_mon+1;
        time[4] = tblock->tm_mday;
        time[3] = tblock->tm_hour;
        time[2] = tblock->tm_min;
        time[1] = tblock->tm_sec;
        time[0] = tv.tv_usec/1000;
    }
    static timeval GetLocalTime(void)
    {
        struct timeval tv;
        struct timezone tz;
        gettimeofday(&tv,&tz);
        return tv;
    }

    static void Conv_uint64_TimeVect(uint64_t timeSec, uint64_t timeUsec, USHORT* time)
    {
       struct tm *tblock;

       tblock = localtime((const time_t*)&timeSec);
       time[6] = tblock->tm_year + 1900;
       time[5] = tblock->tm_mon+1;
       time[4] = tblock->tm_mday;
       time[3] = tblock->tm_hour;
       time[2] = tblock->tm_min;
       time[1] = tblock->tm_sec;
       time[0] = timeUsec/1000;
    }



private:
    CMBuffer<BYTE>* m_pDataBuf;
    CMBuffer<_tPackMsg>* m_pMsgBuf;
};

