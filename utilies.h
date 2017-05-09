#ifndef _UTILIES_H_
#define _UTILIES_H_

#include"iostream"
#include"vector"
#include"opencv2/opencv.hpp"

struct LINE
{
	cv::Point pStart;
	cv::Point pEnd;

	bool operator==(LINE l)
	{
		if (l.pStart.x == pStart.x && l.pStart.y == pStart.y && l.pEnd.x == pEnd.x && l.pEnd.y == pEnd.y)
			return true;
		else
			return false;
	};
};


//****************************************************************************************
//  �����ֱ�ߵĽ���Ĺ�ʽ
//  �����·���    a1*x+b1*y=c1
//                a2*x+b2*y=c2
//                x= | c1 b1|  / | a1 b1 |      y= | a1 c1| / | a1 b1 |
//                   | c2 b2|  / | a2 b2 |         | a2 c2| / | a2 b2 |
//
//   a1= (L1.pEnd.y-L1.pStart.y)   
//   b1= (L1.pEnd.x-L1.pStart.x)
//   c1= L1.pStart.x*(L1.pEnd.y-L1.pStart.y)-(L1.pEnd.x-L1.pStart.x)*L1.pStart.y
//   a2= (L2.pEnd.y-L2.pStart.y)   
//   b2= (L2.pEnd.x-L2.pStart.x)
//   c2= L2.pStart.x*(L2.pEnd.y-L2.pStart.y)-(L2.pEnd.x-L2.pStart.x)*L2.pStart.y  
cv::Point CrossPoint(const LINE *line1, const LINE *line2);


LINE getDotlinePoint(cv::Point _s, cv::Point _e, cv::Size sz);


std::vector<cv::Point> softPoint(std::vector<cv::Point> vecIn);


void OnDrawDotline(cv::Point s, cv::Point e, cv::Mat &workimg);


static int cmp_func(const void* _a, const void* _b, void* userdata)
{
	CvPoint* a = (CvPoint*)_a;
	CvPoint* b = (CvPoint*)_b;
	int y_diff = a->y - b->y;
	int x_diff = a->x - b->x;
	//return x_diff ? x_diff : 0;
	return y_diff ? y_diff : x_diff;//���ȱȽ�y���꣬a��y����������a��bλ��;����Ƚ�x���꣬a��x�����Ҳ����a��bλ�á�
} //������a,bλ�� swap(a,b) if (a->y >b->y)|(a->x >b->x)

#endif


