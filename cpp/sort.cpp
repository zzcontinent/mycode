#include <map>
#include <pthread.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>

using namespace std;

typedef pair<int, string> PAIR;
bool rsort_by_str(const PAIR &p1, const PAIR &p2)
{
	return p1.second.compare(p2.second);
}

int main()
{
	map<int, string> t1;
	t1.insert(PAIR(4, "hello4"));
	t1.insert(PAIR(5, "hello5"));
	t1.insert(PAIR(6, "hello6"));
	t1[0] = "hello0";
	t1[1] = "hello1";
	t1[2] = "hello2";
	map<int, string>::iterator iter;
	for (iter = t1.begin(); iter != t1.end(); iter++)
		cout << iter->first << ":" << iter->second <<endl;

	vector<PAIR> v1(t1.begin(), t1.end());
	sort(v1.begin(), v1.end(), rsort_by_str);
	for (int i=0; i != v1.size(); i++)
	{
		cout << v1[i].first << ":" << v1[i].second << endl;
	}
	return 0;
}
