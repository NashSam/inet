//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 
// Author: Zsolt Prontvai
//
#ifndef STPUTIL_H_
#define STPUTIL_H_

#include "ILifecycle.h"
#include "MACAddress.h"
#include "MACAddressTable.h"
#include "InterfaceTable.h"
#include "Ieee8021DInterfaceData.h"

class STPBase : public cSimpleModule, public ILifecycle {
protected:
    bool visualize;         // colors tree
    unsigned int numPorts;    // number of ports
    bool isOperational;        // for lifecycle

    unsigned int bridgePriority;              // bridge's priority
    MACAddress bridgeAddress;        // bridge's MAC address

    simtime_t maxAge;
    simtime_t helloTime;
    simtime_t forwardDelay;

    MACAddressTable * macTable;      // needed for flushing.
    IInterfaceTable * ifTable;

public:
    virtual bool handleOperationStage(LifecycleOperation *operation, int stage, IDoneCallback *doneCallback);

protected:
    virtual int numInitStages() const {return 2;}
    virtual void initialize(int stage);

    virtual void start();
    virtual void stop();

    /**
     * @brief Adds effects to be represented by Tkenv. Root links colored green. Show port role, state.
     */

    virtual void colorLink(unsigned int i, bool forwarding);
    virtual void visualizer();

    /**
     * @brief Obtain the root gate index
     * @return the root gate index or -1 if there is not root gate.
     */
    virtual int getRootIndex();

    Ieee8021DInterfaceData *getPortInterfaceData(unsigned int portNum);

};
#endif /* STPUTIL_H_ */
