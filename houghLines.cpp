#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat srcColor;

void houghLines(Mat src, int T)
{
	double R = src.rows > src.cols ? src.rows : src.cols;
	int Rho = R * sqrt(2.0), Theta = 180;
	Mat acc = Mat::zeros(Rho, Theta, CV_8U);

	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			if (src.at<uchar>(i, j) > 250)
			{
				for (int t = 0; t < 180; t++)
				{
					R = abs(i * sin(t * M_PI / 180) + j * cos(t * M_PI / 180));

					acc.at<uchar>(round(R), t)++;
				}
			}
		}
	}

	for (int i = 0; i < Rho; i++)
	{
		for (int j = 0; j < Theta; j++)
		{
			if (acc.at<uchar>(i, j) > T)
			{
				double A = cos(j * M_PI / 180);
				double B = sin(j * M_PI / 180);
				double x0 = A * i;
				double y0 = B * i;

				Point p1, p2;
				p1.x = x0 + 1000 * -B;
				p1.y = y0 + 1000 * A;
				p2.x = x0 - 1000 * -B;
				p2.y = y0 - 1000 * A;

				line(srcColor, p1, p2, Scalar(0, 0, 255), 1, 8, 0);
			}
		}
	}
}

int main()
{
	Mat src = imread("./Images/road.jpg", 0);
	srcColor = imread("./Images/road.jpg");

	blur(src, src, Size(7, 7));
	Canny(src, src, 50, 150, 3);

	houghLines(src, 80);

	imshow("Hough Lines", srcColor);
	waitKey(0);
}
