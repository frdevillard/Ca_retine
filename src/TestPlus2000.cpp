/*
 * TestPlus2000.cpp
 *
 *  Created on: 25 févr. 2015
 *      Author: pierre
 */

/*
 * Fichier de test à executer en console
 *
 */

#include <cstdlib>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "Reseau.h"
#include "Cell.h"
#include "VideoStream.h"
#include "ImageMode.h"

using namespace std;
using namespace cv;

void help(char** argv) {
	cout <<" Usage: "<< argv[0] <<" ImageToLoadAndDisplay <video flag> OU " << argv[0] << endl;
}


int main(int argc, char **argv) {

    if (argc != 3 && argc != 1) {
		help(argv);
		return 1;
	}

	int kScale = 0;
	int lSmoothing = 50;
	float a = 0.05;

	if (argc == 1) {
		VideoStream *videoStream = new VideoStream(kScale, lSmoothing, a);
		videoStream->computeVideo(1);
		delete videoStream;
	} else if (atoi(argv[2]) == 1) {
		VideoStream *videoStream = new VideoStream(kScale, lSmoothing, a, argv[1]);
		videoStream->computeVideo(0);
		delete videoStream;
	} else {
		ImageMode *imageMode = new ImageMode(argv[1], kScale, lSmoothing);
		imageMode->computeImage();
		delete imageMode;
	}
	return 0;
}




