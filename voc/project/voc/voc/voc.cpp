// voc.cpp: 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <queue>
using json = nlohmann::json;
using namespace std;
class Voc {
private:
	json data;
	
protected:
	int getFreq(string prefix)
	{
		if (data.count(prefix) == 0) return 0;
		return data[prefix];
	}
public:
	void init()
	{
		ifstream i("./data/prefix");
		i>>data;
	}
	
	const vector<pair<float, char> > getnext(string prefix,int num)
	{
		priority_queue<pair<float, char> > queue;
		for (char i = 'a'; i <= 'z'; i++)
		{
			float rfreq = float(getFreq(prefix + i) + 1) / (getFreq(prefix) + 26);
			queue.push(make_pair(rfreq, i));
		}
		vector<pair<float, char> >vec;
		for (int i = 0; i < num; i++)
		{
			auto tmp = queue.top();
			vec.push_back(tmp);
			queue.pop();
		}
		return vec;
	}

	void debug(string prefix, int num)
	{
		for (auto e : getnext(prefix, num))
		{
			cout << e.first << " " << e.second << endl;
		}
	}
}voc;
int main()
{
	voc.init();
	cout << "init() done" << endl;
	while (1)
	{
		string prefix;
		cin >> prefix;
		voc.debug(prefix, 5);
	}
    return 0;
}

