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


packet CommitReq
{
    int source;
    int depth;
    int kid[];
    int treeID;
}

packet CommitResp 
{
    int source;
    int destination;
    int kid;
    int treeID;
    int mac;
}

packet CommitAck
{
    int source;
    int destination;
	int treeID;
    int mac;
}

packet CommitTimeOut
{
    int device;
    int treeID;
}
    