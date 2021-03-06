//
// Copyright (C) 2009 Thomas Reschka
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

package inet.common.queue;

import inet.common.IHook;

//
// Ordinal Based Dropper module.
//
simple OrdinalBasedDropper like IHook
{
    parameters:
        string dropsVector = default(""); // the dropsVector format is "<packetNumber>;<packetNumber>;...;"
                                          // packet numbers in ascending order e.g. "5;6;12;13;"
        @display("i=block/downarrow");
        @signal[packetReceived](type=cMessage);
        @signal[packetSent](type=cMessage);
        @signal[packetDropped](type=cMessage);
        @statistic[packetReceived](title="packets received"; source=packetReceived; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[packetSent](title="packets sent"; source=packetSent; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[dropPk](title="packets dropped"; source=packetDropped; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
    gates:
        input in;
        output out;
}

