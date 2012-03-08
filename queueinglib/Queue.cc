//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include "Queue.h"
#include "Packet.h"

namespace queueing {

Define_Module(Queue);

Queue::Queue()
{
    packetServiced = NULL;
    endServiceMsg = NULL;
}

Queue::~Queue()
{
    delete packetServiced;
    cancelAndDelete(endServiceMsg);
}

void Queue::initialize()
{
    droppedSignal = registerSignal("dropped");
    queueingTimeSignal = registerSignal("queueingTime");
    queueLengthSignal = registerSignal("queueLength");
    emit(queueLengthSignal, 0);
    busySignal = registerSignal("busy");
    emit(busySignal, 0);

    endServiceMsg = new cMessage("end-service");
    fifo = par("fifo");
    capacity = par("capacity");
    queue.setName("queue");
}

void Queue::handleMessage(cMessage *msg)
{
    if (msg==endServiceMsg)
    {
        endService( packetServiced );
        if (queue.empty())
        {
            packetServiced = NULL;
            emit(busySignal, 0);
        }
        else
        {
            packetServiced = getFromQueue();
            emit(queueLengthSignal, length());
            simtime_t serviceTime = startService( packetServiced );
            scheduleAt( simTime()+serviceTime, endServiceMsg );
        }
    }
    else
    {
        Packet *packet = check_and_cast<Packet *>(msg);
        arrival(packet);

        if (!packetServiced)
        {
        // processor was idle
            packetServiced = packet;
            emit(busySignal, 1);
            simtime_t serviceTime = startService( packetServiced );
            scheduleAt( simTime()+serviceTime, endServiceMsg );
        }
        else
        {
            // check for container capacity
            if (capacity >=0 && queue.length() >= capacity)
            {
                EV << "Capacity full! Packet dropped.\n";
                if (ev.isGUI()) bubble("Dropped!");
                emit(droppedSignal, 1);
                delete packet;
                return;
            }
            queue.insert( packet );
            emit(queueLengthSignal, length());
            packet->setQueueCount(packet->getQueueCount() + 1);
        }
    }

    if (ev.isGUI()) getDisplayString().setTagArg("i",1, !packetServiced ? "" : "cyan3");
}

Packet *Queue::getFromQueue()
{
    Packet *packet;
    if (fifo)
    {
        packet = (Packet *)queue.pop();
    }
    else
    {
        packet = (Packet *)queue.back();
        // FIXME this may have bad performance as remove uses linear search
        queue.remove(packet);
    }
    return packet;
}

int Queue::length()
{
    return queue.length();
}

void Queue::arrival(Packet *packet)
{
    packet->setTimestamp();
}

simtime_t Queue::startService(Packet *packet)
{
    // gather queueing time statistics
    simtime_t d = simTime() - packet->getTimestamp();
    emit(queueingTimeSignal, d);
    packet->setTotalQueueingTime(packet->getTotalQueueingTime() + d);
    EV << "Starting service of " << packet->getName() << endl;
    packet->setTimestamp();
    return par("serviceTime").doubleValue();
}

void Queue::endService(Packet *packet)
{
    EV << "Finishing service of " << packet->getName() << endl;
    simtime_t d = simTime() - packet->getTimestamp();
    packet->setTotalServiceTime(packet->getTotalServiceTime() + d);
    send(packet, "out");
}

void Queue::finish()
{
}

}; //namespace

