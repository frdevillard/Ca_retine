/*
 * ImageMode.cpp
 *
 *  Created on: 29 août 2015
 *      Author: pierre
 */

#include "ImageMode.h"

ImageMode::ImageMode(string imPath, int kScale, int lSmoothing) :
		imPath(imPath), kScale(kScale), lSmoothing(lSmoothing) {
	// TODO Auto-generated constructor stub

}

ImageMode::~ImageMode() {
	// TODO Auto-generated destructor stub
}

int ImageMode::computeImage() {
	Mat image;
	image = imread(imPath, CV_LOAD_IMAGE_GRAYSCALE);   // Read the file
	if (!image.data)                              // Check for invalid input
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}

	Reseau *maillage = new Reseau(image, kScale, lSmoothing, 1);

	maillage->supervise();

	Mat image2 = maillage->createMat();

	delete maillage;
	cout << "appuyez sur s pour sauver l'image résultat" << endl;
	namedWindow("Image de sortie", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Image de sortie", image2);              // Show our image inside it.
	namedWindow("Image Source", CV_WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Image Source", image);                  // Show our image inside it.
	char key = (char) waitKey(0);          // Wait for a keystroke in the window
	if (key == 's' || key == 'S') {
		string outPath = imPath.substr(0, imPath.find_last_of(".")) + "_output"
				+ to_string(kScale) + "_" + to_string(lSmoothing) + ".jpg";
		imwrite(outPath, image2);
		cout << "Saved " << outPath << endl;
	}
	return 0;
}

int ImageMode::getKScale() const {
	return kScale;
}

void ImageMode::setKScale(int scale) {
	kScale = scale;
}

int ImageMode::getLSmoothing() const {
	return lSmoothing;
}

void ImageMode::setLSmoothing(int smoothing) {
	lSmoothing = smoothing;
}

const string& ImageMode::getImPath() const {
	return imPath;
}

void ImageMode::setImPath(const string& imPath) {
	this->imPath = imPath;
}
