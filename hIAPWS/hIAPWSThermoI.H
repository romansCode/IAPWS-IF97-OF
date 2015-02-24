/*---------------------------------------------------------------------------*\
=========                 |
\\      /  F ield         | Unsupported Contributions for OpenFOAM
 \\    /   O peration     |
  \\  /    A nd           | Copyright (C) 2015 Roman Thiele
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

\*---------------------------------------------------------------------------*/

#include "hIAPWSThermo.H"
#include "IAPWS-IF97.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class EquationOfState>
inline Foam::hIAPWSThermo<EquationOfState>::hIAPWSThermo
(
    const EquationOfState& pt,
    const scalar pMax,
    const scalar pMin
)
:
    EquationOfState(pt),
    pMax_(pMax),
    pMin_(pMin)
{}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class EquationOfState>
inline Foam::hIAPWSThermo<EquationOfState>::hIAPWSThermo
(
    const hIAPWSThermo& pt
)
:
    EquationOfState(pt),
    pMax_(pt.pMax_),
    pMin_(pt.pMin_)
{}


template<class EquationOfState>
inline Foam::hIAPWSThermo<EquationOfState>::hIAPWSThermo
(
    const word& name,
    const hIAPWSThermo& pt
)
:
    EquationOfState(name, pt),
    pMax_(pt.pMax_),
    pMin_(pt.pMin_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class EquationOfState>
inline Foam::scalar Foam::hIAPWSThermo<EquationOfState>::limit
(
    const scalar T
) const
{
    return T;
}


template<class EquationOfState>
inline Foam::scalar Foam::hIAPWSThermo<EquationOfState>::cp
(
    const scalar p, const scalar T
) const
{
    // TODO: give warnings when clipping
    // set range for temperature
    scalar TLim = min(T,TMax_);  // bound max temperature
    TLim = max(TLim,TMin_);  // bound min temperature

    // set range for pressure
    scalar pLim = min(p,pMax_);   // bound max pressure
    pLim = max(pLim,pMin_);   // bound min pressure

    // return value
    // multiplication by W(), because assumed to be given per mole
    return cp_pT(pLim, TLim)*this->W();
}


template<class EquationOfState>
inline Foam::scalar Foam::hIAPWSThermo<EquationOfState>::ha
(
    const scalar p, const scalar T
) const
{
    // TODO: give warnings when clipping
    // set range for temperature
    scalar TLim = min(T,TMax_);  // bound max temperature
    TLim = max(TLim,TMin_);  // bound min temperature

    // set range for pressure
    scalar pLim = min(p,pMax_);   // bound max pressure
    pLim = max(pLim,pMin_);   // bound min pressure

    // return value
    // multiplication by W(), because assumed to be given per mole
    return h_pT(pLim, TLim)*this->W();
}


template<class EquationOfState>
inline Foam::scalar Foam::hIAPWSThermo<EquationOfState>::hs
(
    const scalar p, const scalar T
) const
{
    // TODO: give warnings when clipping
    // set range for temperature
    scalar TLim = min(T,TMax_);  // bound max temperature
    TLim = max(TLim,TMin_);  // bound min temperature

    // set range for pressure
    scalar pLim = min(p,pMax_);   // bound max pressure
    pLim = max(pLim,pMin_);   // bound min pressure

    // return value
    return ha(pLim, TLim) - hc();
}


template<class EquationOfState>
inline Foam::scalar Foam::hIAPWSThermo<EquationOfState>::hc()
const
{
    return Hf_;
}


template<class EquationOfState>
inline Foam::scalar Foam::hIAPWSThermo<EquationOfState>::s
(
    const scalar p,
    const scalar T
) const
{
    // TODO: give warnings when clipping
    // set range for temperature
    scalar TLim = min(T,TMax_);  // bound max temperature
    TLim = max(TLim,TMin_);  // bound min temperature

    // set range for pressure
    scalar pLim = min(p,pMax_);   // bound max pressure
    pLim = max(pLim,pMin_);   // bound min pressure

    // return value
    return s_pT(pLim, TLim);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class EquationOfState>
inline Foam::hIAPWSThermo<EquationOfState>&
Foam::hIAPWSThermo<EquationOfState>::operator=
(
    const hIAPWSThermo<EquationOfState>& pt
)
{
    EquationOfState::operator=(pt);

    return *this;
}


template<class EquationOfState>
inline void Foam::hIAPWSThermo<EquationOfState>::operator+=
(
    const hIAPWSThermo<EquationOfState>& pt
)
{
    scalar molr1 = this->nMoles();

    EquationOfState::operator+=(pt);

    molr1 /= this->nMoles();
    scalar molr2 = pt.nMoles()/this->nMoles();

}


template<class EquationOfState>
inline void Foam::hIAPWSThermo<EquationOfState>::operator-=
(
    const hIAPWSThermo<EquationOfState>& pt
)
{
    scalar molr1 = this->nMoles();

    EquationOfState::operator-=(pt);

    molr1 /= this->nMoles();
    scalar molr2 = pt.nMoles()/this->nMoles();

}


template<class EquationOfState>
inline void Foam::hIAPWSThermo<EquationOfState>::operator*=
(
    const scalar s
)
{
    EquationOfState::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class EquationOfState>
inline Foam::hIAPWSThermo<EquationOfState> Foam::operator+
(
    const hIAPWSThermo<EquationOfState>& pt1,
    const hIAPWSThermo<EquationOfState>& pt2
)
{
    EquationOfState eofs = pt1;
    eofs += pt2;

    scalar molr1 = pt1.nMoles()/eofs.nMoles();
    scalar molr2 = pt2.nMoles()/eofs.nMoles();

    return hIAPWSThermo<EquationOfState>
    (
        eofs,
        molr1*pt1.Hf_ + molr2*pt2.Hf_,
        molr1*pt1.Sf_ + molr2*pt2.Sf_
    );
}


template<class EquationOfState>
inline Foam::hIAPWSThermo<EquationOfState> Foam::operator-
(
    const hIAPWSThermo<EquationOfState>& pt1,
    const hIAPWSThermo<EquationOfState>& pt2
)
{
    EquationOfState eofs = pt1;
    eofs -= pt2;

    scalar molr1 = pt1.nMoles()/eofs.nMoles();
    scalar molr2 = pt2.nMoles()/eofs.nMoles();

    return hIAPWSThermo<EquationOfState>
    (
        eofs,
        molr1*pt1.Hf_ - molr2*pt2.Hf_,
        molr1*pt1.Sf_ - molr2*pt2.Sf_
    );
}


template<class EquationOfState>
inline Foam::hIAPWSThermo<EquationOfState> Foam::operator*
(
    const scalar s,
    const hIAPWSThermo<EquationOfState>& pt
)
{
    return hIAPWSThermo<EquationOfState>
    (
        s*static_cast<const EquationOfState&>(pt),
        pt.Hf_,
        pt.Sf_
    );
}


template<class EquationOfState>
inline Foam::hIAPWSThermo<EquationOfState> Foam::operator==
(
    const hIAPWSThermo<EquationOfState>& pt1,
    const hIAPWSThermo<EquationOfState>& pt2
)
{
    return pt2 - pt1;
}


// ************************************************************************* //
