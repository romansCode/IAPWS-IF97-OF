/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2012 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

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

\*---------------------------------------------------------------------------*/

#include "eosIAPWS.H"
#include "IOstreams.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Specie>
eosIAPWS<Specie>::eosIAPWS(Istream& is)
:
    Specie(is),
    TMax_(0.0),
    TMin_(0.0),
    pMax_(0.0),
    pMin_(0.0)
{
    // TODO: change to only read if values are given, otherwise use full range
    const dictionary& dict("thermophysicalProperties");
    TMax_ = readScalar(dict.subDict("IAPWSProperties").lookup("TMax"));
    TMin_ = readScalar(dict.subDict("IAPWSProperties").lookup("TMin"));
    pMax_ = readScalar(dict.subDict("IAPWSProperties").lookup("pMax"));
    pMin_ = readScalar(dict.subDict("IAPWSProperties").lookup("pMin"));
}


template<class Specie>
eosIAPWS<Specie>::eosIAPWS(const dictionary& dict)
:
    Specie(dict),
    TMax_(readScalar(dict.subDict("IAPWSProperties").lookup("TMax"))),
    TMin_(readScalar(dict.subDict("IAPWSProperties").lookup("TMin"))),
    pMax_(readScalar(dict.subDict("IAPWSProperties").lookup("pMax"))),
    pMin_(readScalar(dict.subDict("IAPWSProperties").lookup("pMin")))
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Specie>
void eosIAPWS<Specie>::write(Ostream& os) const
{
    os  << this->name() << endl;
    os  << token::BEGIN_BLOCK << incrIndent << nl;

    Specie::write(os);

    dictionary dict("equationOfState");
    os  << indent << dict.dictName() << dict;

    os  << decrIndent << token::END_BLOCK << nl;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class Specie>
Ostream& operator<<(Ostream& os, const eosIAPWS<Specie>& ip)
{
    os  << static_cast<const Specie&>(ip) << tab
        << "rho based on IAPWS-IF97 with dynamically read pressure" << tab;

    os.check
    (
        "Ostream& operator<<"
        "(Ostream& os, const eosIAPWS<Specie>& ip)"
    );

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
