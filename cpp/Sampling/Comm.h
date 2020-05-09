#pragma once

typedef struct PackMessageRec
{
	__int64  Type;
	__int64  Length;
	__int64  DestIP;
	__int64  SrcIP;
	int  Time[7];//year-month-day-hour-minutes-seconds-msec
}_tPackMsgRec;



typedef struct StrategyRec
{
	int m_PriorityStrategy;//���ȼ�����
	int m_Compression;//ѹ��ģʽ
	int m_EncryptStrategy; //���ܲ���
	int m_Storage;//�洢��ʽ
	int m_ThresholdStrtegy;//������ֵ
	int m_PartitionStrategy;//��������

}_tStrategyRec;