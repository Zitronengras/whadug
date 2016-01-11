#include "ClockDetector.h"

ClockDetector::ClockDetector(void)
{
}


ClockDetector::~ClockDetector(void)
{
}

Point ClockDetector::detectClock(Mat img){

	medianBlur(img, img, 3);
	cvtColor(img, imgHSV, COLOR_BGR2HSV);	// Convert input image to HSV	
	inRange(imgHSV, Scalar(64, 33, 0), Scalar(115, 255, 255), imgBlue);		// Threshold the HSV image, keep only blue pixles
	GaussianBlur(imgBlue, imgBlue, Size(9, 9), 2, 2);
	
	//Use the Hough transform to detect circles in the combined imgBlue image
	vector<Vec3f> circles;					// xc, yc und r
	HoughCircles(imgBlue, circles, CV_HOUGH_GRADIENT, 1, imgBlue.rows/8, 200, 50, 50, 200);

	//loop over detected circles and outline them on img
	for(int i = 0; i < circles.size(); i++){
		circleCenter = Point(cvRound(circles[i][0]), cvRound(circles[i][1]));
		radius = cvRound(circles[i][2]);
		circle(imgBlue, circleCenter, 4, Scalar(255, 0, 0), -1, 8, 0);
		circle(imgBlue, circleCenter, radius, Scalar(255, 0, 255), 5);
		cout << "Radius" << radius << endl;
		cout << "Center" << circleCenter << endl;
	}	
	
	return circleCenter;
}

Mat ClockDetector::getMaskedImg(){
	
	black = Mat(imgHSV.rows, imgHSV.cols, CV_8UC3, Scalar(0, 0, 0));

	maskedClockImg = Mat(imgHSV.rows, imgHSV.cols, CV_8UC3, Scalar(0, 0, 0));
	
	circle(black, circleCenter, radius, Scalar(255, 255, 255),-1, 8, 0);
	
	imgHSV.copyTo(maskedClockImg, black);
	
	imshow("Mask", maskedClockImg);
	return maskedClockImg;
	
}