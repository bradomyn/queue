//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#ifndef __QUEUEING_PACKET_H
#define __QUEUEING_PACKET_H

#include <vector>
#include "Packet_m.h"

namespace queueing {

class PacketList;

/**
 * We extend the generated Packet_Base class with support for split-join, as well
 * as the ability to enumerate all Packets in the system.
 *
 * To support split-join, packets manage parent-child relationships. A
 * relationship is created with the makeChildOf() or addChild() methods,
 * and lives until the parent or the child packet is destroyed.
 * It can be queried with the getParent() and getNumChildren()/getChild(k)
 * methods.
 *
 * To support enumerating all packets in the system, each packet automatically
 * registers itself in a PacketList module, if one exist in the model.
 * (If there's no PacketList module, no registration takes place.) If there
 * are more than one PacketList modules, the first one is chosen.
 * PacketList can also be explicitly specified in the packet constructor.
 * The default PacketList can be obtained with the PacketList::getDefaultInstance()
 * method. Then one can query PacketList for the set of packets currently present.
 */
class QUEUEING_API Packet: public Packet_Base
{
    friend class PacketList;
    protected:
        Packet *parent;
        std::vector<Packet*> children;
        PacketList *packetList;
        virtual void setParent(Packet *parent); // only for addChild()
        virtual void parentDeleted();
        virtual void childDeleted(Packet *child);
    public:
        /**
         * Creates a Packet with the given name, message kind, and PacketList. If
         * PacketList==NULL, the default one (or none if none exist) will be chosen.
         */
        Packet(const char *name=NULL, int kind=0, PacketList *table=NULL);

        /** Copy constructor */
        Packet(const Packet& packet);

        /** Destructor */
        virtual ~Packet();

        /** Duplicates this packet */
        virtual Packet *dup() const {return new Packet(*this);}

        /** Assignment operator. Does not affect parent, children and PacketList. */
        Packet& operator=(const Packet& packet);

        /** @name Parent-child relationships */
        //@{
        /** Returns the parent Packet. Returns NULL if there's no parent or it no longer exists. */
        virtual Packet *getParent();

        /** Returns the number of children. Deleted children are automatically removed from this list. */
        virtual int getNumChildren() const;

        /** Returns the kth child. Throws an error if index is out of range. */
        virtual Packet *getChild(int k);

        /** Marks the given packet as the child of this one. */
        void addChild(Packet *child);

        /** Same as addChild(), but has to be invoked on the child packet */
        virtual void makeChildOf(Packet *parent);
        //@}

        /** Returns the PacketList where this Packet has been registered. */
        PacketList *getContainingPacketList() {return packetList;}

};

}; // namespace

#endif