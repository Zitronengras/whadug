#pragma once

#include <iostream>
#include <opencv2\opencv.hpp>
//#include <math.h> 

using namespace std;
using namespace cv;

class ClockDetector
{

public:
	Point clockCenter;
	Mat maskedClockImg;

	Mat detectClock();

	ClockDetector(void);
	~ClockDetector(void);	

private:
	Mat img,
	imgHSV,
	imgBlue,
	imgGray,
	imgCanny;

	int radius;
	char filename[20];

	VideoCapture cap;
};

