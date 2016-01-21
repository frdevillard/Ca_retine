/*
 * Reseau.h
 *
 *  Created on: 11 mars 2015
 *      Author: pierre
 */

#include <opencv2/opencv.hpp>
#include "Cell.h"

#ifndef SRC_RESEAU_H_
#define SRC_RESEAU_H_

//class Cell;

class Reseau {
public:
	//Reseau(int largeur, int hauteur);
	Reseau(cv::Mat matrix, int kScale, int lSmoothing, float a);
	virtual ~Reseau();
	void importFrame(cv::Mat img);
	Cell** getReseau();

	/*
	 * Lance l'automate pour toutes les cellules jusqu'à sa terminaison
	 */
	void supervise();

	/*
	 * Crée la matrice pour l'export
	 */
	cv::Mat createMat();

	/*
	 * Retourne le voisinage correspondant à la cellule de position posX, posY
	 * et au niveau d'échelle k
	 */
	static Cell** getVoisinage(int posX, int posY, int k);
	static int largeur;
	static int hauteur;
	static Cell** reseau;

private:
	cv::Mat matrice;

};

#endif /* SRC_RESEAU_H_ */
