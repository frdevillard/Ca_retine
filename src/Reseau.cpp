/*
 * Reseau.cpp
 *
 *  Created on: 11 mars 2015
 *      Author: pierre
 */

#include "Reseau.h"
#include <cmath>

using namespace std;

int Reseau::hauteur = 0;
int Reseau::largeur = 0;
Cell** Reseau::reseau;
static Cell* voisinage[8];


Reseau::Reseau(cv::Mat matrix, int kScale, int lSmoothing, float a) :
		/*largeur(matrix.cols), hauteur(matrix.rows),*/ matrice(matrix) {
	hauteur = matrix.rows;
	largeur = matrix.cols;
	reseau = new Cell* [hauteur];
	for (int i = 0; i < hauteur; i++) {
		reseau[i] = new Cell[largeur];
	}

	float level;
	for (int i = 0; i < hauteur; i++) {
		for (int j = 0; j < largeur; j++) {
			level = (float)matrix.at<uchar>(i, j);
			reseau[i][j].setPosX(i);
			reseau[i][j].setPosY(j);
			reseau[i][j].setGreyLevel(level);
			reseau[i][j].setA(a);
			reseau[i][j].initM();
			reseau[i][j].setKScale(kScale);
			reseau[i][j].setLSmoothing(lSmoothing);

			reseau[i][j].setVoisinage(getVoisinage(i,j,0));
		}
	}
	//cout << "cellules initialisées" << endl;
}

Reseau::~Reseau() {
	for (int i = 0; i < hauteur; i++) {
		delete[] reseau[i];
	}
	delete[] reseau;
}

void Reseau::importFrame(cv::Mat matrix) {
	if (matrix.rows != hauteur || matrix.cols != largeur) {
		cout << "problème de dimensions de la frame \n";
		exit(1);
	}
	for (int i = 0; i < hauteur; i++) {
		for (int j = 0; j < largeur; j++) {
			reseau[i][j].setGreyLevel((float)matrix.at<uchar>(i, j));
			reseau[i][j].setSt(0);
			reseau[i][j].setVoisinage(getVoisinage(i,j,0));
		}
	}
}

Cell** Reseau::getReseau() {
	return reseau;
}

void Reseau::supervise() {
	vector <string> list_state = {"0","1a","1b","2a","2b","2c","3","4","5"};
	int nbCelluleStop = 0;
	while (nbCelluleStop < (hauteur * largeur)) {
		for (int i = 0; i < hauteur; i++) {
			for (int j = 0; j < largeur; j++) {
				reseau[i][j].iterate();
				if (reseau[i][j].getSt() == STATE_5)
					nbCelluleStop++;
				// Debug de la msf
				if ((i+j)==0) {
				cout << list_state[reseau[i][j].getSt()];
				if (reseau[i][j].getSt() == STATE_5) cout <<":";
				else cout <<",";}
			}
			// Debug fin iteration
			//cout << nbCelluleStop << " à jour\n";
		}
	}
}

cv::Mat Reseau::createMat() {
	//cout << "Debut de la creation de la matrice" << endl;
	cv::Mat *output = new cv::Mat(hauteur, largeur, CV_8U);
	for (int i = 0; i < hauteur; i++) {
		for (int j = 0; j < largeur; j++) {
			output->at<uchar>(i,j) = (uchar)reseau[i][j].getGreyLevel();
		}
	}
	return *output;
}

Cell** Reseau::getVoisinage(int posX, int posY, int k) {

	int pas = (int) pow(2, k);
	if (posX - pas > 0 && posY - pas > 0)
		voisinage[NORTH_WEST] = &Reseau::reseau[posX - pas][posY - pas];
	else if (posX - pas <= 0 && posY - pas <= 0)
		voisinage[NORTH_WEST] = &Reseau::reseau[posX][posY];
	else if (posX - pas <= 0)
		voisinage[NORTH_WEST] = &Reseau::reseau[posX][posY - pas];
	else
		voisinage[NORTH_WEST] = &Reseau::reseau[posX - pas][posY];

	if (posX - pas > 0)
		voisinage[NORTH] = &Reseau::reseau[posX - pas][posY];
	else
		voisinage[NORTH] = &Reseau::reseau[posX][posY];

	if (posX - pas > 0 && posY + pas < Reseau::largeur)
		voisinage[NORTH_EAST] = &Reseau::reseau[posX - pas][posY + pas];
	else if (posX - pas <= 0 && posY + pas >= Reseau::largeur)
		voisinage[NORTH_EAST] = &Reseau::reseau[posX][posY];
	else if (posX - pas <= 0)
		voisinage[NORTH_EAST] = &Reseau::reseau[posX][posY + pas];
	else
		voisinage[NORTH_EAST] = &Reseau::reseau[posX - pas][posY];

	if (posY + pas < Reseau::largeur)
		voisinage[EAST] = &Reseau::reseau[posX][posY + pas];
	else
		voisinage[EAST] = &Reseau::reseau[posX][posY];

	if (posX + pas < Reseau::hauteur && posY + pas < Reseau::largeur)
		voisinage[SOUTH_EAST] = &Reseau::reseau[posX + pas][posY + pas];
	else if (posX + pas >= Reseau::hauteur && posY + pas >= Reseau::largeur)
		voisinage[SOUTH_EAST] = &Reseau::reseau[posX][posY];
	else if (posX + pas >= Reseau::hauteur)
		voisinage[SOUTH_EAST] = &Reseau::reseau[posX][posY + pas];
	else
		voisinage[SOUTH_EAST] = &Reseau::reseau[posX + pas][posY];

	if (posX + pas < Reseau::hauteur)
		voisinage[SOUTH] = &Reseau::reseau[posX + pas][posY];
	else
		voisinage[SOUTH] = &Reseau::reseau[posX][posY];

	if (posX + pas < Reseau::hauteur && posY - pas > 0)
		voisinage[SOUTH_WEST] = &Reseau::reseau[posX + pas][posY - pas];
	else if (posX + pas >= Reseau::hauteur && posY - pas <= 0)
		voisinage[SOUTH_WEST] = &Reseau::reseau[posX][posY];
	else if (posX + pas >= Reseau::hauteur)
		voisinage[SOUTH_WEST] = &Reseau::reseau[posX][posY - pas];
	else
		voisinage[SOUTH_WEST] = &Reseau::reseau[posX + pas][posY];

	if (posY - pas > 0)
		voisinage[WEST] = &Reseau::reseau[posX][posY - pas];
	else
		voisinage[WEST] = &Reseau::reseau[posX][posY];

	return voisinage;
}

