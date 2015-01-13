/*---------------------------------------------------------------------------*\
=========                 |
\\      /  F ield         | Unsupported Contributions for OpenFOAM
 \\    /   O peration     |
  \\  /    A nd           | Copyright (C) 2014 Christian Lukas
   \\/     M anipulation  |
-------------------------------------------------------------------------------

License
    This file is a derivative work of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.


Description:

Includes the definition of a struct from freestream
Code copied from freestream --> steam.h

\*---------------------------------------------------------------------------*/


typedef struct SteamState_R1_struct{
	double p, T;
} SteamState_R1;

typedef struct SteamState_R2_struct{
	double p, T;
} SteamState_R2;

typedef struct SteamState_R3_struct{
	double rho, T;
} SteamState_R3;

typedef struct SteamState_R4_struct{
	double T, x;
} SteamState_R4;

typedef struct SteamState_struct{
	char region;
	union{
		SteamState_R1 R1;
		SteamState_R2 R2;
		SteamState_R3 R3;
		SteamState_R4 R4;
	};
} SteamState;
