//
//
// Copyright (C) 2011 Juan Luis Garrote Molinero
// Copyright (C) 2013 Zsolt Prontvai
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#ifndef __INET_RSTP_H
#define __INET_RSTP_H

#include "ILifecycle.h"
#include "IEEE8021DBPDU_m.h"
#include "MACAddress.h"
#include "EtherFrame.h"
#include "MACAddressTable.h"
#include "InterfaceTable.h"
#include "Ieee8021DInterfaceData.h"
#include "STPBase.h"

/**
 * Implements the Rapid Spanning Tree Protocol. See the NED file for details.
 */
class RSTP : public STPBase {
protected:
    // kind codes for self messages
    enum SelfKinds {SELF_HELLOTIME = 1, SELF_UPGRADE, SELF_TIMETODESIGNATE};

    // Set by management: see the ned file for more info
    simtime_t migrateTime;
    simtime_t tcWhileTime;
    bool autoEdge;

    cMessage* helloTimer;
    cMessage* forwardTimer;
    cMessage* migrateTimer;

public:
    RSTP();
    virtual ~RSTP();
    virtual int numInitStages() const { return 2; }

protected:
    virtual void initialize(int stage);
    virtual void finish() {}
    virtual void initInterfacedata(unsigned int portNum);

    virtual void start();
    virtual void stop();

    /**
     * @brief initialize RSTP dynamic information
     */
    virtual void initPorts();

    /**
     * @brief Gets the best alternate port
     * @return Best alternate gate index
     */
    virtual int getBestAlternate();

    /**
     * @brief Sends BPDUs through all ports, if they are required
     */
    virtual void sendBPDUs();

    /**
     * @brief Sends BPDU through a port
     */
    virtual void sendBPDU(int port);

    /**
     * @brief General processing
     */
    virtual void handleMessage(cMessage *msg);

    /**
     * @brief BPDU processing.
     * Updates port information. Handles port role changes.
     */
    virtual void handleIncomingFrame(BPDU *frame);

    /**
     * @brief Prints current data base info
     */
    virtual void printState();

    virtual void updateInterfacedata(BPDU *frame, unsigned int portNum);

    /**
     * @brief Compares the BPDU frame with the BPDU this module would send through that port
     * @return (<0 if the root BPDU is better than BPDU)
     * -4=worse port  -3=worse src  -2=worse RPC  -1=worse root  0=Similar  1=better root  2=better RPC  3=better src  4=better port
     */
    virtual int contestInterfacedata(BPDU* msg, unsigned int portNum);

    /**
     * @brief Compares the port's best BPDU with the BPDU this module would send through that port
     * @return (<0 if the root BPDU is better than port's best BPDU)
     * -4=worse port  -3=worse src  -2=worse RPC  -1=worse root  0=Similar  1=better root  2=better RPC  3=better src  4=better port
     */
    virtual int contestInterfacedata(unsigned int portNum);

    /**
     * @brief Compares a port's best BPDU with a BPDU frame
     * @return (<0 if vector better than frame)
     * -4=worse port  -3=worse src  -2=worse RPC  -1=worse root  0=Similar  1=better root  2=better RPC  3=better src  4=better port
     */
    virtual int compareInterfacedata(unsigned int portNum, BPDU * msg, int linkCost);

    /**
     * @brief If root TCWhile has not expired, sends a BPDU to the Root with TCFlag=true.
     */
    virtual void sendTCNtoRoot();

    /**
     * @brief HelloTime event handling.
     */
    virtual void handleHelloTime(cMessage *);

    /**
     * @brief Upgrade event handling. (Every forwardDelay)
     */
    virtual void handleUpgrade(cMessage *);

    /**
     * @brief Migration to designated. (Every migrateTime)
     */
    virtual void handleMigrate(cMessage *);

    /**
     * @brief Checks the frame TC flag.
     * Sets TCWhile if the port was forwarding and the flag is true.
     */
    virtual void checkTC(BPDU * frame, int arrival);

    /**
     * @brief Handles the switch to backup in one of the ports
     */
    virtual void handleBK(BPDU * frame, unsigned int arrival);
};

#endif
