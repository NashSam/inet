//
// Copyright (C) 2004 Andras Varga
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

package inet.transportlayer.contract;

//
// Interface for TCP protocol implementations. All TCP implementations should
// implement this (i.e. declared as: TCP like ITcp)
// The existing implementations are these: ~Tcp, ~TcpNsc and ~TcpLwip.
//
// <b>Communication with applications</b>
//
// For communication between applications and TCP, the ~TcpCommandCode
// and ~TcpStatusInd enums are used as message kinds, and ~TcpCommand
// and its subclasses are used as control info.
//
// To open a connection from a client app, send a cMessage to TCP with
// TCP_C_OPEN_ACTIVE as message kind and a ~TcpOpenCommand object filled in
// and attached to it as control info. (The peer TCP will have to be LISTENing;
// the server app can achieve this with a similar cMessage but TCP_C_OPEN_PASSIVE
// message kind.) With passive open, there's a possibility to cause the connection
// "fork" on an incoming connection, leaving the original connection LISTENing
// on the port (see the fork field in ~TcpOpenCommand).
//
// The app can send data by assigning the TCP_C_SEND message kind
// and attaching a ~TCPSendCommand control info object to the data packet,
// and sending it to TCP. The app will receive data as messages
// with the TCP_I_DATA message kind and ~TCPSendCommand control info.
//
// To close, the client sends a cMessage to TCP with the TCP_C_CLOSE message kind
// and ~TcpCommand control info.
//
// TCP sends notifications to the application whenever there's a significant
// change in the state of the connection: established, remote TCP closed,
// closed, timed out, connection refused, connection reset, etc. These
// notifications are also cMessages with message kind TCP_I_xxx
// (TCP_I_ESTABLISHED, etc.) and ~TcpCommand as control info.
//
// One TCP module can serve several application modules, and several
// connections per application. When talking to applications, a
// connection is identified by the connId that is assigned by the application in
// the OPEN call.
//
// <b>Sockets</b>
//
// The TcpSocket C++ class is provided to simplify managing TCP connections
// from applications. TcpSocket handles the job of assembling and sending
// command messages (OPEN, CLOSE, etc) to TCP, and it also simplifies
// the task of dealing with packets and notification messages coming from TCP.
//
// <b>Communication with the IP layer</b>
//
// A TCP header is represented by the class ~TcpHeader.
//
// The TCP model relies on sending and receiving ~L3AddressReq/~L3AddressInd tags
// attached to TCP segment packets.
//
moduleinterface ITcp
{
    @display("i=block/wheelbarrow");
    gates:
        input appIn @labels(TcpCommand/down);
        input ipIn @labels(TcpHeader,Ipv4ControlInfo/up,Ipv6ControlInfo/up);
        output appOut @labels(TcpCommand/up);
        output ipOut @labels(TcpHeader,Ipv4ControlInfo/down,Ipv6ControlInfo/down);
}

