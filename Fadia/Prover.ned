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


import inet.node.inet.SensorNode;
//import inet.node.inet.WirelessHost;


module Prover extends SensorNode
{
    
	parameters:
		numWlanInterfaces = 1;    // set number of wlan interfaces to 1
		
		wlan[*].typename = default("Ieee80211Interface");
		mobility.typename = default("StationaryMobility");	       
}