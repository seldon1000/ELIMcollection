#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

float **gradDir;

Mat roberts(Mat src)
{
	Mat out = src.clone();
	int g_x[] = {0, 0, 0,
				 0, -1, 0,
				 0, 0, 1};
	int g_y[] = {0, 0, 0,
				 0, 0, -1,
				 0, 1, 0};

	for (int i = 1; i < src.rows - 1; i++)
	{
		for (int j = 1; j < src.cols - 1; j++)
		{
			int gradX = 0, gradY = 0, index = 0;

			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					gradX += src.at<uchar>(i + x, j + y) * g_x[index];
					gradY += src.at<uchar>(i + x, j + y) * g_y[index++];
				}
			}

			gradDir[i][j] = atan2(gradY, gradX);
			out.at<uchar>(i, j) = abs(gradX) + abs(gradY);
		}
	}

	return out;
}

Mat sopp(Mat src)
{
	Mat out = src.clone();

	for (int i = 1; i < src.rows - 1; i++)
	{
		for (int j = 1; j < src.cols - 1; j++)
		{
			if (gradDir[i][j] > (3 / 4) / M_PI)
			{
				if (src.at<uchar>(i, j) < src.at<uchar>(i - 1, j) ||
					src.at<uchar>(i, j) < src.at<uchar>(i + 1, j) ||
					src.at<uchar>(i, j) < src.at<uchar>(i, j - 1) ||
					src.at<uchar>(i, j) < src.at<uchar>(i, j + 1))
					out.at<uchar>(i, j) = 0;
			}
		}
	}

	return out;
}

Mat canny(Mat src, int min, int max)
{
	Mat out = src.clone();

	for (int i = 1; i < src.rows - 1; i++)
	{
		for (int j = 1; j < src.cols - 1; j++)
		{
			if (src.at<uchar>(i, j) < min)
				out.at<uchar>(i, j) = 0;
			else if (src.at<uchar>(i, j) > max)
				out.at<uchar>(i, j) = 255;
			else if (src.at<uchar>(i - 1, j) > max ||
					 src.at<uchar>(i + 1, j) > max ||
					 src.at<uchar>(i, j) > max ||
					 src.at<uchar>(i, j + 1) > max)
				out.at<uchar>(i, j) = 255;
			else
				out.at<uchar>(i, j) = 0;
		}
	}

	return out;
}

int main()
{
	Mat src = imread("./Immagini/apples.jpg", 0);

	gradDir = new float *[src.rows];
	for (int i = 0; i < src.rows; i++)
		gradDir[i] = new float[src.cols];

	blur(src, src, Size(3, 3));
	src = roberts(src);
	src = sopp(src);
	src = canny(src, 4, 10);

	imshow("Canny", src);
	waitKey(0);
}
