/*
 *30. 串联所有单词的子串
 给定一个字符串 s 和一些长度相同的单词 words。找出 s 中恰好可以由 words 中所有单词串联形成的子串的起始位置。
 注意子串要与 words 中的单词完全匹配，中间不能有其他字符，但不需要考虑 words 中单词串联的顺序。

 示例 1：
 输入：
 s = "barfoothefoobarman",
 words = ["foo","bar"]
 输出：[0,9]
 解释：
 从索引 0 和 9 开始的子串分别是 "barfoo" 和 "foobar" 。
 输出的顺序不重要, [9,0] 也是有效答案。
 示例 2：
 输入：
 s = "wordgoodgoodgoodbestword",
 words = ["word","good","best","word"]
 输出：[]
 * */
#include <stdint.h>
#include <stdio.h>

uint32_t simple_hash(char* s, int len)
{
	uint32_t ret = 0;
	for (int i = 0; i < len; i++) {
		char shift = i % 4;
		ret += (*(s + i) << shift * 8);
	}
	return ret;
}

int isContactSubstr(uint32_t* pow_s_hash, uint32_t add_words_hash, int wordsSize, int word_len)
{
	uint32_t add_s_hash = 0;
	for (int i = 0; i < wordsSize; i++) {
		add_s_hash += pow_s_hash[i * word_len];
	}
	return add_words_hash == add_s_hash;
}

int* findSubstring(char* s, char** words, int wordsSize, int* returnSize)
{
	*returnSize = 0;
	int* results = (int*)malloc(sizeof(int) * 0xFF);

	if (!wordsSize || !s[0]) {
		return results;
	}

	int word_len = strlen(words[0]);
	int substr_len = wordsSize * word_len;

	uint32_t pow_s_hash[0xFFFF];
	uint32_t add_words_hash = 0;

	int s_len = strlen(s);

	for (int i = 0; i <= s_len - word_len; i++) {
		uint32_t hash = simple_hash(s + i, word_len);
		pow_s_hash[i] = hash * hash;
	}

	for (int i = 0; i < wordsSize; i++) {
		uint32_t hash = simple_hash(words[i], word_len);
		add_words_hash += hash * hash;
	}

	for (int i = 0; i <= s_len - substr_len; i++) {
		if (isContactSubstr(pow_s_hash + i, add_words_hash, wordsSize, word_len)) {
			results[(*returnSize)++] = i;
		}
	}

	return results;
}
