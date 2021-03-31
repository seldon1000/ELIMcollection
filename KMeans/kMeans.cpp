#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

const int K = 5;

void kMeans(Mat src)
{
	int center;
	float min, dist;
	Vec3b val;
	Mat means(1, K, CV_8UC3);

	for (int i = 0; i < K; i++)
		means.at<Vec3b>(0, i) = src.at<Vec3b>(rand() % src.rows, rand() % src.cols);

	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			min = sqrt(pow(means.at<Vec3b>(0, 0).val[0] - src.at<Vec3b>(i, j).val[0], 2) +
					   pow(means.at<Vec3b>(0, 0).val[1] - src.at<Vec3b>(i, j).val[1], 2) +
					   pow(means.at<Vec3b>(0, 0).val[2] - src.at<Vec3b>(i, j).val[2], 2));

			for (int x = 1; x < K; x++)
			{
				dist = sqrt(pow(means.at<Vec3b>(0, x).val[0] - src.at<Vec3b>(i, j).val[0], 2) +
							pow(means.at<Vec3b>(0, x).val[1] - src.at<Vec3b>(i, j).val[1], 2) +
							pow(means.at<Vec3b>(0, x).val[2] - src.at<Vec3b>(i, j).val[2], 2));

				if (dist < min)
				{
					min = dist;
					center = x;
					val = means.at<Vec3b>(0, x);
				}

				src.at<Vec3b>(i, j) = val;

				src.at<Vec3b>(0, center).val[0] = (means.at<Vec3b>(0, center).val[0] + val.val[0]) / 2;
				src.at<Vec3b>(0, center).val[1] = (means.at<Vec3b>(0, center).val[1] + val.val[1]) / 2;
				src.at<Vec3b>(0, center).val[2] = (means.at<Vec3b>(0, center).val[2] + val.val[2]) / 2;
			}
		}
	}
}

int main()
{
	srand(time(NULL));

	Mat src = imread("./Images/lena.jpg");

	kMeans(src);

	imshow("K-Means", src);
	waitKey(0);
}
