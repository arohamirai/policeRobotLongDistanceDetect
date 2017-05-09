#include <stdio.h>
#include "opencv2/opencv.hpp"
#include <iostream>
#include"utilies.h"
#include"hikfun.h"



void on_mouse(int Event, int x, int y, int flags, void *userdata);
void createDistanceMap();

void loadDistanceMap(cv::Mat &disMat);