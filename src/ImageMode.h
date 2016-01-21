/*
 * ImageMode.h
 *
 *  Created on: 29 août 2015
 *      Author: pierre
 */

#include <string>
#include <opencv2/opencv.hpp>
#include "Reseau.h"

#ifndef SRC_IMAGEMODE_H_
#define SRC_IMAGEMODE_H_

using namespace cv;
using namespace std;

class ImageMode {
public:
	ImageMode(string imPath, int kScale, int lSmoothing);
	virtual ~ImageMode();
	int computeImage();
	int getKScale() const;
	void setKScale(int scale);
	int getLSmoothing() const;
	void setLSmoothing(int smoothing);
	const string& getImPath() const;
	void setImPath(const string& ImPath);

private:
	string imPath;
	int kScale;
	int lSmoothing;
};

#endif /* SRC_IMAGEMODE_H_ */
