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

#include "hIAPWSThermo.H"
#include "IOstreams.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class EquationOfState>
Foam::hIAPWSThermo<EquationOfState>::hIAPWSThermo
(
    Istream& is
)
:
    EquationOfState(is),
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


template<class EquationOfState>
Foam::hIAPWSThermo<EquationOfState>::hIAPWSThermo
(
    const dictionary& dict
)
:
    EquationOfState(dict),
    TMax_(readScalar(dict.subDict("IAPWSProperties").lookup("TMax"))),
    TMin_(readScalar(dict.subDict("IAPWSProperties").lookup("TMin"))),
    pMax_(readScalar(dict.subDict("IAPWSProperties").lookup("pMax"))),
    pMin_(readScalar(dict.subDict("IAPWSProperties").lookup("pMin")))
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class EquationOfState>
void Foam::hIAPWSThermo<EquationOfState>::write
(
    Ostream& os
) const
{
    EquationOfState::write(os);

    dictionary dict("thermodynamics");
    os  << indent << dict.dictName() << dict;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class EquationOfState>
Foam::Ostream& Foam::operator<<
(
    Ostream& os,
    const hIAPWSThermo<EquationOfState>& pt
)
{
    os  << static_cast<const EquationOfState&>(pt) << tab
        << pt.Hf_/pt.W() << tab
        << pt.Sf_/pt.W() << tab;

    os.check
    (
        "operator<<"
        "("
            "Ostream&, "
            "const hIAPWSThermo<EquationOfState>&"
        ")"
    );

    return os;
}


// ************************************************************************* //
