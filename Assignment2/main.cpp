#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "sp_image_proc_util.h"
#include "main_aux.h"

using namespace cv;
using namespace std;

#define RGB_TEMP 3

int main() {

	//1
	printf("Enter images directory path:\n");
	fflush(NULL);
	char* dir;
	scanf("%s",&dir);

	//2
	printf("Enter images prefix:\n");
	fflush(NULL);
	char* pref;
	scanf("%s",&pref);

	//3
	printf("Enter number of images:\n");
	fflush(NULL);
	char* temp;
	scanf("%s",&temp);
	int num_imgs = atoi(temp);
	if(num_imgs < 1){ //bad arg
		printf("An error occurred - invalid number of images\n");
		//free alloc
		free(dir); free(pref); free(temp);
		return -1;
	}

	//4
	printf("Enter images suffix:\n");
	fflush(NULL);
	char* suff;
	scanf("%s",&suff);

	//5
	printf("Enter number of bins:\n");
	fflush(NULL);
	int num_bins;
	scanf("%d",&num_bins);
	if(num_bins < 1){ //bad arg
		printf("An error occurred - invalid number of bins\n");
		//free alloc
		free(dir); free(pref); free(temp); free(suff);
		return -1;
	}

	//6
	printf("Enter number of features:\n");
	fflush(NULL);
	int max_num_sift;
	scanf("%d",&max_num_sift);
	if(max_num_sift < 1){ //bad arg
		printf("An error occurred - invalid number of features\n");
		//free alloc
		free(dir); free(pref); free(temp); free(suff);
		return -1;
	}

	//7
	//init RGB histogram for each image in the images directory
	int*** theArray = initIMat(num_imgs, RGB_TEMP, num_bins);

	//calculate histogram for each image
	char* img_url, temp1, temp2;
	for (int i = 0; i < num_imgs; i++){
		//concat the image full url
		temp1 = concat(dir, pref);
		temp2 = concat((char*)i, suff);
		img_url = concat((char*)temp1,(char*)temp2);

		//calc hist for the i'th img
		theArray[i] = spGetRGBHist(img_url, num_bins);
	}

	return 0;
}
