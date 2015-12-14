#include <iostream>
#include <opencv2\opencv.hpp>
#include <math.h> 

#define CAMERA

using namespace std;
using namespace cv;

//********************Initialisierung von Methoden***********************
void runProgramm();
float calculateRectangleDistanceBetweenTwoPoints(float point1X, float point1Y, float point2X, float point2Y);
float calculateRectangleCenterX(float point1X, float point1Y, float point2X, float point2Y);
float calculateRectangleCenterY(float point1X, float point1Y, float point2X, float point2Y);
float calculateTriangleCenterX(float point1X, float point1Y, float point2X, float point2Y, float point3X, float point3Y);
float calculateTriangleCenterY(float point1X, float point1Y, float point2X, float point2Y, float point3X, float point3Y);
void runCameraAndCircle();
void runCompareArea();
void compareAreas(float areaOne, float areaTwo);
String toString(int number);
void showTime(int minutes, int hours);

double smallArea, bigArea;
	
int main(){

	char character;
	float rectangleDistanceBetweenTwoPoints, rectangleCenterX, rectangleCenterY, triangleCenterX, triangleCenterY, x1, y1, x2, y2, x3, y3;

	
	//***************Calculation points***************
	//Rectangle: P1(x1, y1) & P2(X2, y2)
	x1 = 0;
	y1 = 0;
	x2 = 1; 
	y2 = 0;

	//Triangle: P1(x1, y1) & P2(X2, y2) & P3(X3, y3)
	x3 = 0.5; 
	y3 = 1;

	//************************************************

	cout << "a: Strecke zwischen zwei Punkten" << endl;
	cout << "b: Mittelpunkt des Rechtecks" << endl;
	cout << "c: Mittelpunkt des Dreiecks" << endl;
	cout << "d: Eigene Punkte angeben fuer Rechteck" << endl;
	cout << "e: Eigene Punkte angeben fuer Dreieck" << endl;
	cout << "f: Programm starten" << endl;
	cout << "g: Kreis erkennen und mit \"s\" einen Snapshot machen" << endl;
	cout << "h: Uhrzeit und Flaecheninhalt anzeigen" << endl;

	for(;;){
		cout << "Eingabe des Buchstaben: ";
		cin >> character;

		switch (character)
		{
case 'a': 
				rectangleDistanceBetweenTwoPoints = calculateRectangleDistanceBetweenTwoPoints(x1,y1,x2,y2);
				cout << "Strecke zwischen zwei Punkten: " << rectangleDistanceBetweenTwoPoints << endl;
				break;

			case 'b': 
				rectangleCenterX = calculateRectangleCenterX(x1,y1,x2,y2);
				rectangleCenterY = calculateRectangleCenterY(x1,y1,x2,y2);
				cout << "Mittelpunkt des Rechtecks: " << "M(" << rectangleCenterX << ","<< rectangleCenterY << ")"<< endl;
				break;

			case 'c': 
				triangleCenterX = calculateTriangleCenterX(x1,y1,x2,y2,x3,y3);
				triangleCenterY = calculateTriangleCenterY(x1,y1,x2,y2,x3,y3);
				cout << "Mittelpunkt des Dreiecks: " << "S(" << triangleCenterX << ","<< triangleCenterY << ")"<< endl;
				break;

			case 'd': 
				cout << "X1: ";
				cin >> x1;
				cout << "Y1: ";
				cin >> y1;
				cout << "X2: ";
				cin >> x2;
				cout << "Y2: ";
				cin >> y2;
				break;

			case 'e': 
				cout << "X1: ";
				cin >> x1;
				cout << "Y1: ";
				cin >> y1;
				cout << "X2: ";
				cin >> x2;
				cout << "Y2: ";
				cin >> y2;
				cout << "X3: ";
				cin >> x3;
				cout << "Y3: ";
				cin >> y3;
				break;

			case 'f': 
				runProgramm();
			break;

			case 'g': 
				runCameraAndCircle();
			break;

			case 'h': 
				runCompareArea();
			break;

			default: 
				cout << "Bitte gueltigen Wert eingeben! ";
				break;
		}
	}
}

void runProgramm(){

const float FAKTOR = 0.5;
Mat img;
RNG rng(12345);

#ifdef CAMERA
	VideoCapture capture(0);

	if(!capture.isOpened()){
		cout<<"Fehler!"<<endl;
		system("pause");
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


}
//****************************CODE SIMON********************************
//***************FUNCTIONS***************
float calculateRectangleDistanceBetweenTwoPoints(float point1X, float point1Y, float point2X, float point2Y){
	//Point 1: (x1,y1) and Point 2: (x2, y2)
	//d = distance
	//interimResultX and interimResultY handle the result of x2-x1 and y2-y1
	float d, interimResultX, interimResultY;

	interimResultX = (point2X-point1X);
	interimResultY = (point2Y-point1Y);

	interimResultX = pow(interimResultX, 2);	//Exponent function
	interimResultY = pow(interimResultY, 2);	//Exponent function

	d = sqrt(interimResultX+interimResultY);
	return d;
}

float calculateRectangleCenterX(float point1X, float point1Y, float point2X, float point2Y){
	float mX;
	mX =  (point1X + point2X) / 2;
	return mX;
}

float calculateRectangleCenterY(float point1X, float point1Y, float point2X, float point2Y){
	float mY;
	mY =  (point1Y + point2Y) / 2;
	return mY;
}

float calculateTriangleCenterX(float point1X, float point1Y, float point2X, float point2Y, float point3X, float point3Y){
	float mX;
	mX = (point1X + point2X + point3X) / 3;
	return mX;
}

float calculateTriangleCenterY(float point1X, float point1Y, float point2X, float point2Y, float point3X, float point3Y){
	float mY;
	mY = (point1Y + point2Y + point3Y) / 3;
	return mY;
}
//****************************CODE SIMON ENDE********************************

//****************************CODE SONJA********************************
void runCameraAndCircle(){
	Mat img,
	imgGray,
	imgBinary,
	imgCanny,
	imgContours,
	imgCircle;

int radius;
char filename[20];

// Kamera
	VideoCapture cap(0);		// default camera 
	if(!cap.isOpened()){		// erfolgreich?
		cout << "Kamera konnte nicht aktiviert werden." << endl;
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
}
//****************************CODE SONJA ENDE********************************

//****************************CODE CARO********************************
void runCompareArea(){

	float areaOne = 21.5;
	float areaTwo = 22.0; 
	compareAreas(areaOne, areaTwo);

	int minutes = 58;
	int hours = 23;
	showTime(minutes, hours);

	waitKey(0);
}


void compareAreas(float areaOne, float areaTwo){

		if (areaOne < areaTwo){
			smallArea = areaOne;
			bigArea = areaTwo;
			cout << "Grosser Zeiger: " << bigArea << " \nKleiner Zeiger: " << smallArea << endl;
		}
		else if(areaOne > areaTwo){
			smallArea = areaTwo;
			bigArea = areaOne;
			cout << "Grosser Zeiger: " << bigArea << " \nKleiner Zeiger: " << smallArea << endl;
		}
		else{
			cout << "Fehler bei Vergleich der Flächeninhalte" << endl;
		}
}

String toString(int number){

	String s;
	stringstream out;
	out << number;
	s = out.str();
	return s;

}

void showTime(int minutes, int hours){
	//just for test
	Mat img = imread("..\\src\\tulpen.jpg", CV_LOAD_IMAGE_COLOR);

	String label = toString(hours) + ":" + toString(minutes);

	// rectangle for GUI
	Point RectUpLeft = Point(50, 0);
	Point RectDownRight = Point(195, 220);
	rectangle(img, RectUpLeft, RectDownRight, Scalar(40, 40, 40), CV_FILLED, 8, 0);

	//text properties
	int fontface = FONT_HERSHEY_SIMPLEX; //FONT_HERSHEY_PLAIN
    double scale = 1.1;
    int thickness = 1;
    int baseline = 0;

	// center label horizontally
	Size text = getTextSize(label, fontface, scale, thickness, &baseline);
	int textX = (((RectDownRight.x - RectUpLeft.x)/2) + RectUpLeft.x) - (text.width /2);
	// vertical position
	int textY = (RectDownRight.y - (RectDownRight.y / 5));
	//display time
	putText(img, label, Point(textX, textY), fontface, scale, Scalar(215, 215, 215), thickness, 8);
	
	namedWindow("Uhrzeit");
	imshow("Uhrzeit", img);	
}
//****************************CODE CARO ENDE********************************