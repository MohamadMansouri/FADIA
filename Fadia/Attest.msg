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


message PullAttestReq
{
    int source;
    int kid[];
    double seniority;
}

message AttestReq 
{
    int source;
    int destination;
    int kid;
    int challenge;
    int mac;
}


message AttestResp
{
    int source;
    int destination;
	int response;
    int mac;
}

message AttestAck
{
    int source;
    int destination;
    int mac;
}

message AttestTimeOut
{
    int correspondent;
}
    