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

//import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
//import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
//import inet.physicallayer.energyconsumer.SensorStateBasedEpEnergyConsumer;
import inet.visualizer.contract.IIntegratedVisualizer;

network Fadia
{
    parameters:
        int numCollectors;
        int numProvers;
        double connectionProb = default(1);
		int keyRingSize;
		int keyPoolSize;
        
        int keyidseed = default(0);
        int keyseed = default(1);
        
        int width = default(1000);
        int height = default(500);

        @display("bgb=$width,$height;bgg=100,1,grey95");
        @figure[title](type=label; pos=0,-1; anchor=sw; color=darkblue);

        @figure[rcvdPkText](type=indicatorText; pos=380,20; anchor=w; font=,18; textFormat="packets received: %g"; initialValue=0);
      //  @statistic[packetReceived](source=hostB.app[0].packetReceived; record=figure(count); targetFigure=rcvdPkText);
            
    types:
        channel C extends ned.DatarateChannel {    
            datarate = 100Mbps;
        }

    submodules:
        collector[numCollectors]: Collector;
        prover[numProvers]: Prover;
        visualizer: <default("IntegratedCanvasVisualizer")> like IIntegratedVisualizer if hasVisualizer() {
            @display("p=21,22");
        }

}

