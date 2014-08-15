/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
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

#include "IAPWSTransport.H"
#include "IOstreams.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Thermo>
Foam::IAPWSTransport<Thermo>::IAPWSTransport(Istream& is)
:
    Thermo(is),
    pMax_(0.0),
    pMin_(0.0)
{
    // TODO: change to only read if values are given, otherwise use full range
    const dictionary& dict("thermophysicalProperties");
    pMax_ = readScalar(dict.subDict("IAPWSProperties").lookup("pMax"));
    pMin_ = readScalar(dict.subDict("IAPWSProperties").lookup("pMin"));
}


template<class Thermo>
Foam::IAPWSTransport<Thermo>::IAPWSTransport
(
    const dictionary& dict
)
:
    Thermo(dict),
    pMax_(readScalar(dict.subDict("IAPWSProperties").lookup("pMax"))),
    pMin_(readScalar(dict.subDict("IAPWSProperties").lookup("pMin")))
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Thermo>
void Foam::IAPWSTransport<Thermo>::write(Ostream& os) const
{
    os  << this->name() << endl;
    os  << token::BEGIN_BLOCK << incrIndent << nl;

    Thermo::write(os);

    dictionary dict("transport");
    os  << indent << dict.dictName() << dict;

    os  << decrIndent << token::END_BLOCK << nl;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Thermo>
Foam::Ostream& Foam::operator<<
(
    Ostream& os,
    const IAPWSTransport<Thermo>& pt
)
{
    os  << static_cast<const Thermo&>(pt) << tab
        << "transport variables based on IAPWS-IF97<" << tab;

    os.check
    (
        "Ostream& operator<<"
        "("
            "Ostream&, "
            "const IAPWSTransport<Thermo>&"
        ")"
    );

    return os;
}


// ************************************************************************* //
