#include"utilies.h"

cv::Point CrossPoint(const LINE *line1, const LINE *line2)
{
	//	if(!SegmentIntersect(line1->pStart, line1->pEnd, line2->pStart, line2->pEnd))
	//	{// segments not cross	
	//		return 0;
	//	}
	cv::Point pt;
	// line1's cpmponent
	double X1 = line1->pEnd.x - line1->pStart.x;//b1
	double Y1 = line1->pEnd.y - line1->pStart.y;//a1
	// line2's cpmponent
	double X2 = line2->pEnd.x - line2->pStart.x;//b2
	double Y2 = line2->pEnd.y - line2->pStart.y;//a2
	// distance of 1,2
	double X21 = line2->pStart.x - line1->pStart.x;
	double Y21 = line2->pStart.y - line1->pStart.y;
	// determinant
	double D = Y1*X2 - Y2*X1;// a1b2-a2b1
	// 
	if (D == 0) return 0;
	// cross point
	pt.x = (X1*X2*Y21 + Y1*X2*line1->pStart.x - Y2*X1*line2->pStart.x) / D;
	// on screen y is down increased ! 
	pt.y = -(Y1*Y2*X21 + X1*Y2*line1->pStart.y - X2*Y1*line2->pStart.y) / D;
	// segments intersect.
	if ((abs(pt.x - line1->pStart.x - X1 / 2) <= abs(X1 / 2)) &&
		(abs(pt.y - line1->pStart.y - Y1 / 2) <= abs(Y1 / 2)) &&
		(abs(pt.x - line2->pStart.x - X2 / 2) <= abs(X2 / 2)) &&
		(abs(pt.y - line2->pStart.y - Y2 / 2) <= abs(Y2 / 2)))
	{
		return pt;
	}
	return 0;
}



LINE getDotlinePoint(cv::Point _s, cv::Point _e,cv::Size sz)
{
	cv::Point s, e;
	if (_s.x <= _e.x)
	{
		s = _s;
		e = _e;
	}
	else
	{
		s = _e;
		e = _s;
	}

	CvPoint pa, pb;

	double k = (s.y - e.y) / (s.x - e.x + 0.000001);//不加0.000001 会变成曲线，斜率可能为0，即e.x-s.x可能为0  

	double h = sz.height, w = sz.width;

	pa.y = s.y - k*s.x;
	if (pa.y < 0)
	{
		pa.y = 0;
		pa.x = s.x - s.y / k;
	}
	else if (pa.y > h)
	{
		pa.y = h;
		pa.x = s.x + (h - s.y) / k;
	}

	else
	{
		pa.x = 0;
	}

	//y = kx +b 
	pb.y = e.y + k*(w - e.x);
	if (pb.y < 0)
	{
		pb.y = 0;
		pb.x = e.x - e.y / k;
	}
	else if (pb.y > h)
	{
		pb.y = h;
		pb.x = e.x + (h-e.y) / k;
	}
	else
	{
		pb.x = w;
	}
	

	

	LINE l;
	l.pStart = pa;
	l.pEnd = pb;

	

	return l;
	//line(workimg, e, pa, CV_RGB(0, 255, 255));   //向右画线  
	//line(workimg, pb, s, CV_RGB(0, 0, 255)); //向左画线  

}


void OnDrawDotline(cv::Point _s, cv::Point _e, cv::Mat &workimg)
{
	cv::Point s, e;
	if (_s.x <= _e.x)
	{
		s = _s;
		e = _e;
	}
	else
	{
		s = _e;
		e = _s;
	}

	CvPoint pa, pb;

	double k = (s.y - e.y) / (s.x - e.x + 0.000001);//不加0.000001 会变成曲线，斜率可能为0，即e.x-s.x可能为0  

	double h = workimg.rows, w = workimg.cols;

	/*
	pa.x = w;
	pa.y = s.y + k*(w - s.x);
	pb.y = (e.y - k*e.x);
	pb.x = 0;
	*/

	pa.y = s.y - k*s.x;
	if (pa.y < 0)
	{
		pa.y = 0;
		pa.x = s.x - s.y / k;
	}
	else if (pa.y > h)
	{
		pa.y = h;
		pa.x = s.x + (h - s.y) / k;
	}

	else
	{
		pa.x = 0;
	}

	//y = kx +b 
	pb.y = e.y + k*(w - e.x);
	if (pb.y < 0)
	{
		pb.y = 0;
		pb.x = e.x - e.y / k;
	}
	else if (pb.y > h)
	{
		pb.y = h;
		pb.x = e.x + (h - e.y) / k;
	}
	else
	{
		pb.x = w;
	}





	line(workimg, pa, s, CV_RGB(255, 255, 255));   //向右画线 
	line(workimg, e, pb, CV_RGB(255, 255, 255)); //向左画线  

	//printf("inline = %d,%d,%d,%d\n", pa.x, pa.y, pb.x, pb.y);
}