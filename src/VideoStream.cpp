/*
 * VideoStream.cpp
 *
 *  Created on: 29 août 2015
 *      Author: pierre
 */

#include "VideoStream.h"
#include <string>

VideoStream::VideoStream(int kScale, int lSmoothing, float a, string vidPath,
		int frameSkip) :
		kScale(kScale), lSmoothing(lSmoothing), a(a), vidPath(vidPath), frameSkip(
				frameSkip) {
}

VideoStream::~VideoStream() {
	// TODO Auto-generated destructor stub
}

int VideoStream::computeVideo(int cam) {
	VideoCapture capture;
	if (cam != 0)
		capture.open(0);
	else
		capture.open(vidPath);

	int n = 0;
	char filename[200];
	string window_name = "video | q or esc to quit";
	cout << "press space to save a picture. q or esc to quit" << endl;
	namedWindow(window_name, CV_WINDOW_AUTOSIZE); //resizable window;
	namedWindow("source", CV_WINDOW_AUTOSIZE);

	Mat frame;
	Mat outFrame;
	capture >> frame;
	imshow("source", frame);
	cvtColor(frame, outFrame, CV_BGR2GRAY);
	Reseau *maillage = new Reseau(outFrame, kScale, lSmoothing, a);
	maillage->supervise();
	outFrame = maillage->createMat();
	time_t start, end;
	time(&start);
	int counter = 0;

	for (;;) {
		imshow(window_name, outFrame);

		for (int i = 0; i < 0; i++)
			capture.grab();

		capture >> frame;
		if (frame.empty())
			break;

		imshow("source", frame);
		cvtColor(frame, outFrame, CV_BGR2GRAY);
		maillage->importFrame(outFrame);

		maillage->supervise();
		outFrame = maillage->createMat();

		time(&end);
		++counter;
		double sec = difftime(end, start);
		double fps = counter / sec;
		cout << "FPS : " << fps << endl;

		char key = (char) waitKey(5); //delay N millis, usually long enough to display and capture input
		switch (key) {
		case 'q':
		case 'Q':
		case 27: //escape key
			exit(0);
		case ' ': //Save an image
			sprintf(filename, "filename%d_%d%.3d.jpg", kScale, lSmoothing, n++);
			imwrite(filename, outFrame);
			cout << "Saved " << filename << endl;
			break;
		default:
			break;
		}
	}
	delete maillage;
	return 0;
}

float VideoStream::getA() const {
	return a;
}

void VideoStream::setA(float a) {
	this->a = a;
}

int VideoStream::getKScale() const {
	return kScale;
}

void VideoStream::setKScale(int scale) {
	kScale = scale;
}

int VideoStream::getLSmoothing() const {
	return lSmoothing;
}

void VideoStream::setLSmoothing(int smoothing) {
	lSmoothing = smoothing;
}

const string& VideoStream::getVidPath() const {
	return vidPath;
}

void VideoStream::setVidPath(const string& vidPath) {
	this->vidPath = vidPath;
}
