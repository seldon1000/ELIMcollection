#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat srcColor, g_x, g_y;

Mat BW(Mat src)
{
	Mat out = src.clone();

	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			if (src.at<uchar>(i, j) < 128)
				out.at<uchar>(i, j) = 0;
			else
				out.at<uchar>(i, j) = 255;
		}
	}

	return out;
}

void roberts(Mat src)
{
	g_x = Mat(src.rows, src.cols, CV_32F);
	g_y = g_x.clone();

	for (int i = 0; i < src.rows - 1; i++)
	{
		for (int j = 0; j < src.cols - 1; j++)
		{
			g_x.at<float>(i, j) = src.at<uchar>(i + 1, j + 1) - src.at<uchar>(i, j);
			g_y.at<float>(i, j) = src.at<uchar>(i + 1, j) - src.at<uchar>(i, j + 1);
		}
	}
}

void harris(Mat src, int T)
{
	for (int i = 3; i < src.rows - 3; i++)
	{
		for (int j = 3; j < src.cols - 3; j++)
		{
			float C00 = 0, C11 = 0, C10 = 0, det, traccia;

			for (int x = -3; x < 4; x++)
			{
				for (int y = -3; y < 4; y++)
				{
					C00 += pow(g_x.at<float>(i + x, j + y), 2);
					C11 += pow(g_y.at<float>(i + x, j + y), 2);
					C10 += g_x.at<float>(i + x, j + y) * g_y.at<float>(i + x, j + y);
				}
			}

			det = (C00 * C11) - (C10 * C10);
			traccia = C00 + C11;
			double R = det - 0.04 * pow(traccia, 2);

			if (R > T * 1e+11)
				circle(srcColor, Point(j, i), 5, Scalar(0, 0, 255), 2, 8, 0);
		}
	}
}

int main()
{
	Mat src = imread("./Images/ape2.jpg", 0);
	srcColor = imread("./Images/ape2.jpg");

	src = BW(src);
	roberts(src);
	harris(src, 8);

	imshow("Harris", srcColor);
	waitKey(0);
}
