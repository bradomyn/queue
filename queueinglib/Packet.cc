//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include <algorithm>
#include "Packet.h"
#include "PacketList.h"

namespace queueing {

Packet::Packet(const char *name, int kind, PacketList *packetList) : Packet_Base(name, kind)
{
    parent = NULL;
    if (packetList==NULL && PacketList::getDefaultInstance()!=NULL)
        packetList = PacketList::getDefaultInstance();
    this->packetList = packetList;
    if (packetList!=NULL)
        packetList->registerPacket(this);
}

Packet::Packet(const Packet& packet)
{
    setName(packet.getName());
    operator=(packet);
    parent = NULL;
    packetList = packet.packetList;
    if (packetList!=NULL)
        packetList->registerPacket(this);
}

Packet::~Packet()
{
    if (parent)
        parent->childDeleted(this);
    for (int i=0; i<(int)children.size(); i++)
        children[i]->parentDeleted();
    if (packetList!=NULL)
        packetList->deregisterPacket(this);
}

Packet& Packet::operator=(const Packet& packet)
{
    if (this==&packet) return *this;
    Packet_Base::operator=(packet);
    // leave parent and packetList untouched
    return *this;
}

Packet *Packet::getParent()
{
    return parent;
}

void Packet::setParent(Packet *parent)
{
    this->parent = parent;
}

int Packet::getNumChildren() const
{
    return children.size();
}

Packet *Packet::getChild(int k)
{
    if (k<0 || k>=(int)children.size())
        throw cRuntimeError(this, "child index %d out of bounds", k);
    return children[k];
}

void Packet::makeChildOf(Packet *parent)
{
    parent->addChild(this);
}

void Packet::addChild(Packet *child)
{
    child->setParent(this);
    ASSERT(std::find(children.begin(), children.end(), child)==children.end());
    children.push_back(child);
}

void Packet::parentDeleted()
{
    parent = NULL;
}

void Packet::childDeleted(Packet *child)
{
    std::vector<Packet*>::iterator it = std::find(children.begin(), children.end(), child);
    ASSERT(it!=children.end());
    children.erase(it);
}

}; // namespace
