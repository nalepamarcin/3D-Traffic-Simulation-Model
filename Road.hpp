/*
 * Road.h
 *
 *  Created on: 17.11.2016
 *      Author: kamil
 */

#ifndef ROAD_HPP_
#define ROAD_HPP_

#include <ostream>
#include <vector>
#include <map>
#include <set>

class Lane;

class Road {
friend std::ostream & operator<<(std::ostream & stream, const Road & road);
friend class MapParser;
private:
	int length;
	int left;
	int right;
	std::vector<Lane*> lanesRight;
	std::vector<Lane*> lanesLeft;
	std::map<Lane*, std::set<Lane*>* > forbiddenLaneChanges;
public:
	Road(int length, int left, int right);
	~Road();
	void forbidLaneChange(Lane* lane1, Lane* lane2);
	std::vector<Lane *> getLanes(int direction);
	void update();
	void lockUpdate();
	void changeLanes();
	int getLength();
	int getLanesQuantity(int direction);
	void cleanUpdate();
	void allowUpdate();
	bool isForbiddenToChangeLane(Lane* lane1, Lane* lane2);

};

#endif /* ROAD_HPP_ */
