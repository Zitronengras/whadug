#include <iostream>
#include <opencv2\opencv.hpp>

#define CAMERA

using namespace std;
using namespace cv;

const float FAKTOR = 0.5;

RNG rng(12345);

int main(){
	
	Mat img;
	
#ifdef CAMERA
	VideoCapture capture(0);

	if(!capture.isOpened()){
		cout<<"Fehler!"<<endl;
		system("pause");
		return 0;
	}

	int s = 0;
	for(;;){		//endlos Schleife
		capture>>img;
		imshow("image", img);

		Mat gray;
		cvtColor(img, gray, CV_BGR2GRAY);

		//imshow("gray", gray);

		//Mat Ecken = gray.clone();
		//Eckfinder
		//Canny(gray, Ecken, 100, 200, 3);
		//imshow("canny", Ecken);

		namedWindow("gray", WINDOW_AUTOSIZE);
		
		createTrackbar("Schwellwert", "gray", &s, 255, 0);
		threshold(gray, gray, s, 255, CV_THRESH_BINARY);

		vector<vector<Point>> contures;
			vector<Vec4i> hierachy;

		imshow("gray", gray);

		findContours(gray, contures, hierachy, RETR_LIST, CV_CHAIN_APPROX_NONE);

		Mat Execute = Mat::zeros(img.size(), CV_8UC3);

		for(int i = 0; i < contures.size(); i++){
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(Execute, contures, i, color, CV_FILLED, 8, noArray());
		}

		imshow("contours", Execute);

		Mat Work = gray.clone();

		// die wichtigsten Bildverarbeitungsalg
		erode(Work, Work, Mat(), Point(-1, -1), 5, 0, morphologyDefaultBorderValue());
		dilate(Work, Work, Mat(), Point(-1, -1), 5, 0, morphologyDefaultBorderValue());


		imshow("Work", Work);
		imshow("gray", gray);


		/*vector<Vec3f> circles;
		HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 1, img.rows/8, 200, 100, 0, 0);
		
		for(int i = 0; i < circles.size(); i++){
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			circle(img, center, 3, Scalar(0, 255, 0), -1, 8, 0);
			circle(img, center, radius, Scalar(0, 255, 255), 3, 8, 0);
		}*/
		imshow("Kreise", img);


		if(cvWaitKey(1) == 27) break;
	}

	//imwrite("C:\\Test.jpg", img);

	capture.release();
#else
	img = imread("C:/Users/Public/Pictures/Sample Pictures/desert.jpg", CV_LOAD_IMAGE_COLOR);

	int sizex = img.size().width * FAKTOR;
	int sizey = img.size().height * FAKTOR;

	resize(img, img, Size(sizex, sizey));

	//Male in das Bild einen Kreis an der Position x = 200, y = 200 mit dem Radius 23, Farbe blau, 1 = Dicke, 8 = Linientyp, 0 =
	//circle(img, Point(200, 200), 23, Scalar(255, 0, 0), -1, 8, 0);
	//rectangle(img, Point(200, 500), Point(400, 600), Scalar(0, 255, 255), CV_FILLED, 8, 0);
	//line(img, Point(100, 100), Point(110, 300), Scalar(0, 225, 0), 10, 8, 0);
	//putText(img, "Was geht?", Point(40, 40), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0), 1, 8, false);

	

	int count = 0;
	for(;;) {
		Mat Klaus = img.clone();
		//ostringstream s1;
		//count++;
		//s1<<count;
		//putText(Klaus, s1.str(), Point(40, 40), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0), 1, 8, false);

		//Mat RGB_channel[3];
		//split(img, RGB_channel);

	/*	imshow("Red", RGB_channel[2]);
		imshow("Green", RGB_channel[1]);
		imshow("Blue", RGB_channel[0]);*/
		
		Mat HSV_img;
		Mat HSV_channel[3];

		cvtColor(img, HSV_img, CV_BGR2HSV);
		split(HSV_img, HSV_channel);

		imshow("HSV", HSV_img);
		imshow("Hue", HSV_channel[0]);
		imshow("Saturation", HSV_channel[1]);
		imshow("Value", HSV_channel[2]);

		//HSV_img.save("c:\Hello.jpg");

		imshow("image", Klaus);
		if(cvWaitKey(1) == 27) break;
	}

	cout<<"Keine Kamera"<<endl;

#endif

	system("pause");
	return 0;

}