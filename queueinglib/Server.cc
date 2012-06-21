//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include "Server.h"
#include "Packet.h"
#include "IPassiveQueue.h"

namespace queueing {

Define_Module(Server);

Server::Server()
{
    jobServiced = NULL;
    triggerServiceMsg = NULL;
}

Server::~Server()
{
    delete jobServiced;
    cancelAndDelete(triggerServiceMsg);
}

void Server::initialize()
{
    busySignal = registerSignal("busy");
    emit(busySignal, 0);

    jobServiced = NULL;

    triggerServiceMsg = new cMessage("triggerServiceMessage");

    // keep a pointer to queue 7
    _q7 = getQueue(7);

    _nofCoS = par("nofCoS");

    // pointers to other queues
    for( int i=_nofCoS-1; i>-1; i-- )
    	_qs.push_back( getQueue(i) );

    WATCH(numSent);
	numSent = 0;

	_weight7 = par("weight7");
	_rrN = par("rrN");


	_schedulingAlgorithm = par("schedulingAlgorithm");
	std::cout << this->getName() << " scheduling algorithm " << _schedulingAlgorithm << std::endl;

	if (strcmp(_schedulingAlgorithm, "priority") == 0) {
		_scheduling = 0;
		cout << "server priority" << endl;
	} else if (strcmp(_schedulingAlgorithm, "7first") == 0) {
		_scheduling = 1;
		cout << "server mixed1" << endl;
	} else if (strcmp(_schedulingAlgorithm, "feedback") == 0) {
		_scheduling = 2;
		cout << "server feedback" << endl;
	} else if (strcmp(_schedulingAlgorithm, "wfq1") == 0) {
		_scheduling = 3;
		cout << "server wfq1" << endl;
	} else if (strcmp(_schedulingAlgorithm, "mixed1") == 0) {
		_scheduling = 4;
		cout << "server mixed1" << endl;
	} else if (strcmp(_schedulingAlgorithm, "wfq2") == 0) {
		_scheduling = 5;
		cout << "server wfq2" << endl;
	} else if (strcmp(_schedulingAlgorithm, "wfq3") == 0) {
		_scheduling = 6;
		cout << "server wfq3" << endl;
	} else if (strcmp(_schedulingAlgorithm, "wfq4") == 0) {
		_scheduling = 7;
		cout << "server wfq4" << endl;
	} else if (strcmp(_schedulingAlgorithm, "fcfs") == 0) {
		_scheduling = 8;
		cout << "server fcfs" << endl;
	} else if (strcmp(_schedulingAlgorithm, "original") == 0) {
		_scheduling = 9;
		cout << "server original" << endl;
	}


}

void Server::handleMessage(cMessage *msg)
{
	//cout << __FILE__ << ": " << __FUNCTION__ << " " << msg->getName() << endl;
	switch(_scheduling ) {
	case 0:
		priority(msg);
		break;
	case 1:
		sevenfirst(msg);
		break;
	case 2:
		feedback(msg);
		break;
	case 3:
		wfq1(msg);
		break;
	case 4:
		mixed1(msg);
		break;
	case 5:
		wfq2(msg);
		break;
	case 6:
		wfq3(msg);
		break;
	case 7:
		wfq4(msg);
		break;
	case 8:
		fcfs(msg);
		break;
	case 9:
		original(msg);
		break;
	default:
		break;
	}

} //handleMessage()

void Server::original(cMessage* msg) {
	if ( strcmp(msg->getName(), "trigger") == 0 ) {
		// request packet from queue and send to sink

		// priority
		if( _q7 != NULL ) {
			if( _q7->length()>0 ) {
				_q7->request(0);
			}
		}
		vector<IPassiveQueue* >::iterator it;
		for( it=_qs.begin();it!=_qs.end(); it++ ) {
			if( (*it)->length()>0 ) {
				(*it)->request(0);
			}
		}
	} else {
		Packet *packet = check_and_cast<Packet *>(msg);
		send(packet, "out");
	}
} // original()

void Server::priority(cMessage* msg) {
	if ( strcmp(msg->getName(), "trigger") == 0 ) {
		// request packet from queue and send to sink

		//priority
		if( _q7 != NULL ) {
			//if( _q7->length()>0 ) {
			while( _q7->length()>0 ) {
				_q7->request(0);
			}
		}
		vector<IPassiveQueue* >::iterator it;
		for( it=_qs.begin();it!=_qs.end(); it++ ) {
			if( _q7 != NULL ) {
				if( _q7->length()>0 ) {	// check queue7 again
					//if( _q7->length()>0 )
					while( _q7->length()>0 )
						_q7->request(0);
				}
			}
			//if( (*it)->length()>0 ) {
			while( (*it)->length()>0 ) {
				(*it)->request(0);
			}
		}
	} else {
		Packet *packet = check_and_cast<Packet *>(msg);
		send(packet, "out");
	}
} // priority()

void Server::sevenfirst(cMessage* msg) {
	if ( strcmp(msg->getName(), "trigger") == 0 ) {
		// request packet from queue and send to sink

		//priority
		vector<IPassiveQueue* >::iterator it;
		for( it=_qs.begin();it!=_qs.end(); it++ ) {
			//if( (*it)->length()>0 ) {
			while( (*it)->length()>0 ) {
				(*it)->request(0);
			}
		}
	} else {
		Packet *packet = check_and_cast<Packet *>(msg);
		send(packet, "out");
	}
} // sevenfirst()

void Server::fcfs(cMessage* msg) {
	if ( strcmp(msg->getName(), "trigger") == 0 ) {
		// request packet from queue and send to sink
		// check order list
		while(_fcfsQueueServeList.size()>0 ) {
			int nr = getFromQueueServeList();
			std::string queue;
			char buffer[3];
			queue = "queue";
			sprintf(buffer, "%d", nr);
			buffer[2] = '\0';
			queue += buffer;
			cModule *thisqueue = (cModule*)getParentModule()->findObject(queue.c_str(), true);
			(check_and_cast<PassiveQueue *>(thisqueue))->request(0);
			_fcfsQueueServeList.erase(_fcfsQueueServeList.begin());
		}

	} else {
		Packet *packet = check_and_cast<Packet *>(msg);
		send(packet, "out");
	}
} // fcfs()

void Server::feedback(cMessage* msg) {
#if 0
	// parse queues for oldest packets
	if ( strcmp(msg->getName(), "trigger") == 0 ) {
		// request packet from queue and send to sink
		//cout << "trigger " << _q7->length() << endl;

		//priority
		if( _q7 != NULL ) {
			while( _q7->length()>0 ) {
				_q7->request(0);
			}
		}

		vector<IPassiveQueue* > order;

		// request oldest packet first
		vector<IPassiveQueue* >::iterator it;
		for( it=_qs.begin(); it!=_qs.end(); it++) {
			if( (*it)->length()>0 ) {
				for( int i=0; i<(*it)->length(); i++ ) {
					Packet* p = (*it)->front();
					if( p!=NULL ) {
						cout << "queue l " << (*it)->length() << " " << p->getName() << " map size " << _map.size() << endl;
						_map.insert(pair<simtime_t,IPassiveQueue*>(p->getCreationTime(),*it));
						order.push_back(*it);
					}
				}
			}
		}

		if( _map.size()>0 ) {
			std::map<simtime_t, IPassiveQueue*>::iterator mit;
			mit=_map.end();
			if( mit==_map.end() ) {
				mit--;
				for( ; mit!=_map.begin(); mit-- ) {
					while( (*mit).second->length() > 0 ) {
						(*mit).second->request(0);
					}
				}
			}
		}
	} else {
		Packet *packet = check_and_cast<Packet *>(msg);
		send(packet, "out");
	}

#else
	if ( strcmp(msg->getName(), "trigger") == 0 ) {
		// request packet from queue and send to sink
		//cout << "trigger " << _q7->length() << endl;

		//priority
		if( _q7 != NULL ) {
			while( _q7->length()>0 ) {
				_q7->request(0);
			}
		}
		_mapFeedback.clear();
		// request oldest packet first (supposedly in longest queue)
		vector<IPassiveQueue* >::iterator it;
		int i=0;
		for( it=_qs.begin(); it!=_qs.end(); it++, i++ ) {
			//cout  << "i: " << i << " " << (*it)->length() << endl;
			_mapFeedback.insert(pair<int, IPassiveQueue*>( (*it)->length(), (*it) ));
		}

		map<int, IPassiveQueue*>::iterator mit;
		mit=_mapFeedback.end();
		if( mit == _mapFeedback.end() ) {
			mit--;
			for( ; mit!=_mapFeedback.begin(); mit-- ) {
				while( (*mit).second->length() > 0 ) {
					(*mit).second->request(0);
				}
			}
		}
	} else {
		Packet *packet = check_and_cast<Packet *>(msg);
		send(packet, "out");
	}
#endif
} // feedback()

void Server::wfq1(cMessage* msg) {
	if ( strcmp(msg->getName(), "trigger") == 0 ) {
		// request packet from queue and send to sink
		//cout << "trigger " << _q7->length() << endl;
		int N=_weight7;
		int i=0;
		if( _q7 != NULL ) {
			if( _q7->length()>0 ) {
				for( i=0; i<N; i++ ) {
					if( _q7->length()>0 ) {
						_q7->request(0);
					}
				}
			}

			if( i==N || _q7->length()==0 ) {
				vector<IPassiveQueue* >::iterator it;
				for( it=_qs.begin();it!=_qs.end(); it++ ) {
					if( (*it)->length()>0 ) {
						(*it)->request(0);
					}
				}
			}
		}
	} else {
		Packet *packet = check_and_cast<Packet *>(msg);
		send(packet, "out");
	}
} // wfq1()

// use round robin
// works best with
// _rrCounter = 7; _rrN = 5;
void Server::wfq2(cMessage* msg) {
	if ( strcmp(msg->getName(), "trigger") == 0 ) {
		// request packet from queue and send to sink
		//cout << "trigger " << _q7->length() << endl;
		int i=0;
		vector<IPassiveQueue* >::iterator it;

		if( _q7 != NULL ) {
			if( _q7->length()>0 ) {
				for( i=0; i<_rrN; i++ ) {
					if( _q7->length()>0 ) {
						_q7->request(0);
					}
				}
			}
		}
		switch (_rrCounter) {
		case 6:
			for (i = 0; i < _rrN - 1; i++) {
				if (_qs.at(0)->length() > 0) {
					_qs.at(0)->request(0);
				}
			}
			break;
		case 5:
			for (i = 0; i < _rrN - 2; i++) {
				if (_qs.at(1)->length() > 0) {
					_qs.at(1)->request(0);
				}
			}
			break;
		case 4:
			for (i = 0; i < _rrN - 3; i++) {
				if (_qs.at(2)->length() > 0) {
					_qs.at(2)->request(0);
				}
			}
			break;
		case 3:
			for (i = 0; i < _rrN - 4; i++) {
				if (_qs.at(3)->length() > 0) {
					_qs.at(3)->request(0);
				}
			}
			break;
		case 2:
			for (i = 0; i < _rrN - 5; i++) {
				if (_qs.at(4)->length() > 0) {
					_qs.at(4)->request(0);
				}
			}
			break;
		case 1:
			for (i = 0; i < _rrN - 5; i++) {
				if (_qs.at(5)->length() > 0) {
					_qs.at(5)->request(0);
				}
			}
			break;
		case 0:
			for (i = 0; i < _rrN - 5; i++) {
				if (_qs.at(6)->length() > 0) {
					_qs.at(6)->request(0);
				}
			}
			break;
		}

		// reset counter
		_rrCounter = _rrCounter - 1;
		if (_rrCounter < 0)
			_rrCounter = 6;


		//cout << " rrCounter = " << _rrCounter << endl;
	} else {
		Packet *packet = check_and_cast<Packet *>(msg);
		send(packet, "out");
	}
} // wfq2()


void Server::wfq3(cMessage* msg) {
	if ( strcmp(msg->getName(), "trigger") == 0 ) {
		// request packet from queue and send to sink
		//cout << "trigger " << _q7->length() << endl;
		int N=_weight7;
		int i=0;

		if( _q7 != NULL ) {
			if( _q7->length()>0 ) {
				for( i=0; i<N; i++ ) {
					if( _q7->length()>0 ) {
						_q7->request(0);
					}
				}
			}
		}
		_mapFeedback.clear();
		for( int i=0; i<_nofCoS; i++ ) {
			_mapFeedback.insert(pair<int, IPassiveQueue*>( _qs.at(i)->length(), _qs.at(i) ));
		}
		map<int, IPassiveQueue*>::iterator mit;
		mit=_mapFeedback.end();
		if( mit == _mapFeedback.end() ) {
			mit--;
			// longest queue first
			for( ; mit!=_mapFeedback.begin(); mit-- ) {
				while( (*mit).second->length() > 0 ) {
					(*mit).second->request(0);
				}
			}
		}
	} else {
		Packet *packet = check_and_cast<Packet *>(msg);
		send(packet, "out");
	}
} // wfq3()

void Server::wfq4(cMessage* msg) {
	if ( strcmp(msg->getName(), "trigger") == 0 ) {
		// request packet from queue and send to sink
		//cout << "trigger " << _q7->length() << endl;
		int N=_weight7;
		int i=0;

		if( _q7 != NULL ) {
			if( _q7->length()>0 ) {
				for( i=0; i<N; i++ ) {
					if( _q7->length()>0 ) {
					//while( _q7->length()>0 ) {
						_q7->request(0);
					}
				}
			}
		}
		// consider packet sizes instead of queue lengths
		_mapFeedback.clear();
		for( int i=0; i<_nofCoS; i++ ) {
			_mapFeedback.insert(pair<int, IPassiveQueue*>( _qs.at(i)->size(), _qs.at(i) ));
		}
		map<int, IPassiveQueue*>::iterator mit;
		mit=_mapFeedback.end();
		if( mit == _mapFeedback.end() ) {
			mit--;
			int j=0;
			// biggest queue first, apply weights appropriately
			for( ; mit!=_mapFeedback.begin(); mit-- ) {
				if( (*mit).second->length() > 0 ) {
				//while( (*mit).second->length() > 0 ) {
					for( int i=0; i<N-j; i++ ) {
						if( (*mit).second->length() > 0 ) {
							(*mit).second->request(0);
						} else {
							break;
						}
					}
				}
				j+=1;
			}
		}
	} else {
		Packet *packet = check_and_cast<Packet *>(msg);
		send(packet, "out");
	}
} // wfq4()

void Server::mixed1(cMessage* msg) {
	if ( strcmp(msg->getName(), "trigger") == 0 ) {
		// request packet from queue and send to sink
		//cout << "trigger " << _q7->length() << endl;

		// 7, FIFO
		// 6-4,
		// request oldest packet first (supposedly in longest queue)
		// _qs is sorted from 6 to 0

		vector<IPassiveQueue* >::iterator it;
		for( int i=0; i<=3; i++ ) {
			//cout  << "i: " << i << " " << _qs.at(i)->length() << endl;
			while( _qs.at(i)->length()>0 ) {
				_qs.at(i)->request(0);
			}
		}

		// 3-0,
		_mapFeedback.clear();
		for( int i=4; i<=6; i++ ) {
			//cout  << "i: " << i << " " << _qs.at(i)->length() << endl;
			_mapFeedback.insert(pair<int, IPassiveQueue*>( _qs.at(i)->length(), _qs.at(i) ));
		}
		map<int, IPassiveQueue*>::iterator mit;
		mit=_mapFeedback.end();
		mit--;
		for( ; mit!=_mapFeedback.begin(); mit-- ) {
			while( (*mit).second->length() > 0 ) {
				(*mit).second->request(0);
			}
		}
	} else {
		Packet *packet = check_and_cast<Packet *>(msg);
		send(packet, "out");
	}
} // mixed1()

IPassiveQueue *Server::getQueue(int index) {
	std::string queue = "queue";
	char buffer[3];

	sprintf(buffer,"%d",index);
	buffer[2]='\0';
	queue += buffer;
	cModule *module = getParentModule()->getSubmodule(queue.c_str());
	IPassiveQueue *pqueue = dynamic_cast<IPassiveQueue *>(module);
	return pqueue;
} // getQueue()

void Server::finish()
{
	// remove all undisposed messages in the end!
	this->setPerformFinalGC(true);
}

bool Server::isIdle()
{
    return jobServiced == NULL;
}

}; //namespace

