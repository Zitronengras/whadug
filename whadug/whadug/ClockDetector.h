#pragma once

#include <iostream>
#include <opencv2\opencv.hpp>
//#include <math.h> 

using namespace std;
using namespace cv;

class ClockDetector
{

public:
	Point circleCenter;
	Mat maskedClockImg;

	Point detectClock(Mat img);

	ClockDetector(void);
	~ClockDetector(void);	

private:
	Mat img,
	imgHSV,
	imgBlue;

	int radius;
	char filename[20];

	VideoCapture cap;
};
