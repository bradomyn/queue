/*
 * Useful.h
 *
 *  Created on: Feb 20, 2012
 *      Author: matthies
 */

#ifndef USEFUL_H_
#define USEFUL_H_

#include <sstream>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace queueing {

#ifdef WIN32
#define log(src,...) td::cout << __FUNCTION__ << "() at " << __FILE__ << ":" << __LINE__ << ": " << src << __VA_ARGS__ << std::endl;
#elif __linux__
//#define log(src,args...) std::cout << __FUNCTION__ << "() at " << __FILE__ << ":" << __LINE__ << ": " << src << ##args << std::endl;
//OS::slog(LOG_LOCAL0 | sev, "{%d} in %s() at %s:%d: " fmt , syscall(SYS_gettid), __FUNCTION__, __FILE__, __LINE__, ##args)
#endif

#define log(src, args...) std::cout << __FUNCTION__ << "() at " << __FILE__ << ":" << __LINE__ << ": " << src << std::endl;
//##args <<

// simple helper class
class JobDescription {
public:
	JobDescription(int priority, int size) {
		_priority = priority;
		_size = size;
	};

	int getPriority(){ return _priority; };
	int getSize(){ return _size; };

private:
	int _priority;
	int _size;
};

class Useful {

public:
	Useful();
	virtual ~Useful();

	static Useful *getInstance();

	double getTime(string name);
	int getPriority(string name);

	int generateRandomPriority();
	int generateRandomSize();

	std::vector<JobDescription> readDataList(std::string filename);

	void writeRandomDataToList(std::string filename, int priority, int size);

	void appendToFile(std::string filename, std::string value);
	void appendToFile(std::string filename, double value);
	void appendToFile(std::string filename, int value);

private:
	static Useful *_instance;

};

} /* namespace queueing */
#endif /* USEFUL_H_ */
