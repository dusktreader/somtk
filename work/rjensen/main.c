/* 
 * File:   main.c
 * Author: russell
 *
 * Created on March 29, 2010, 9:13 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>


#define IMAGE_FILE "../../lib/images/hs-2004-07-a-full_jpg.jpg"

void performMeanShiftFiltering()
{
	
	IplImage* src = cvLoadImage(IMAGE_FILE, CV_LOAD_IMAGE_COLOR);

	IplImage* dst = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);

	cvPyrMeanShiftFiltering(src, dst, 20, 30, 2, cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 5, 1));

	cvSaveImage("hubblefiltered.jpg", dst, 0);

	cvReleaseImage(&dst);
	cvReleaseImage(&src);
}

void segmentGalaxies()
{
	int threshold = 100;

	
	IplImage* dst = cvLoadImage("hubblefiltered.jpg", CV_LOAD_IMAGE_COLOR);

	//Now try to find the contours
	IplImage* gray = cvCreateImage(cvGetSize(dst), 8, 1);

	cvCvtColor(dst, gray, CV_BGR2GRAY);

	cvSaveImage("hubblgrayscale.jpg", gray, 0);

	IplImage* smoothed = cvCreateImage(cvGetSize(gray), 8, 1);

	cvMorphologyEx(gray, smoothed, 0, 0, CV_MOP_OPEN, 3);
	
	//cvSmooth(gray, smoothed, CV_GAUSSIAN, 3, 0, 0, 0);

	cvSaveImage("hubblesmoothed.jpg", smoothed, 0);
	
	cvThreshold(smoothed, smoothed, threshold, 255, CV_THRESH_BINARY);

	CvMemStorage* storage1 = cvCreateMemStorage(0);

	CvSeq* contours1 = NULL;

	cvFindContours(smoothed, storage1, &contours1, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));

	cvDrawContours(smoothed, contours1, cvScalarAll(255), cvScalarAll(255), 100, 1, 8, cvPoint(0,0));

	cvSaveImage("hubblecontours.jpg", smoothed, 0);

	cvReleaseImage(&dst);
	cvReleaseImage(&gray);
	cvReleaseImage(&smoothed);
}

/*
 * 
 */
int main(int argc, char** argv) {

	performMeanShiftFiltering();
	segmentGalaxies();

	return (EXIT_SUCCESS);
}

