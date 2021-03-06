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
	parameters:
        @signal[dataSent](type=double);
        @statistic[datasent](title="Data Sent"; source=dataSent; record=vector);
        @signal[dataRcvd](type=double);
        @statistic[datarcvd](title="Data Received"; source=dataRcvd; record=vector);
        @signal[crashTime](type=double);
        @statistic[crashtime](title="Crash Time"; source="crashTime"; record=vector);
        @signal[keyRevoked](type=long);
        @statistic[keyrevokedsig](title="Key revoked"; source="keyRevoked"; record=vector);
        
    gates:
        input radioIn @directIn;
        inout appio[] @loose;
}

module Prover extends NodeBase
{
    parameters:
        double range = default(50);
        int aggsize = default(99999999);
        int neighbors = default(0);
        int type = default(0);			// NA = 0, SERV = 1, SKY = 2, PI2 = 3
        @display("i=misc/sensoroff;is=s;bgb=589.05,603.225;r=$range");

    gates:
        //        inout gate[]	@allowunconnected;
        input radioIn @directIn;
		inout gate[neighbors + 1] @loose;
    submodules:
        proverapp: AProver {

            @display("i=block/app,p,625.27496,466.19998;p=348.07498,398.47498");
        }
    connections:
        proverapp.radioIn <-- radioIn;
        for i=0..sizeof(gate) - 1 {
			proverapp.appio++ <--> gate[i];
        }

}

