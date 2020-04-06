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


import inet.node.base.NodeBase;
//import inet.node.inet.WirelessHost;



simple AProver
{
    gates:
        input radioIn @directIn;
}

module Prover extends NodeBase
{
    parameters:
        double range;
        @display("i=misc/sensor3,is=s;bgb=589.05,603.225;r=$range");
        mobility.typename = default("StationaryMobility");

    gates:
        //        inout gate[]	@allowunconnected;
        input radioIn @directIn;

    submodules:
        proverapp: AProver {

            @display("i=block/app,p,625.27496,466.19998;p=348.07498,398.47498");
        }
    connections:
        proverapp.radioIn <-- radioIn;


}

