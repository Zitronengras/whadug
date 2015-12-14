#include <iostream>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

Mat img,
	imgHSV,
	imgBlue,
	imgGray,
	imgCanny,
	imgCircle;
int radius;

int main(){
	 
	// Camera
	VideoCapture cap(0);					// capture the video from web cam
	if(!cap.isOpened()){					// if not success, exit program
         cout << "Cannot open the web cam" << endl;
         return -1;
    }
	while(true){
		bool bSuccess = cap.read(img);		// read a new frame from video
		if(!bSuccess){						// if not success, break loop
			cout << "Cannot read a frame from video stream" << endl;
            break;
	}

	medianBlur(img, img, 3);
	cvtColor(img, imgHSV, COLOR_BGR2HSV);	// Convert input image to HSV	
	inRange(imgHSV, Scalar(64, 33, 0), Scalar(115, 255, 255), imgBlue);		// Threshold the HSV image, keep only blue pixles
	GaussianBlur(imgBlue, imgBlue, Size(9, 9), 2, 2);

	cvtColor(img, imgGray, CV_BGR2GRAY);	// Graubild
	blur(imgGray, imgGray, Size(3,3));		// mit 3x3 Filtermaske weichzeichnen
	Canny(imgGray, imgCanny, 0, 200, 3);	// Kantenbild
		
	imgCircle = imgBlue.clone();
	img.copyTo(imgCircle, imgBlue);
	
	// Use the Hough transform to detect circles in the combined imgBlue image
	vector<Vec3f> circles;					// xc, yc und r
	HoughCircles(imgBlue, circles, CV_HOUGH_GRADIENT, 1, imgBlue.rows/8, 200, 50, 50, 200);
	
	// Loop over detected circles and outline them on img
	for(int i = 0; i < circles.size(); i++){
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		radius = cvRound(circles[i][2]);
		circle(imgCircle, center, 4, Scalar(255, 0, 0), -1, 8, 0);
		circle(imgCircle, center, radius, Scalar(255, 0, 255), 5);
		cout << "Radius" << radius << endl;
		cout << "Center" << center << endl;
	}
	imshow("Circle", imgCircle);
	imshow("Original", img);			//show the original image
        if (waitKey(30) == 27){			//wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
            cout << "esc key is pressed by user" << endl;
            break; 
       }
    }
	return 0;
}