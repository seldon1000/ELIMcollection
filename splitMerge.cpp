#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int T = 50;

struct region
{
	bool active;
	Scalar color;
	Rect roi;
	vector<region> childs;
};

bool verify(Mat src)
{
	Scalar mean, s;

	meanStdDev(src, mean, s);

	if (s[0] < 5.8 || src.rows * src.cols < T)
		return true;

	return false;
}

region split(Mat src, Rect roi)
{
	region R;
	R.roi = roi;

	if (verify(src))
	{
		Scalar mean, s;

		meanStdDev(src, mean, s);

		R.color = mean;
	}
	else
	{
		int w = src.cols / 2, h = src.rows / 2;

		R.childs.push_back(split(src(Rect(0, 0, w, h)), Rect(roi.x, roi.y, w, h)));
		R.childs.push_back(split(src(Rect(w, 0, w, h)), Rect(roi.x + w, roi.y, w, h)));
		R.childs.push_back(split(src(Rect(0, h, w, h)), Rect(roi.x, roi.y + h, w, h)));
		R.childs.push_back(split(src(Rect(w, h, w, h)), Rect(roi.x + w, roi.y + h, w, h)));
	}

	return R;
}

bool regionUnion(Mat src, region r1, region r2)
{
	if (r1.childs.size() < 1 && r2.childs.size() < 1)
	{
		Rect r12 = r1.roi | r2.roi;
		
		if (verify(src(r12)))
		{
			r1.active = true;
			r1.roi = r12;
			r2.active = false;
			
			return true;
		}
	}

	return false;
}

void merge(Mat src, region &R)
{
	bool row1, row2, col1, col2;

	row1 = regionUnion(src, R.childs[0], R.childs[1]);
	row2 = regionUnion(src, R.childs[2], R.childs[3]);

	if (!row1 && !row2)
	{
		col1 = regionUnion(src, R.childs[0], R.childs[2]);
		col2 = regionUnion(src, R.childs[1], R.childs[3]);
	}

	for (int i = 0; i < R.childs.size(); i++)
	{
		if (R.childs[i].childs.size() > 0)
			merge(src, R.childs[i]);
	}
}

void color(Mat src, region R)
{
	if (R.childs.size() < 1 && R.active)
		rectangle(src, R.roi, R.color, CV_FILLED);

	for (int i = 0; i < R.childs.size(); i++)
		color(src, R.childs[i]);
}

int main()
{
	Mat src = imread("./Images/apples.jpg");
	region R = split(src, Rect(0, 0, src.rows, src.cols));

	merge(src, R);
	color(src, R);

	imshow("Split - Merge", src);
	waitKey(0);
}
