#include "ShapesDetector.h"

using namespace std;
using namespace cv;

Point rectangleCenterHours(0,0);
Point rectangleCenterMinutes(0,0);

ShapesDetector::ShapesDetector(void)
{
}


ShapesDetector::~ShapesDetector(void)
{
}

Point ShapesDetector::getTriangleCenter(Mat image) {

	//original Image for later access
	//originalImg = image;

    //cvtColor(image, image, COLOR_BGR2HSV);
    Mat binaryImage;
    Point triangleCenter;
    
    inRange(image, Scalar(50,150,0), Scalar(90,255,255), binaryImage);

	imshow("binaryImg", binaryImage);
    
    morphologyEx(binaryImage, binaryImage, MORPH_OPEN, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    
    // Konturen finden
    vector<vector<Point>> contours;
    findContours(binaryImage.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    vector<Point> approx;

    for (int i=0; i<contours.size(); i++) {
        
        // Ungefaehre Kontur
        approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.05, true);
        
        // Kleine und offene Formen ausschliessen
        if (fabs(contourArea(contours[i])) < 100 || !isContourConvex(approx)) {
            continue;
        }
        
        cout << "Kanten: " << approx.size() << endl;
        
        if (approx.size() == 3) {
            Moments M = moments(contours[i]);
            triangleCenter = cv::Point(int(M.m10/M.m00), int(M.m01/M.m00));
			cout << "triangleCenter: " << triangleCenter << endl;
        }
    }
    
   
    
    return triangleCenter;
}

void ShapesDetector::detectRectangles(Mat image){

    //VideoCapture cap(1); //capture the video from web cam

    //if ( !cap.isOpened() )  // if not success, exit program
    //{
    //     cout << "Cannot open the web cam" << endl;
    //    // return -1;
    //}

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
 
    //while (true)
    //{
        //Mat imgOriginal;

        //bool bSuccess = cap.read(imgOriginal); // read a new frame from video

        // if (!bSuccess) //if not success, break loop
        //{
        //     cout << "Cannot read a frame from video stream" << endl;
        //     break;
        //}

	//Mat imgHSV;
	//cvtColor(originalImg, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
 
	Mat imgThresholded;

	inRange(image, Scalar(0, 48,117), Scalar(94, 255, 255), imgThresholded); //Threshold the image
	//inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

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

	for (int i = 0; i < contours.size(); i++){
		// Approximate contour with accuracy proportional
		// to the contour perimeter
		 approxPolyDP( Mat(contours[i]), approx,  arcLength( Mat(contours[i]), true)*0.02, true);

		 if (fabs(contourArea(contours[i])) < 100 || !isContourConvex(approx))
			continue;

		if (approx.size() == 4)
		{
			Moments M = moments (contours[i]);
			//setLabel(imgOriginal, "Rec", contours[i]);
			detectedAreas.push_back(fabs(contourArea(contours[i])));
			centerPointArray.push_back(Point((M.m10/M.m00),(M.m01/M.m00)));
			cout << detectedAreas.size() <<endl;
			if(detectedAreas.size() == 2){	
					compareAreas(detectedAreas[0], detectedAreas[1], centerPointArray[0], centerPointArray[1]);
					centerPointArray.clear();
					detectedAreas.clear();				
			}
		} else{continue;}
	}	

	imshow("Thresholded Image", imgThresholded); //show the thresholded image
	
 }

void ShapesDetector::compareAreas(double areaOne, double areaTwo, Point areaOneCenter, Point areaTwoCenter){

	//double tRad = atan2(2.0,2.0);

	double smallArea;
	double bigArea;
	Point centerMinutes, centerHours;

		if (areaOne < areaTwo){
			smallArea = areaOne;
			centerMinutes = areaOneCenter;
			setRectangleCenterMinutes(areaOneCenter);

			bigArea = areaTwo;
			centerHours = areaTwoCenter;
			setRectangleCenterHours(areaTwoCenter);

			cout << "Grosser Zeiger: " << bigArea << " \nKleiner Zeiger: " << smallArea << endl;
			cout << "Grosser Zeiger Center: " << centerMinutes << " \nKleiner Zeiger Center: " << centerHours << endl;
			cout << "areaOne < areaTwo" << getRectangleCenterMinutes() << " -- "<< getRectangleCenterHours() << endl;
		}
		else if(areaOne > areaTwo){
			smallArea = areaTwo;
			centerMinutes = areaTwoCenter;
			setRectangleCenterMinutes(areaTwoCenter);

			bigArea = areaOne;
			centerHours = areaOneCenter;
			setRectangleCenterHours(areaOneCenter);
	
			cout << "Grosser Zeiger: " << bigArea << " \nKleiner Zeiger: " << smallArea << endl;
			cout << "Grosser Zeiger Center: " << centerMinutes << " \nKleiner Zeiger Center: " << centerHours << endl;
			cout << "areaOne > areaTwo" << getRectangleCenterMinutes() << " -- "<< getRectangleCenterHours() << endl;
		}
		else{
			cout << "Fehler bei Vergleich der Flächeninhalte" << endl;
		}
}

void ShapesDetector::setRectangleCenterHours(Point area){
	rectangleCenterHours = area;
}

Point ShapesDetector::getRectangleCenterHours(){
	return rectangleCenterHours;
}

void ShapesDetector::setRectangleCenterMinutes(Point area){
	rectangleCenterMinutes = area;
}

Point ShapesDetector::getRectangleCenterMinutes(){
	return rectangleCenterMinutes;
}

