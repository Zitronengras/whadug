#include <iostream>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

Mat img,
	imgGray,
	imgBinary,
	imgCanny,
	imgContours,
	imgCircle;

int radius;
char filename[20];

int main(){

	// Kamera
	VideoCapture cap(0);		// default camera 
	if(!cap.isOpened()){		// erfolgreich?
		cout << "Kamera konnte nicht aktiviert werden." << endl;
		return -1;
	}
	for(;;){
		cap >> img;
		if(img.empty())
			break;
		
		imshow("Camera", img);

		char key = (char)waitKey(5);
		switch (key){
		case 83:
		case 115:
			sprintf(filename, "Clock.jpg");
			imwrite(filename, img);
			cout << "Saved " << filename << endl;
			break;

		default:
			break;
		}
		if(waitKey(30) >= 0) break;
	}

	//img = imread("IMG_0351.jpg");
	
	cvtColor(img, imgGray, CV_BGR2GRAY);	// Graubild
	blur(imgGray, imgGray, Size(3,3));		// mit 3x3 Filtermaske weichzeichnen
	threshold(imgGray, imgBinary, 100, 255, THRESH_BINARY);	// Binärbild
	Canny(imgBinary, imgCanny, 0, 200, 3);	// Kantenbild

	// Konturen finden
	vector<vector<Point>> contours;			// Kontur wird als Vektor aus Punkten abgespeichert
	findContours(imgCanny, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0,0));

	// Konturen blau zeichnen
	imgContours = img.clone();
	for(int i = 0; i < contours.size(); i++){
		drawContours(imgContours, contours, i, Scalar(200, 0, 0), 2, 8);		
	}
	
	cvtColor(imgGray, imgCircle, CV_GRAY2BGR);

	// Kreise finden
	vector<Vec3f> circles;		// xc, yc und r
	HoughCircles(imgGray, circles, CV_HOUGH_GRADIENT, 1, imgGray.rows/8, 150, 20, 100, 0);

	// gefundenen Kreis einzeichnen
	for(int i = 0; i < circles.size(); i++){
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		radius = cvRound(circles[i][2]);
		circle(imgCircle, center, 4, Scalar(255, 0, 0), -1, 8, 0);
		circle(imgCircle, center, radius, Scalar(255, 0, 255), 3, 8, 0);

		cout << "Radius" << radius << endl;
		cout << "Center" << center << endl;
	}
	
	const char* image = "Clock";
	const char* gray = "Gray";
	const char* binary = "Binary";
	const char* canny = "Canny";
	const char* contour = "Contours";
	const char* circleIMG = "Circles";
	
	namedWindow(image, WINDOW_AUTOSIZE);
	namedWindow(gray, WINDOW_AUTOSIZE);
	namedWindow(binary, WINDOW_AUTOSIZE);
	namedWindow(canny, WINDOW_AUTOSIZE);
	namedWindow(contour, WINDOW_AUTOSIZE);
	namedWindow(circleIMG, WINDOW_AUTOSIZE);
	
	imshow(image, img);
	imshow(gray, imgGray);
	imshow(binary, imgBinary);
	imshow(canny, imgCanny);
	imshow(contour, imgContours);
	imshow(circleIMG, imgCircle);

	waitKey (0);
	return 0;
}