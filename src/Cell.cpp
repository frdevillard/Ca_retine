/*
 * Cell.cpp
 *
 *  Created on: 25 févr. 2015
 *      Author: pierre
 */

#include "Cell.h"
#include "Reseau.h"
#include <cstdlib>
#include <iostream>
#include <cmath>

Cell::Cell() :
		posX(0), posY(0), st(0), greyLevel(0.0), futurGreyLevel(0.0), k(0), kScale(
				0), lS(0), lSmoothing(0), iS(0), iSmoothing(2), a(0.01), m(0.0), M(
				0.0), pm(0.0), PM(0.0) {

}

Cell::~Cell() {
	//std::cout << "Cell destructor called" << std::endl;
}


void Cell::setPosX(int x) {
	this->posX = x;
}

int Cell::getPosY() const {
	return posY;
}

void Cell::setPosY(int y) {
	this->posY = y;
}

int Cell::getSt() const {
	return st;
}

void Cell::setSt(int st) {
	this->st = st;
}

float Cell::getFuturGreyLevel() const {
	return futurGreyLevel;
}

void Cell::setFuturGreyLevel(float futurGreyLevel) {
	this->futurGreyLevel = futurGreyLevel;
}

float Cell::getGreyLevel() const {
	return greyLevel;
}


void Cell::setGreyLevel(float greyLevel) {
	this->greyLevel = greyLevel;
}

void Cell::setVoisinage(Cell** voisinage) {
	for (int i = 0; i < 8; i++) {
		this->voisinage[i] = voisinage[i];
	}
}

void Cell::setKScale(int kScale) {
	this->kScale = kScale;
}

void Cell::setLSmoothing(int lSmoothing) {
	this->lSmoothing = lSmoothing;
}

void Cell::setISmoothing(int iSmoothing) {
	this->iSmoothing = iSmoothing;
}

void Cell::setA(float a) {
	this->a = a;
}

void Cell::initM() {
	this->m = greyLevel;
	this->M = greyLevel;
	this->pm = greyLevel;
	this->PM = greyLevel;
}

void Cell::complement() {
	this->greyLevel = 255 - this->greyLevel;
}

/*
 * Filtrage temporel en fonction du coefficient d'oubli a
 *
 */
void Cell::temporalFilter() {
	float A = std::abs(a);
	this->PM = this->M;
	this->pm = this->m;
	this->M = A * this->greyLevel + (1 - A) * fmax(this->greyLevel, this->PM);
	this->m = A * this->greyLevel + (1 - A) * fmin(this->greyLevel, this->pm);
	float moy = (this->M + this->m) / 2;
	if (a > 0)
		this->greyLevel = moy;
	else
		this->greyLevel = this->greyLevel - moy;
}

/*
 * Filtre binomial appliqué au voisinage courant
 *
 * Le voisinage est géré par le superviseur (Reseau)
 *
 */
void Cell::spatialFilter() {
	float outLvl = 0.0;
	outLvl += 4 * this->greyLevel;
	for (int i = 0; i < 8; i++) {
		//std::cout << "jusqu'ici\n";
		if (this->voisinage[i]->getGreyLevel() < 0
				|| this->voisinage[i]->getGreyLevel() > 256) {
			std::cout << "i : " << this->posX << " j : " << this->posY
					<< " voisinage : " << i
					<< " erreur de valeur de niveau de gris : "
					<< this->voisinage[i]->getGreyLevel() << std::endl;
			//std::cin.get();
			exit(-1);
		}
		if (i % 2 != 0)
			outLvl += (2 * this->voisinage[i]->getGreyLevel());
		else
			outLvl += this->voisinage[i]->getGreyLevel();
		//std::cout << outLvl << std::endl;
	}
	outLvl = outLvl / 16.0;
	this->futurGreyLevel = outLvl;
}

/*
 * Méthode de mise à jour des niveaux
 *
 */
void Cell::updateLevels() {
	this->greyLevel = this->futurGreyLevel;
	this->futurGreyLevel = 0.0;
}

/*
 * Demande au superviseur de fournir le nouveau voisinage
 */
void Cell::boundCells() {
	setVoisinage(Reseau::getVoisinage(posX, posY, k));
}

void Cell::resetNeighbourhood() {
	setVoisinage(Reseau::getVoisinage(posX, posY, 0));
}

void Cell::iterate() {
	switch (st) {
	case STATE_0:					//état 0 initialisation
		this->lS = 0;
		this->iS = 0;
		this->k = 0;
		if (kScale > 0)
			this->st = STATE_2a;
		else if (lSmoothing > 0)
			this->st = STATE_1a;
		else if (a >= -1 && a != 0 && a < 1 && kScale == 0 && lSmoothing == 0)	//transition c0c4
			this->st = STATE_4;
		else if (a == 1 && kScale == 0 && lSmoothing == 0)
			this->st = STATE_5;
		else
			this->st = -1;
		break;
	case STATE_2a:					//état 2a calcul des niveaux du filtre antialiasing
		Cell::spatialFilter();
		this->iS++;
		this->st = STATE_2b;
		break;
	case STATE_2b:					//état 2b application des niveaux calculés
		Cell::updateLevels();
		if (iS < iSmoothing)
			this->st = STATE_2a;
		else {
			if (lSmoothing == 0 && k == kScale - 1) {
				this->st = STATE_3;
			} else {
				this->st = STATE_2c;
			}
		}
		break;
	case STATE_2c:					//état 2c remaillage (calcul des nouveaux voisins)
		this->iS = 0;
		this->k++;
		Cell::boundCells();
		if (k < kScale) {
			this->st = STATE_2a;
		} else {
			this->st = STATE_1a;
		}
		break;
	case STATE_1a:					//état 1a calcul des nouveaux niveaux
		Cell::spatialFilter();
		this->lS++;
		this->st = STATE_1b;
		break;
	case STATE_1b:					//état 1b application des niveaux calculés
		Cell::updateLevels();
		if (lS < lSmoothing)
			this->st = STATE_1a;
		else if (kScale == 0)	//transition c1bc4
			this->st = STATE_4;
		else
			this->st = STATE_3;
		break;
	case STATE_3:					//état 3 RAZ du voisinage
		Cell::resetNeighbourhood();
		this->st = STATE_4;
		break;
	case STATE_4:					//état 4 filtrage temporel
		Cell::temporalFilter();
		this->st = STATE_5;
		break;
	case STATE_5:					//état 5 stop
		break;
	default:
		/*
		 * TODO message d'erreur
		 */
		std::cout << "Mauvais paramètres entrés" << std::endl;
		exit(-1);
	}
}

