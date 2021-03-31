#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

vector<Vec3f> houghCircles(Mat src, int min, int max, int T)
{
	int size[] = {max - min + 1, src.rows, src.cols};
	Mat acc = Mat::zeros(3, size, CV_8U);

	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			if (src.at<uchar>(i, j) > 250)
			{
				for (int r = min; r <= max; r++)
				{
					for (int t = 0; t < 360; t++)
					{
						double A = abs(i - r * cos(t * M_PI / 180));
						double B = abs(j - r * sin(t * M_PI / 180));

						acc.at<uchar>(r - min, A, B)++;
					}
				}
			}
		}
	}

	vector<Vec3f> circles;

	for (int x = 0; x < size[0]; x++)
	{
		for (int y = 0; y < size[1]; y++)
		{
			for (int z = 0; z < size[2]; z++)
			{
				if (acc.at<uchar>(x, y, z) > T)
					circles.push_back(Vec3f(x + min, y, z));
			}
		}
	}

	return circles;
}

int main()
{
	Mat src = imread("./Immagini/road.jpg", 0);
	Mat srcColor = imread("./Immagini/road.jpg");

	blur(src, src, Size(7, 7));
	Canny(src, src, 50, 150, 3);

	vector<Vec3f> circles = houghCircles(src, 4, 10, 80);

	for (int i = 0; i < circles.size(); i++)
	{
		Point center(cvRound(circles[i][2]), cvRound(circles[i][1]));
		int rad = cvRound(circles[i][0]);

		circle(srcColor, center, rad, Scalar(0, 0, 255), 1, 8, 0);
	}

	imshow("Hough Circles", srcColor);
	waitKey(0);
}
