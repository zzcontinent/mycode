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
	int m_PriorityStrategy;//优先级策略
	int m_Compression;//压缩模式
	int m_EncryptStrategy; //加密策略
	int m_Storage;//存储方式
	int m_ThresholdStrtegy;//报警阈值
	int m_PartitionStrategy;//分区策略

}_tStrategyRec;