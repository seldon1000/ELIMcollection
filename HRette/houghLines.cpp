#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

vector<Vec2d> houghLines(Mat src, int T)
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

	vector<Vec2d> lines;

	for (int i = 0; i < Rho; i++)
	{
		for (int j = 0; j < Theta; j++)
		{
			if (acc.at<uchar>(i, j) > T)
				lines.push_back(Vec2d(i, j * M_PI / 180));
		}
	}

	return lines;
}

int main()
{
	Mat src = imread("./Images/road.jpg", 0);
	Mat srcColor = imread("./Images/road.jpg");

	blur(src, src, Size(7, 7));
	Canny(src, src, 50, 150, 3);

	vector<Vec2d> lines = houghLines(src, 80);

	for (int i = 0; i < lines.size(); i++)
	{
		double A = cos(lines[i][1]);
		double B = sin(lines[i][1]);
		double x0 = A * lines[i][0];
		double y0 = B * lines[i][0];

		Point p1, p2;
		p1.x = x0 + 1000 * -B;
		p1.y = y0 + 1000 * A;
		p2.x = x0 - 1000 * -B;
		p2.y = y0 - 1000 * A;

		line(srcColor, p1, p2, Scalar(0, 0, 255), 1, 8, 0);
	}

	imshow("Hough Lines", srcColor);
	waitKey(0);
}
