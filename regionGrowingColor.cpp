#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

bool **visited;
int T = 60;
Vec3b seed;
Mat src, regions;

double dist(Vec3b x)
{
	double distB = x.val[0] - seed.val[0];
	double distG = x.val[1] - seed.val[1];
	double distR = x.val[2] - seed.val[2];

	return sqrt(pow(distB, 2) + pow(distG, 2) + pow(distR, 2));
}

void regionGrowingColor(int row, int col)
{
	if (row > 0 && !visited[row - 1][col])
	{
		visited[--row][col] = true;

		if (dist(src.at<Vec3b>(row, col)) < T)
		{
			regions.at<Vec3b>(row, col) = seed;
			regionGrowing(row, col);
		}
	}

	if (col > 0 && !visited[row][col - 1])
	{
		visited[row][--col] = true;

		if (dist(src.at<Vec3b>(row, col)) < T)
		{
			regions.at<Vec3b>(row, col) = seed;
			regionGrowing(row, col);
		}
	}

	if (row < src.rows - 1 && !visited[row + 1][col])
	{
		visited[++row][col] = true;

		if (dist(src.at<Vec3b>(row, col)) < T)
		{
			regions.at<Vec3b>(row, col) = seed;
			regionGrowing(row, col);
		}
	}

	if (col < src.cols - 1 && !visited[row][col + 1])
	{
		visited[row][++col] = true;

		if (dist(src.at<Vec3b>(row, col)) < T)
		{
			regions.at<Vec3b>(row, col) = seed;
			regionGrowing(row, col);
		}
	}
}

int main()
{
	srand(time(NULL));

	src = imread("./Images/lena.jpg"), regions = src.clone();

	visited = new bool *[src.rows];
	for (int i = 0; i < src.rows; i++)
	{
		visited[i] = new bool[src.cols];
		for (int j = 0; j < src.cols; j++)
		{
			visited[i][j] = false;
			regions.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
		}
	}

	for (int i = 0; i < 3; i++)
	{
		int x = rand() % src.rows, y = rand() % src.cols;
		seed = src.at<Vec3b>(x, y);

		regionGrowingColor(x, y);
	}

	imshow("Region Growing Color", regions);
	waitKey(0);
}
