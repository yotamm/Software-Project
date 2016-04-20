#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "sp_image_proc_util.h"

using namespace cv;
using namespace std;

int main() {
	int temp = 100;
	char* img = "img9.png";
	spGetSiftDescriptors(img, 512, &temp);
	//Mat image, gray_image;
	//image = imread("img9.png", CV_LOAD_IMAGE_COLOR);
	//namedWindow( "Display window", WINDOW_AUTOSIZE );
	//imshow( "Display window", image);
	//waitKey(0);
	return 0;
}
