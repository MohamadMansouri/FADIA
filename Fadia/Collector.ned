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




simple ACollector
{
    parameters:
        @signal[elapsedTime](type=double);
        @statistic[elapsedtimesig](title="Elapsed Time"; source="elapsedTime"; record=last);
        @signal[reportSize](type=unsigned long);
        @statistic[reportsize](title="Report Size"; source="reportSize"; record=vector);
        @signal[keyRevoked](type=long);
        @statistic[keyrevokedsig](title="Key revoked"; source="keyRevoked"; record=vector);
        
    gates:
        input radioIn @directIn;
        inout appio;
}

module Collector extends NodeBase
{
    parameters:
        @display("i=device/accesspoint;is=l;bgb=573.3,680.39996");
    gates:
        input radioIn @directIn;
		inout gate @loose;
		
    submodules:
        collectorapp: ACollector {

            @display("i=block/app,p,330.75,491.4;p=357.048,521.208");
        }
    connections:
        collectorapp.radioIn <-- radioIn;
        collectorapp.appio <--> gate;
}
