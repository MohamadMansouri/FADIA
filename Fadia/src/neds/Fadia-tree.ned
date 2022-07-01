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

network Fadiatree
{
    parameters:
        int numCollectors;
        int numProvers;
        double connectionProb = default(1);
        int keyRingSize;
        int keyPoolSize;

        int keyIdSeed = default(0);
        int keySeed = default(1);

        double width = default(1000);
        double height = default(500);
		
//		double  posy = 5;
//		double posx = width / 2;	

        int htime = default(120);

		int neighbors = default(2);
		
		int root = 0;
		int maxdepth = default(5);
		bool statadapt = default(false);
		
        @display("bgb=$width,$height;bgg=100,1,grey95");
        @figure[title](type=label; pos=0,-1; anchor=sw; color=darkblue);

    types:
        channel C extends ned.DatarateChannel
        {
            datarate = 12.51Mbps;
            delay = 2.3ms;
        }

    submodules:
        collector[numCollectors]: Collector {
//            @display("p=$posx,$posy");
        }
        prover[numProvers]: Prover;
//        visualizer: <default("IntegratedCanvasVisualizer")> like IIntegratedVisualizer if hasVisualizer() {
//            @display("p=5,5");
//        }
   connections allowunconnected:
       collector[0].gate <--> C <--> prover[root].gate++;
       
       for i=0..numProvers - 1, for j=1..neighbors
       {
       		prover[i].gate++ <--> C <--> prover[neighbors*i+j].gate++ if neighbors*i+j < numProvers;
       }

//	   for i=0..numProvers - 1
//	   {
//	        prover[i].gate++ <--> C <--> prover[int(floor(double(i/2)))*(neighbors+1) + 1].gate++ if prover[i].type == 2 && floor(double(i/2))*(neighbors+1) + 1 < numProvers;
//	   }
//       
//       for i=0..numProvers - 1, for j=1..neighbors
//       {
//       		prover[i].gate++ <--> C <--> prover[int(floor(double(i/2)))*(neighbors+1)+1+j].gate++ if prover[i].type == 3 && floor(double(i/2))*(neighbors+1)+1+j < numProvers;
//       }
}
