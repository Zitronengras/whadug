#include <iostream>
#include <opencv2\opencv.hpp>
#include <math.h> 

using namespace std;
using namespace cv;

//********************Initialisierung von Methoden***********************
float calculateRectangleDistanceBetweenTwoPoints(float point1X, float point1Y, float point2X, float point2Y);
float calculateRectangleCenterX(float point1X, float point1Y, float point2X, float point2Y);
float calculateRectangleCenterY(float point1X, float point1Y, float point2X, float point2Y);
float calculateTriangleCenterX(float point1X, float point1Y, float point2X, float point2Y, float point3X, float point3Y);
float calculateTriangleCenterY(float point1X, float point1Y, float point2X, float point2Y, float point3X, float point3Y);
int runCameraAndCircle();
void runCompareArea();
void compareAreas(double areaOne, double areaTwo, Point areaOneCenter, Point areaTwoCenter);
String toString(int number);
void showTime(int minutes, int hours);
void setLabel( Mat& im, const  string label,  vector< Point>& contour);
void runCameraRectangle();

void setRecCenterMinutes(cv::Point receivedRecCenterMinutes);
cv::Point getRecCenterMinutes();

void setRecCenterHours(cv::Point receivedRecCenterHours);
cv::Point getRecCenterHours();


double smallArea, bigArea;
Point centerMinutes(0,0);
Point centerHours(0,0);

Point recCenterMinutes(0,0);
Point recCenterHours(0,0);


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
	cout << "n: Rechtecke erkennen" << endl;
	cout << "m: Zeiger Mittelpunkte anzeigen" << endl;

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

			case 'g': 
				runCameraAndCircle();
			break;

			case 'h': 
				runCompareArea();
			break;

			case 'n':
				runCameraRectangle();
			break;

			case 'm':
				//Erst "n" ausführen, dann esc drücken um "m" zu starten.
				cout << "Grosser Zeiger Center: " << centerMinutes << " \nKleiner Zeiger Center: " << centerHours << endl;
			break;

			default: 
				cout << "Bitte gueltigen Wert eingeben! ";
				break;
		}
	}
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
int runCameraAndCircle(){
	Mat img,
	imgHSV,
	imgBlue,
	imgGray,
	imgCanny,
	imgCircle;

int radius;
char filename[20];

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
//****************************CODE SONJA ENDE********************************

//****************************CODE CARO********************************
void runCompareArea(){

	int minutes = 58;
	int hours = 8;
	showTime(minutes, hours);

	waitKey(0);
}

void setRecCenterMinutes(cv::Point receivedRecCenterMinutes){
	 recCenterMinutes = receivedRecCenterMinutes;
}

cv::Point getRecCenterMinutes(){
	return recCenterMinutes;
}

void setRecCenterHours(cv::Point receivedRecCenterHours){
	 recCenterHours = receivedRecCenterHours;
}

cv::Point getRecCenterHours(){
	return recCenterHours;
}

void compareAreas(double areaOne, double areaTwo, Point areaOneCenter, Point areaTwoCenter){

		if (areaOne < areaTwo){
			smallArea = areaOne;
			centerMinutes = areaOneCenter;
			//setRecCenterMinutes(areaOneCenter);

			bigArea = areaTwo;
			centerHours = areaTwoCenter;
			//setRecCenterHours(areaTwoCenter);

			cout << "Grosser Zeiger: " << bigArea << " \nKleiner Zeiger: " << smallArea << endl;
			cout << "Grosser Zeiger Center: " << centerMinutes << " \nKleiner Zeiger Center: " << centerHours << endl;
			//cout << "Grosser Zeiger Center: " << getRecCenterMinutes << " \nKleiner Zeiger Center: " << getRecCenterHours << endl;
		}
		else if(areaOne > areaTwo){
			smallArea = areaTwo;
			centerMinutes = areaTwoCenter;
			//setRecCenterMinutes(areaTwoCenter);

			bigArea = areaOne;
			centerHours = areaOneCenter;
			//setRecCenterHours(areaOneCenter);
			
			cout << "Grosser Zeiger: " << bigArea << " \nKleiner Zeiger: " << smallArea << endl;
			cout << "Grosser Zeiger Center: " << centerMinutes << " \nKleiner Zeiger Center: " << centerHours << endl;
			//cout << "Grosser Zeiger Center: " << getRecCenterMinutes << " \nKleiner Zeiger Center: " << getRecCenterHours << endl;
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


//****************************CODE NIELAB ********************************

void setLabel( Mat& im, const  string label,  vector< Point>& contour)
{
	int fontface =  FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;

	 Size text =  getTextSize(label, fontface, scale, thickness, &baseline);
	 Rect r =  boundingRect(contour);

	 Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	 rectangle(im, pt +  Point(0, baseline), pt +  Point(text.width, -text.height), CV_RGB(255,255,255), CV_FILLED);
	 putText(im, label, pt, fontface, scale, CV_RGB(0,0,0), thickness, 8);
}



void runCameraRectangle()
 {
    VideoCapture cap(0); //capture the video from web cam

    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
        // return -1;
    }

 //   namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

 //int iLowH = 0;
 //int iHighH = 179;

 //int iLowS = 0; 
 //int iHighS = 255;

 //int iLowV = 0;
 //int iHighV = 255;

 ////Create trackbars in "Control" window
 //cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
 //cvCreateTrackbar("HighH", "Control", &iHighH, 179);

 //cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
 //cvCreateTrackbar("HighS", "Control", &iHighS, 255);

 //cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
 //cvCreateTrackbar("HighV", "Control", &iHighV, 255);
 
    while (true)
    {
        Mat imgOriginal;

        bool bSuccess = cap.read(imgOriginal); // read a new frame from video

         if (!bSuccess) //if not success, break loop
        {
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }

  Mat imgHSV;

  cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
 
  Mat imgThresholded;

 inRange(imgHSV, Scalar(0, 48,117), Scalar(94, 255, 255), imgThresholded); //Threshold the image
//  inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

  //remove small objects from the foreground
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

  //fill small holes in the foreground
  //dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
  //erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

	// Find contours
	 vector< vector< Point> > contours;
	 findContours(imgThresholded.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	 vector<Point> approx;
	 vector<double> detectedAreas;
	 vector<Point> centerPointArray;

	for (int i = 0; i < contours.size(); i++)
	{
		// Approximate contour with accuracy proportional
		// to the contour perimeter
		 approxPolyDP( Mat(contours[i]), approx,  arcLength( Mat(contours[i]), true)*0.02, true);

		 if (fabs(contourArea(contours[i])) < 100 || !isContourConvex(approx))
			continue;

		if (approx.size() == 4)
		{
			Moments M = moments (contours[i]);
			setLabel(imgOriginal, "Rec", contours[i]);
			detectedAreas.push_back(fabs(contourArea(contours[i])));
			centerPointArray.push_back(Point((M.m10/M.m00),(M.m01/M.m00)));
			cout << detectedAreas.size() <<endl;
			if(detectedAreas.size() == 2){
				for (int i=0; i< detectedAreas.size(); i++){
					//double ratio = detectedAreas[i]/detectedAreas[i+1]; Ratio of two Areas
					compareAreas(detectedAreas[i],detectedAreas[i+1], centerPointArray[i], centerPointArray[i+1]);
					centerPointArray.clear();
					detectedAreas.clear();
				}
			}
		} else{continue;}


	}
	

	

	imshow("Thresholded Image", imgThresholded); //show the thresholded image
	imshow("Original", imgOriginal); //show the original image



 
        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
            break; 
       }
    }



  // return 0;

}

//****************************CODE NIELAB ENDE********************************