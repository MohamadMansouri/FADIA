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

//
// TODO generated message class
//


message CommitReq
{
    int source;
    int kid[];
    int depth;
    int treeID;
}

message CommitResp 
{
    int source;
    int destination;
    int kid;
    int treeID;
    int mac;
}

message CommitAck
{
    int source;
    int destination;
	int treeID;
    int mac;
}

message CommitTimeOut
{
    int device;
    int treeID;
}
    