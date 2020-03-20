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
    gates:
        input radioIn @directIn;
}

module Collector extends NodeBase
{
    parameters:
        @display("i=device/accesspoint;bgb=573.3,680.39996");

    gates:
        input radioIn @directIn;

    submodules:
        collectorapp: ACollector {

            @display("i=block/app,p=330.75,491.4");
        }
    connections:
        collectorapp.radioIn <-- radioIn;
}
