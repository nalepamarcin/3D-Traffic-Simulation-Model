#include "Lane.hpp"
#include "Car.hpp"
#include "World.hpp"
#include "Road.hpp"
#include <algorithm>


Lane::Lane(Road* road, int direction, int length):
	road(road), direction(direction), length(length), lanes({std::vector<Car*>(length), std::vector<Car*>(length)})
{}

Lane::~Lane() {
	//remove lane data
	for(unsigned int i=0; i<lanes[0].size(); ++i) {
		if(lanes[0][i])
			delete lanes[0][1];
	}

	//remove update lane data
	for(unsigned int i=0; i<lanes[1].size(); ++i) {
		if(lanes[1][i])
			delete lanes[1][1];
	}
}

void Lane::update() {
	for(auto &car : lanes[0])
		if(car)
			car->update();
}

void Lane::lockUpdate() {
	std::swap(lanes[0], lanes[1]);
}

void Lane::spawnCar(int length) {
	bool free = true;
	for(int i=0; i<World::maxLength; i++) {
		if(lanes[0][i] != nullptr) {
			if(i-lanes[0][i]->getLength() < 0) {
				free = false;
			}
		}
	}
	if(free)
		lanes[0][0] = new Car(this, length);
}

std::ostream & operator<<(std::ostream & ostr, const Lane & lane) {
	if(lane.direction == RIGHT) {
	for(int i=0; i<lane.length; i++)
		if(lane.lanes[0][i] == nullptr)
			ostr<<"[   ]";
		else
			ostr<<(*(lane.lanes[0][i]));
	}
	else {
		for(int i=lane.length-1; i>=0; i--)
				if(lane.lanes[0][i] == nullptr)
					ostr<<"[   ]";
				else
					ostr<<(*(lane.lanes[0][i]));
	}
	return ostr;
}

void Lane::log() {

	std::cout<<"====================================\n";
	if(direction == Direction::RIGHT) {
		std::cout<<"direction: ->\nData:\n";

		for(int i=0; i<length; i++)
			if(lanes[0][i])
				std::cout<<(*(lanes[0][i]));
			else
				std::cout<<"[   ]";


		std::cout<<"\nTemp:"<<std::endl;

		for(int i=0; i<length; i++)
			if(lanes[1][i])
				std::cout<<(*(lanes[1][i]));
			else
				std::cout<<"[   ]";
		std::cout<<std::endl;
	}
	else {
		std::cout<<"direction: <-\nData:\n";

				for(int i=length-1; i>=0; i--)
					if(lanes[0][i])
						std::cout<<(*(lanes[0][i]));
					else
						std::cout<<"[   ]";


				std::cout<<"\nTemp:"<<std::endl;

				for(int i=length-1; i>=0; i--)
					if(lanes[1][i])
						std::cout<<(*(lanes[1][i]));
					else
						std::cout<<"[   ]";
				std::cout<<std::endl;
	}

}

Car* Lane::getCar(int position) {
	return lanes[0][position];
}

void Lane::moveCar(int from, int to) {
	//std::cout<<"copying car for attributes: from="<<from<<" to="<<to<<std::endl;
	lanes[1][to] = lanes[0][from];
}

void Lane::removeCar(int position) {

	if(lanes[0][position])
		delete lanes[0][position];

	lanes[0][position] = nullptr;
}

void Lane::cleanUpdate() {
	for(int i=0; i<length; ++i)
		lanes[1][i] = nullptr;
}

int Lane::getLength() {
	return length;
}

//return next lane from road vector
//if next==false then returns previous lane
Lane* Lane::seekLane(bool next=true) {
	int size = road->getLanes(direction).size();
	if(next) {
		for(int i=0; i<size; i++) {
			if(this == road->getLanes(direction)[i]) {
				return (i<size-1)?(road->getLanes(direction))[i+1]:nullptr;
			}
		}
	} else {
		for(int i=size-1; i>=0; i--) {
			if(this == road->getLanes(direction)[i])
				return (i>0)?(road->getLanes(direction)[i-1]):nullptr;
		}
	}
	return nullptr;
}

void Lane::updateCarChangeLane(bool next) {

	for(auto& iter: lanes[0])
		if(iter!=nullptr)
			iter->changeLane(iter->doChangeLane(next));

}

void Lane::putCar(Car* car, int position) {
	lanes[0][position] = car;
}

bool Lane::isUsed(int position, int mylength) {
	for(int i=position; i<position+World::maxLength && i<length; i++) {
		if(lanes[0][i] && (position >= i-lanes[0][i]->getLength() + 1))
			return true;
	}
	for(int i=position; i>position-mylength && i>=0; i--) {
			if(lanes[0][i])
				return true;
		}
	return false;
}

bool Lane::getCrossInfo() {
	return toCrossroad;
}

void Lane::setCrossInfo(bool set) {
	toCrossroad = set;
}

TrafficLight* Lane::getTrafficLight() {
	return trafficLight;
}

void Lane::setTrafficLight(TrafficLight* trafficLight) {
	this->trafficLight = trafficLight;
}
