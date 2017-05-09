#include"createDistanceMap.h"

using namespace std;
using namespace cv;


extern CRITICAL_SECTION g_cs_frameList;
extern Mat lineImg, picBGR, frame;;
extern cv::Point point1, point2;
extern int scale;
extern int g_width, g_height;

extern std::vector<LINE> g_l;
extern bool bShouldReturn;



void on_mouse(int Event, int x, int y, int flags, void *userdata)
{
	if (frame.empty()) return;
	resize(frame, frame, cv::Size(g_width / scale, g_height / scale));

	//int p = 255;
	cv::Vec3i p(0, 0, 255);
	if (frame.empty())
		return;
	Mat copy = frame.clone();

	copy.setTo(p, lineImg);
	imshow("pic", copy);

	if (Event == CV_EVENT_LBUTTONDOWN)
	{
		point1 = cvPoint(x, y);
	}
	if (Event == CV_EVENT_MOUSEMOVE && flags == CV_EVENT_FLAG_LBUTTON)
	{
		line(copy, point1, Point(x, y), CV_RGB(0, 255, 0));
		OnDrawDotline(point1, Point(x, y), copy);
		imshow("pic", copy);
	}
	if (Event == CV_EVENT_LBUTTONUP)
	{
		point2 = cvPoint(x, y);
		cv::line(lineImg, point1, point2, CV_RGB(255, 255, 255));
		OnDrawDotline(point1, Point(x, y), lineImg);
		g_l.push_back(getDotlinePoint(point1, point2, copy.size()));
	}
	/*
	if (Event == EVENT_RBUTTONUP)
	{
	int l = 0;
	scanf("%d", &l);
	char pt[15];
	sprintf_s(pt, "%d", l);
	putText(lineImg, pt, cv::Point(x, y), FONT_HERSHEY_SIMPLEX, 1, CV_RGB(255, 255, 255));
	}
	*/
	copy.setTo(p, lineImg);
	imshow("pic", copy);
	waitKey(1);
}


void createDistanceMap()
{
	imwrite("line.bmp", lineImg);

	//添加四条外框直线
	LINE r0, r1, c0, c1;
	r0.pStart = Point(0, 0);
	r0.pEnd = Point(g_width / scale, 0);

	r1.pStart = Point(0, g_height / scale);
	r1.pEnd = Point(g_width / scale, g_height / scale);

	c0.pStart = Point(0, 0);
	c0.pEnd = Point(0, g_height / scale);

	c1.pStart = Point(g_width / scale, 0);
	c1.pEnd = Point(g_width / scale, g_height / scale);

	line(lineImg, r0.pStart, r0.pEnd, CV_RGB(255, 255, 255), 1);
	line(lineImg, r1.pStart, r1.pEnd, CV_RGB(255, 255, 255), 1);
	line(lineImg, c0.pStart, c0.pEnd, CV_RGB(255, 255, 255), 1);
	line(lineImg, c1.pStart, c1.pEnd, CV_RGB(255, 255, 255), 1);

	g_l.push_back(r0);
	g_l.push_back(r1);
	g_l.push_back(c0);
	g_l.push_back(c1);

	/*
	// 保存直线
	FILE *pw = fopen("line.dat", "w");

	fprintf(pw, "%d\n", g_l.size());
	for (auto it = g_l.begin(); it != g_l.end(); ++it)
	{
		Point s, e;
		s = it->pStart;
		e = it->pEnd;
	}
	fclose(pw);
	*/

	/*
	// 读取并显示直线
	FILE *pr = fopen("line.dat", "r");

	int n = 0;
	fscanf(pr, "%d\n", &n);
	for (int i = 0; i < n; i++)
	{
	Point s, e;
	fscanf(pr, "%d %d %d %d\n", &s.x, &s.y, &e.x, &e.y);
	printf("%d %d %d %d\n", s.x, s.y, e.x, e.y);
	}
	*/

	//行和列直线分开
	std::vector<LINE> r_l, c_l;
	for (int i = 0; i < g_l.size(); i++)
	{
		if (g_l[i].pStart.x == 0 && g_l[i].pEnd.x != 0)
			r_l.push_back(g_l[i]);
		else if (g_l[i].pStart.y == 0 && g_l[i].pEnd.y != 0)
			c_l.push_back(g_l[i]);
	}
	//排序直线
	// 行排列
	for (int i = 0; i < r_l.size() - 1; i++)
	{
		for (int j = i + 1; j < r_l.size(); j++)
		{
			LINE lt;
			if (r_l[i].pStart.y > r_l[j].pStart.y)
			{
				lt = r_l[i];
				r_l[i] = r_l[j];
				r_l[j] = lt;
			}
		}
	}
	// 列排列
	for (int i = 0; i < c_l.size() - 1; i++)
	{
		for (int j = i + 1; j < c_l.size(); j++)
		{
			LINE lt;
			if (c_l[i].pStart.x > c_l[j].pStart.x)
			{
				lt = c_l[i];
				c_l[i] = c_l[j];
				c_l[j] = lt;
			}
		}
	}

	// 求交点
	std::vector<std::vector<cv::Point>> vecCP_r;
	for (int i = 0; i < r_l.size(); i++)
	{
		std::vector<cv::Point> vecCP;
		for (int j = 0; j <c_l.size(); j++)
		{
			Point p = CrossPoint(&r_l[i], &c_l[j]);
			vecCP.push_back(p);
		}
		vecCP_r.push_back(vecCP);

	}
	/*
	// 验证是否找对交点
	for (int i = 0; i < vecCrossPoint.size(); i++)
	{
	circle(frame, vecCrossPoint[i], 3, CV_RGB(255, 0, 0));
	}
	imshow("crossPoint", frame);
	waitKey(1);
	*/

	/*
	// 查看是否正确：
	int n = 0;
	for (int i = 0; i < vecCP_r.size(); i++)
	{
		for (int j = 0; j < vecCP_r[i].size(); j++)
		{
			char text[5];
			sprintf(text, "%d", n++);
			putText(frame, text, vecCP_r[i][j], 1, 1, CV_RGB(255, 255, 255));
		}

	}
	imshow("crosspoint_soft", frame);
	imwrite("crosspoint_soft.jpg", frame);
	waitKey(1000);
	*/

	// 组合四边形区域
	std::vector<std::vector<Point>> vecRect;
	for (int i = 0; i < vecCP_r.size() - 1; i++)
	{
		for (int j = 0; j < vecCP_r[i].size() - 1; j++)
		{
			vector<Point> vec;
			vec.push_back(vecCP_r[i][j]);
			vec.push_back(vecCP_r[i][j + 1]);
			vec.push_back(vecCP_r[i + 1][j + 1]);
			vec.push_back(vecCP_r[i + 1][j]);

			vecRect.push_back(vec);
		}


	}

	//*
	// 保存四边形
	FILE *prect = fopen("rect.dat", "w");
	fprintf(prect, "%d\n", vecRect.size());
	fprintf(prect, "%d %d\n", g_width / scale, g_height / scale);
	for (int i = 0; i < vecRect.size(); i++)
	{
		fprintf(prect, "%d %d %d %d\n", vecRect[i][0], vecRect[i][1], vecRect[i][2], vecRect[i][3]);
	}
	fclose(prect);
	/**/

	// 显示分区域是否正确
	//*
	Mat distanceMapColor;
	distanceMapColor.create(Size(g_width / scale, g_height / scale), CV_8UC3);

	for (int i = 0; i < vecRect.size(); i++)
	{
		auto vec = vecRect[i];
		int num = (int)vec.size();
		const Point *pt = &(vec[0]);

		cv::Scalar color = CV_RGB(rand() % 255, rand() % 255, rand() % 255);
		fillPoly(distanceMapColor, &pt, &num, 1, color);
	}
	imshow("distanceMapColor", distanceMapColor);
	imwrite("distanceMapColor.jpg", distanceMapColor);
	waitKey(1);
	/**/


	// 保存距离映射图像
	//*
	Mat distanceMap;
	distanceMap.create(Size(g_width / scale, g_height / scale), CV_8UC1);
	

	int stepColor =  255 / (int)vecRect.size();
	for (int i = 0; i < vecRect.size(); i++)
	{
		auto vec = vecRect[i];
		int num = (int)vec.size();
		const Point *pt = &(vec[0]);

		cv::Scalar color = CV_RGB(stepColor*i, stepColor*i, stepColor*i);
		fillPoly(distanceMap, &pt, &num, 1, color);
	}
	imwrite("distanceMap.bmp", distanceMap);
	/**/
}