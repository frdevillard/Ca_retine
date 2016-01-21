/*
 * VideoStream.h
 *
 *  Created on: 29 août 2015
 *      Author: pierre
 */

#include <string>
#include <opencv2/opencv.hpp>
#include <time.h>

#include "Reseau.h"

#ifndef SRC_VIDEOSTREAM_H_
#define SRC_VIDEOSTREAM_H_

using namespace cv;
using namespace std;

class VideoStream {
public:
	VideoStream(int kScale, int lSmoothing, float a, string vidPath="0", int frameSkip=0);
	virtual ~VideoStream();
	int computeVideo(int cam=0);
	float getA() const;
	void setA(float a);
	int getKScale() const;
	void setKScale(int scale);
	int getLSmoothing() const;
	void setLSmoothing(int smoothing);
	const string& getVidPath() const;
	void setVidPath(const string& vidPath);

private:
	int kScale;
	int lSmoothing;
	float a;
	string vidPath;
	int frameSkip;

};

#endif /* SRC_VIDEOSTREAM_H_ */
