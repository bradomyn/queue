/*
 * Useful.cc
 *
 *  Created on: Feb 20, 2012
 *      Author: matthies
 */

#include "Useful.h"

namespace queueing {

queueing::Useful * queueing::Useful::_instance = 0;


Useful::Useful() {
	_instance = NULL;
}

Useful::~Useful() {
}

Useful *Useful::getInstance() {
	if( _instance==NULL ) {
		_instance = new Useful();
	}
	return _instance;
}

double Useful::getTime(std::string name) {
	size_t found;
	found = name.find("; ");
	//std::cout << "packetname " << packetname << " found " << found << std::endl;
	// extract time from packetname
	double triggerTime=0.;
	if( found!=std::string::npos ) {
		std::string time = name.substr(found+2);
		std::istringstream stm;
		stm.str(time);
		stm >> triggerTime;
		//std::cout << "time " << time << " " << triggerTime << std::endl;
	}
	return triggerTime;
}

int Useful::getPriority(std::string name) {
	size_t found1;
	found1 = name.find("y: ");
	//std::cout << "packetname " << packetname << " found1 " << found1 << std::endl;
	// extract priority from packetname
	int prio=0;
	if( found1!=std::string::npos ) {
		std::string priority = name.substr(found1+2);
		std::istringstream stm;
		stm.str(priority);
		stm >> prio;
		//std::cout << "priority " << prio << std::endl;
	}
	return prio;
} // getPriority()

int Useful::generateRandomPriority() {
	int random = 0;
#if 0
	srand(time(NULL));
	// TODO
	random = rand() % 7 + 0;  //number between 0 and 7
#else
	random = (int)(rand() / (((double)RAND_MAX + 1)/ (double)(7+1)));
#endif
	return random;
} // generateRandomPriority()

int Useful::generateRandomSize() {
	int random = 0;
#if 0
	srand(time(NULL));
	// TODO
	random = rand() % 7 + 0;  //number between 0 and 7
#else
	random = (int)(rand() / (((double)RAND_MAX + 1)/ (double)(1500+1)));
#endif
	return random;
} // generateRandomSize()

void Useful::writeRandomDataToList(std::string filename, int priority, int size) {
	FILE* filehandle = fopen( filename.c_str(),"a" );
	if( filehandle ) {
		fprintf(filehandle,"%d %d\n", priority, size);	// append to end of file
		fclose(filehandle );
	}
} // writeRandomDataToList()

std::vector<PacketDescription> Useful::readDataList(std::string filename) {

	std::vector<PacketDescription> v;
	int priority, size;
	FILE* filehandle = fopen( filename.c_str(),"r" );
	if( filehandle ) {
		while( fscanf(filehandle,"%d %d\n", &priority, &size)>0 ) {
			v.push_back(PacketDescription(priority, size));
		}
		fclose(filehandle );
		return v;
	} else {
		std::cerr << "File " << filename.c_str() << " does not exist." << std::endl;
		return v;
	}

} //readDataList()

void Useful::appendToFile(std::string filename, std::string value) {
	FILE* filehandle = fopen( filename.c_str(),"a" );
	if( filehandle ) {
		fprintf(filehandle,"%s\n", value.c_str());	// append to end of file
		fclose(filehandle );
	}
} // appendToFile()

void Useful::appendToFile(std::string filename, double value) {
	FILE* filehandle = fopen( filename.c_str(),"a" );
	if( filehandle ) {
		fprintf(filehandle,"%lf\n", value);	// append to end of file
		fclose(filehandle );
	}
} // appendToFile()

void Useful::appendToFile(std::string filename, int value) {
	FILE* filehandle = fopen( filename.c_str(),"a" );
	if( filehandle ) {
		fprintf(filehandle,"%d\n", value);	// append to end of file
		fclose(filehandle );
	}
} // appendToFile()
} /* namespace queueing */
