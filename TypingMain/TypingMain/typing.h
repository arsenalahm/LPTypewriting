#pragma once

#include <cstdio>
#include <iostream>
#include <cmath>
#include <opencv2\opencv.hpp>
#include <iostream>
#include <cmath>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <queue>
#include <thread>
#include "LeapC.h"
#include "motion.h"
#include "voc.h"
#include "display.h"

class Typing
{
public:
	Motion motion;
	Voc voc;

	char wheelR[6];
	int wheelL;
	double rate[6];
	int angleR[6];
	string text;
	string word;
	int len;
	
	int counterEmit;
	int counterSlot;
	cv::Mat picture;
	std::vector<std::pair<float, char>> vocRec;
	char inputChar;

	double angleLeft;
	double angleRight;

	void initshow();
	void runshow();
	void renewWheel();
	void getVoc();
	void getLeft(double angle);
	void getRight(double angle);
	void getDel();
	void getSps();
	void getEnt();

	Typing();
	~Typing();
};