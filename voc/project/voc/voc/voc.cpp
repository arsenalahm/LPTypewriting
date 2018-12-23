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
#define DEBUG 1
class Voc {
private:
	json wordPrefix;			//所有前缀的词频
	json wordFreq;				//所有词的词频
	json prefix2Word;
	int allfreq=0;				//所有开头字母的频数
	const string wordPrefixPath = "./data/prefix";		//词前缀路径
	const string wordFreqPath = "./data/lexicon.json";	//词频路径
	const string prefix2WordPath = "./data/prefix2Word";
protected:
	/* @Func 返回prefix的频数
	*/
	int getFreq(string prefix)
	{
		if (prefix == "") return getAllFreq();
		if (wordPrefix.count(prefix) == 0) return 0;
		return wordPrefix[prefix];
	}
	/* @Func 返回总词数
	*/
	int getAllFreq()
	{
		return allfreq ? allfreq : 1;
	}

	/** @Func 根据输入的prefix得到下一个备选字母和频率（num个备选）
		@ret [(freq,char)] 可能少于num个
	*/
	const vector<pair<float, string> > getnextsingles(string prefix, int num)
	{
		priority_queue<pair<float, string> > queue;

		
		for (char i = 'a'; i <= 'z'; i++)
		{
			float nextFreq = getFreq(prefix + string(1,i));
			float rfreq = nextFreq==0?0 : float(nextFreq + 1) / (getFreq(prefix) + 26);
			queue.push(make_pair(rfreq, string(1, i)));
		}

		vector<pair<float, string> >vec;
		for (int i = 0; i < num; i++)
		{
			auto tmp = queue.top();
			if (tmp.first == 0) break;
			vec.push_back(tmp);
			queue.pop();
		}
		return vec;
	}
	/** @Func 通过prefix得到联想的单词
		@ret [(freq,word)] 可能为空，数量为确有的排序
	*/
	const vector<pair<float, string> > getnextwords(string prefix, int num)
	{
		priority_queue<pair<float, string> > queue;
		const vector<pair<float, string> > a;
		if (prefix2Word.find(prefix)==prefix2Word.end()) {
		
			return a;
		}
		vector<pair<float, string> >vec;
		int allnum = 0;
		for (string word : prefix2Word[prefix])
		{
			allnum += wordFreq[word];
		}
		for (string word : prefix2Word[prefix])
		{
			queue.push(make_pair((float)wordFreq[word] / allnum, word));
		}
		for (int i = 0; (!queue.empty())&&(i < num) ; i++)
		{
			auto tmp = queue.top();
			vec.push_back(tmp);
			queue.pop();
		}
		return vec;
	}
public:
	/* @Func 清空内部存储
	*/
	void clearall()
	{
		wordPrefix = NULL;
		allfreq = 0;
		wordFreq = NULL;
		prefix2Word = NULL;
	}
	/*	@Func 初始化，读取词频文件，统计总词数
	*/
	void init()
	{
		clearall();
		ifstream i(wordPrefixPath);
		i>> wordPrefix;
#if DEBUG
		cout << "Load wordPrefix done" << endl;
#endif
		ifstream ifsw(wordFreqPath);
		ifsw >> wordFreq;
#if DEBUG
		cout << "Load wordFreq done" << endl;
#endif
		ifstream ifsp(prefix2WordPath);
		ifsp >> prefix2Word;
#if DEBUG
		cout << "Load prefix2Word done" << endl;
#endif

		for (char i = 'a'; i <= 'z'; i++)
		{
			this->allfreq += getFreq(string(1,i));
		}
	}
	/** @Func 通过prefix得到联想,字母不足单词补全，字母和单词都不足则最高频率单字母补全
		@param alphaNum 字母数量，wordNum 联想单词数量
		@ret [(freq,word)] 数量为alphaNum+wordNum
	*/
	const vector<pair<float, string> > getnexts(string prefix, int alphaNum, int wordNum)
	{
		const vector<pair<float, string> > alphas = getnextsingles(prefix, alphaNum);
		if (alphas.size() < alphaNum)
		{
			wordNum += alphaNum - alphas.size();
		}
		const vector<pair<float, string> > words = getnextwords(prefix, wordNum);
		vector<pair<float, string> > res;
		for (auto e : alphas)
		{
			res.push_back(e);
		}
		for (auto e : words)
		{
			res.push_back(e);
		}
		if (words.size() < wordNum)
		{
			const vector<pair<float, string> > firstAlphas = getnextsingles("", wordNum - words.size());
			for (auto e : firstAlphas)
			{
				res.push_back(e);
			}
		}
		return res;
	}
	void debug(string prefix, int alphaNum, int wordNum)
	{
		cout << "--------singles----" << endl;
		for (auto e : getnextsingles(prefix, alphaNum))
		{
			cout << e.first << " " << e.second << endl;
		}
		cout << "--------words" << endl;
		for (auto e : getnextwords(prefix, wordNum))
		{
			cout << e.first << " " << e.second << endl;
		}
		cout << "--------singles & words" << endl;
		for (auto e : getnexts(prefix, alphaNum,wordNum))
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
		cout << "get " << prefix << endl;
		voc.debug(prefix, 5,3);
		
	}
    return 0;
}

