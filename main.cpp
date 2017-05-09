#include <stdio.h>
#include <iostream>
#include "Windows.h"
#include "HCNetSDK.h"
#include "plaympeg4.h"
#include <time.h>
#include "opencv2/opencv.hpp"
#include "process.h"
#include"utilies.h"
#include"hikfun.h"
#include"createDistanceMap.h"
#include"segmentPerson.h"
using namespace std;
using namespace cv;


extern CRITICAL_SECTION g_cs_frameList;
extern Mat frame,picBGR;
extern int g_width, g_height;
extern int scale;

Mat ig = imread("123.jpg",1);
void main() 
{
	HANDLE hThread;
	unsigned threadID;
	Mat frame1;

	InitializeCriticalSection(&g_cs_frameList);
	hThread = (HANDLE)_beginthreadex(NULL, 0, &readCam, NULL, 0, &threadID);

	
	// 创建距离图
	/*

	namedWindow("pic",1);
	imshow("pic",ig);
	Sleep(4000);
	setMouseCallback("pic", on_mouse);

	while (waitKey(1) != 'q')
	{
		EnterCriticalSection(&g_cs_frameList);
		frame = picBGR.clone();
		LeaveCriticalSection(&g_cs_frameList);
	
	}
	createDistanceMap();
	return ;
	/**/


	//*
	// 检测                  
	Mat segmentationMap;        // Will contain the segmentation map. This is the binary output map. 
	vibeModel_Sequential_t *model = NULL; 

	
	Mat frame;
	/*
	VideoCapture cap("person.mp4");
	for (int i = 0; i < 30*30; i++)
	{
		cap >> frame;
	}
	/**/
	while (1)
	{
		//cap>>frame;
		//*
		EnterCriticalSection(&g_cs_frameList);
		frame = picBGR.clone();
		LeaveCriticalSection(&g_cs_frameList);
		/**/

		cout << "init" << endl;
		if (frame.empty()) continue;
		g_width = frame.cols;
		g_height = frame.rows;
		resize(frame, frame, cv::Size(g_width / scale, g_height / scale));
		
		segmentationMap = Mat(frame.rows, frame.cols, CV_8UC1);
		model = (vibeModel_Sequential_t*)libvibeModel_Sequential_New();
		libvibeModel_Sequential_AllocInit_8u_C3R(model, frame.data, frame.cols, frame.rows);

		break;
	}

	VideoWriter writer("FindPersonNoFilt.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, Size(g_width / scale, g_height / scale), true);
	VideoWriter writerF("FindPersonFilt.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, Size(g_width / scale, g_height / scale), true);
	RNG rng(12345);
	while (cv::waitKey(1) != 'q')
	{
		static int n = 0;
		//cout << n++ << endl;
		//cap >> frame;
		//cout << "detec" << endl;
		//*
		EnterCriticalSection(&g_cs_frameList);
		frame = picBGR.clone();
		LeaveCriticalSection(&g_cs_frameList);
		/**/
		if (frame.empty()) continue;
		resize(frame, frame, cv::Size(g_width / scale, g_height / scale));

		libvibeModel_Sequential_Segmentation_8u_C3R(model, frame.data, segmentationMap.data);
		libvibeModel_Sequential_Update_8u_C3R(model, frame.data, segmentationMap.data);


		medianBlur(segmentationMap, segmentationMap, 3); // 3x3 median filtering 
		dilate(segmentationMap, segmentationMap, Mat::ones(5, 5, CV_8UC1));
		erode(segmentationMap, segmentationMap, Mat::ones(5, 5, CV_8UC1));

		imshow("Frame", frame);
		imshow("Segmentation", segmentationMap);
		Mat segColor;
		cvtColor(segmentationMap, segColor, CV_GRAY2RGB);
		writer << segColor;

		cv::waitKey(1);

		// 背景分割
		double areaThresh = 1;
		vector<vector<Point> > contours, contours1;
		vector<Vec4i> hierarchy;
		findContours(segmentationMap, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		// 目标定位
		// 剔除面积过小的轮廓
		for (int i = 0; i<contours.size(); i++)
		{

			double tmpArea = fabs(contourArea(contours[i]));
			//std::cout<<tmpArea<<std::endl;
			//Rect aRect = boundingRect(contours[i]);
			if (tmpArea > areaThresh)
			{
				contours1.push_back(contours[i]);
			}
		}

		/// Draw contours
		//*
		Mat drawing = Mat::zeros(segmentationMap.size(), CV_8UC3);
		for (size_t i = 0; i< contours1.size(); i++)
		{
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(drawing, contours1, (int)i, color, -1, 8, -1, 0, Point());
		}
		/// Show in a window
		//namedWindow("Contours", WINDOW_AUTOSIZE);
		imshow("Contours", drawing);
		/**/
		writerF << drawing;



	}
	libvibeModel_Sequential_Free(model);
	/**/
	

	cv::waitKey(0);
	system("pause");
	return;
}

