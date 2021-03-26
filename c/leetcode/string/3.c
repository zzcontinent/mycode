/*
 *3. 无重复字符的最长子串
给定一个字符串，请你找出其中不含有重复字符的 最长子串 的长度。
示例 1:
输入: s = "abcabcbb"
输出: 3 
解释: 因为无重复字符的最长子串是 "abc"，所以其长度为 3。
示例 2:

输入: s = "bbbbb"
输出: 1
解释: 因为无重复字符的最长子串是 "b"，所以其长度为 1。
示例 3:

输入: s = "pwwkew"
输出: 3
解释: 因为无重复字符的最长子串是 "wke"，所以其长度为 3。
     请注意，你的答案必须是 子串 的长度，"pwke" 是一个子序列，不是子串。
示例 4:

输入: s = ""
输出: 0
 

提示：

0 <= s.length <= 5 * 104
s 由英文字母、数字、符号和空格组成
 * */


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

//hash table

#define HashLen 256
int hash[HashLen] = {0};

int HashInsert(int data)
{
	int j=data%HashLen;
	if (hash[j] != 0)
		return -1;
	hash[j]=data;
	return j;
}

void HashInit()
{
	memset(hash, 0, sizeof(hash[0]) * HashLen);
}


//hash table

int lenghOfLongestSubstring_hash(char * s)
{
	int len_s = strlen(s);
	int window = 2;
	int window_fail = 0;
	if (len_s == 0)
		return 0;
	if (len_s == 1)
		return 1;

	for (; window <= len_s; window ++)
	{
		window_fail = 0;
		for (int i=0; i< len_s - window + 1; i++)
		{
			HashInit();
			for (int ii = i; ii < i+window; ii ++)
			{
				if (HashInsert(s[ii]) == -1)
				{
					window_fail ++;
					break;
				}
			}
		}
		if (window_fail == len_s - window + 1)
			break;
	}
	if (window_fail != len_s)
		--window;
	return window;
}

int lengthOfLongestSubstring_map_sample(char * s){
    int start = 0, end = 0, maxlen = 0;
    char map[128] = {0};
    map[(int)*(s+start)] = 1;
    
    while( *(s+end) != 0 )
    {
        maxlen = maxlen>(end-start+1)?maxlen:(end-start+1);
        ++end;
        while( 0 != map[ (int)*(s+end) ] )//将要加入的新元素与map内元素冲突
        {
            map[ (int)*(s+start) ] = 0;
               ++start;
        }
        map[(int)*(s+end)] = 1;
    }
    
    return maxlen;
}


int lenghOfLongestSubstring(char * s)
{
	int len_s = strlen(s);
	int window = 2;
	int window_fail = 0;
	if (len_s == 0)
		return 0;
	if (len_s == 1)
		return 1;

	for (; window <= len_s; window ++)
	{
		window_fail = 0;
		for (int i=0; i< len_s - window + 1; i++)
		{
			int same_found = 0;
			for (int ii = i; ii < i+window; ii ++)
			{
				for (int jj = ii + 1; jj < i + window; jj++)
				{
					if (s[ii] == s[jj])
					{
						window_fail ++;
						same_found++;
						break;
					}
				}
				if (same_found > 0)
					break;
			}
		}
		if (window_fail == len_s - window + 1)
			break;
	}
	if (window_fail != len_s)
		--window;
	return window;
}


int main()
{
	//int fd = open("3.test", O_RDONLY);
	//char test1[10240];
	//printf("%d", read(fd, test1, 10240));
	//printf("%d\n", lenghOfLongestSubstring_hash(test1));
	printf("%d\n", lenghOfLongestSubstring_hash("abcabcbb"));
	printf("%d\n", lenghOfLongestSubstring_hash("abcabcde "));
	printf("%d\n", lenghOfLongestSubstring_hash("abcd"));
	printf("%d\n", lenghOfLongestSubstring_hash("abcabcbb"));
	return 0;
}
