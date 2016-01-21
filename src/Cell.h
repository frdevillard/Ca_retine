/*
 * Cell.h
 *
 *  Created on: 25 févr. 2015
 *      Author: pierre
 */

#ifndef SRC_CELL_H_
#define SRC_CELL_H_

class Reseau;

class Cell {
	friend class Reseau;
public:
	Cell();
	//Cell();
	virtual ~Cell();
	float getIn() const;
	void setIn(int in);
	int getOut() const;
	int getPosX() const;
	void setPosX(int x);
	int getPosY() const;
	void setPosY(int y);
	int getSt() const;
	void setSt(int st);
	float getFuturGreyLevel() const;
	void setFuturGreyLevel(float futurGreyLevel);
	float getGreyLevel() const;
	void setGreyLevel(float greyLevel);
	void setVoisinage(Cell** voisinage);
	void setKScale(int kScale);
	void setLSmoothing(int lSmoothing);
	void setISmoothing(int iSmoothing);
	void setA(float a);
	void initM();
	void complement();
	void temporalFilter();
	void spatialFilter();
	void updateLevels();
	void boundCells();
	void resetNeighbourhood();
	void iterate();

	//std::array<Cell, 8>  voisinage;	//voisins
	Cell* voisinage[8];
	/*
	 * 0  1  2
	 * 7  X  3
	 * 6  5  4
	 *
	 */
#define NORTH_WEST	0
#define NORTH 		1
#define NORTH_EAST	2
#define EAST		3
#define SOUTH_EAST	4
#define SOUTH		5
#define SOUTH_WEST	6
#define WEST		7

private:
	int posX;	//localisation dans le réseau
	int posY;
	int st;		// état présent
	float greyLevel;
	float futurGreyLevel;
	int k;
	int kScale;
	int lS;
	int lSmoothing;
	int iS;		// pour ne pas interférer avec le i des boucles...
	int iSmoothing;
	float a;
	float m;
	float M;
	float pm;
	float PM;
};

#define STATE_0		0
#define STATE_1a	1
#define STATE_1b	2
#define STATE_2a	3
#define STATE_2b	4
#define STATE_2c	5
#define STATE_3		6
#define STATE_4		7
#define STATE_5		8

#endif /* SRC_CELL_H_ */
