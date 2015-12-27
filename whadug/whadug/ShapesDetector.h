#pragma once

#include <iostream>
#include <opencv2\opencv.hpp>
#include "math.h"
#include <math.h> 

using namespace std;
using namespace cv;

class ShapesDetector
{
public:
	//Point triangleCenter;
	
	ShapesDetector(void);
	~ShapesDetector(void);

	Point getTriangleCenter(Mat image);
	//void setLabel(Mat& im, const string label,  vector<Point>& contour);
	void detectRectangles(Mat image);
	void compareAreas(double areaOne, double areaTwo);

private:
	Point minPt;
	Point hourPt;
	Point centerPt;

	Mat originalImg;

	double tDeg;
	double mDeg;
	double hDeg;

};

