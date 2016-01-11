#include "ClockDetector.h"

ClockDetector::ClockDetector(void)
{
	//Camera
	 cap = VideoCapture(0);					// capture the video from web cam
}


ClockDetector::~ClockDetector(void)
{
}

Point ClockDetector::detectClock(Mat img){
	
	if(!cap.isOpened()){					// if not success, exit program
         cout << "Cannot open the web cam" << endl;
    }
	bool bSuccess = cap.read(img);		// read a new frame from video

	medianBlur(img, img, 3);
	cvtColor(img, imgHSV, COLOR_BGR2HSV);	// Convert input image to HSV	
	inRange(imgHSV, Scalar(64, 33, 0), Scalar(115, 255, 255), imgBlue);		// Threshold the HSV image, keep only blue pixles
	imshow("Blue", imgBlue);
	GaussianBlur(imgBlue, imgBlue, Size(9, 9), 2, 2);

	maskedClockImg = imgBlue.clone();
	img.copyTo(maskedClockImg, imgBlue);
	
	//Use the Hough transform to detect circles in the combined imgBlue image
	vector<Vec3f> circles;					// xc, yc und r
	HoughCircles(imgBlue, circles, CV_HOUGH_GRADIENT, 1, imgBlue.rows/8, 200, 50, 50, 200);

	//loop over detected circles and outline them on img
	for(int i = 0; i < circles.size(); i++){
		circleCenter = Point(cvRound(circles[i][0]), cvRound(circles[i][1]));
		radius = cvRound(circles[i][2]);
		circle(maskedClockImg, circleCenter, 4, Scalar(255, 0, 0), -1, 8, 0);
		circle(maskedClockImg, circleCenter, radius, Scalar(255, 0, 255), 5);
		cout << "Radius" << radius << endl;
		cout << "Center" << circleCenter << endl;
	}

	imshow("Circle", maskedClockImg);
	imshow("Original", img);			//show the original image
	
	return circleCenter;
}