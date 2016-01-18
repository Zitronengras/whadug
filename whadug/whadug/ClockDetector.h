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
	

	Point detectClock(Mat img);
	Mat getMaskedImg();

	ClockDetector(void);
	~ClockDetector(void);

	bool clockDetected;

private:
	Mat img,
	imgHSV,
	imgBlue,
	black,
	maskedClockImg;

	int radius;
	char filename[20];

	/*VideoCapture cap;*/
};
