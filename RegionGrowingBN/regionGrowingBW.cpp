#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

bool **visited;
int seed[] = {40, 0}, T = 60;
Mat src, regions;

void regionGrowing(int row, int col)
{
	if (row > 0 && !visited[row - 1][col])
	{
		if (src.at<uchar>(--row, col) - seed[0] > T)
			regions.at<uchar>(row, col) = seed[0];

		visited[row][col] = true;
		regionGrowing(row, col);
	}

	if (col > 0 && !visited[row][col - 1])
	{
		if (src.at<uchar>(row, --col) - seed[0] > T)
			regions.at<uchar>(row, col) = seed[0];

		visited[row][col] = true;
		regionGrowing(row, col);
	}

	if (row < src.rows - 1 && !visited[row + 1][col])
	{
		if (src.at<uchar>(++row, col) - seed[0] > T)
			regions.at<uchar>(row, col) = seed[0];

		visited[row][col] = true;
		regionGrowing(row, col);
	}

	if (col < src.cols - 1 && !visited[row][col + 1])
	{
		if (src.at<uchar>(row, ++col) - seed[0] > T)
			regions.at<uchar>(row, col) = seed[0];

		visited[row][col] = true;
		regionGrowing(row, col);
	}
}

int main()
{
	src = imread("./Images/lena.jpg", 0);
	resize(src, src, Size(), 0.5, 0.5);
	regions = src.clone();

	visited = new bool *[src.rows];
	for (int i = 0; i < src.rows; i++)
	{
		visited[i] = new bool[src.cols];
		for (int j = 0; j < src.cols; j++)
		{
			visited[i][j] = false;
			regions.at<uchar>(i, j) = seed[1];
		}
	}

	regionGrowing(0, 0);

	imshow("Region Growing", regions);
	waitKey(0);
}
