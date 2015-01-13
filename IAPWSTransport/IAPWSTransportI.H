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

//#include "specie.H"
#include "IAPWSTransport.H"
#include "IAPWS-IF97.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Thermo>
inline Foam::IAPWSTransport<Thermo>::IAPWSTransport
(
    const IAPWSTransport& pt
)
:
    Thermo(pt),
    pMax_(pt.pMax_),
    pMin_(pt.pMin_)
{}


template<class Thermo>
inline Foam::IAPWSTransport<Thermo>::IAPWSTransport
(const Thermo& t)
:
    Thermo(t),
    pMax_(t.pMax_),
    pMin_(t.pMin_)

{}


template<class Thermo>
inline Foam::IAPWSTransport<Thermo>::IAPWSTransport
(
    const word& name,
    const IAPWSTransport& pt
)
:
    Thermo(name, pt),
    pMax_(pt.pMax_),
    pMin_(pt.pMin_)

{}


template<class Thermo>
inline Foam::autoPtr<Foam::IAPWSTransport<Thermo> >
Foam::IAPWSTransport<Thermo>::clone() const
{
    return autoPtr<IAPWSTransport<Thermo> >
    (
        new IAPWSTransport<Thermo>(*this)
    );
}


template<class Thermo>
inline Foam::autoPtr<Foam::IAPWSTransport<Thermo> >
Foam::IAPWSTransport<Thermo>::New(Istream& is)
{
    return autoPtr<IAPWSTransport<Thermo> >
    (
        new IAPWSTransport<Thermo>(is)
    );
}


template<class Thermo>
inline Foam::autoPtr<Foam::IAPWSTransport<Thermo> >
Foam::IAPWSTransport<Thermo>::New(const dictionary& dict)
{
    return autoPtr<IAPWSTransport<Thermo> >
    (
        new IAPWSTransport<Thermo>(dict)
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Thermo>
inline Foam::scalar Foam::IAPWSTransport<Thermo>::mu
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
    return mu_pT(pLim,TLim);
}


template<class Thermo>
inline Foam::scalar Foam::IAPWSTransport<Thermo>::kappa
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
    return tc_pT(pLim,TLim);
}


template<class Thermo>
inline Foam::scalar Foam::IAPWSTransport<Thermo>::alphah
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
    return kappa(pLim, TLim)/cp_pT(pLim, TLim);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Thermo>
inline Foam::IAPWSTransport<Thermo>&
Foam::IAPWSTransport<Thermo>::operator=
(
    const IAPWSTransport<Thermo>& pt
)
{
    Thermo::operator=(pt);

    return *this;
}


template<class Thermo>
inline void Foam::IAPWSTransport<Thermo>::operator+=
(
    const IAPWSTransport<Thermo>& pt
)
{
    scalar molr1 = this->nMoles();

    Thermo::operator+=(pt);

    molr1 /= this->nMoles();
    scalar molr2 = pt.nMoles()/this->nMoles();
}


template<class Thermo>
inline void Foam::IAPWSTransport<Thermo>::operator-=
(
    const IAPWSTransport<Thermo>& pt
)
{
    scalar molr1 = this->nMoles();

    Thermo::operator-=(pt);

    molr1 /= this->nMoles();
    scalar molr2 = pt.nMoles()/this->nMoles();
}


template<class Thermo>
inline void Foam::IAPWSTransport<Thermo>::operator*=
(
    const scalar s
)
{
    Thermo::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Thermo>
inline Foam::IAPWSTransport<Thermo> Foam::operator+
(
    const IAPWSTransport<Thermo>& pt1,
    const IAPWSTransport<Thermo>& pt2
)
{
    Thermo t
    (
        static_cast<const Thermo&>(pt1) + static_cast<const Thermo&>(pt2)
    );

    scalar molr1 = pt1.nMoles()/t.nMoles();
    scalar molr2 = pt2.nMoles()/t.nMoles();

    return IAPWSTransport<Thermo>
    (
        t
    );
}


template<class Thermo>
inline Foam::IAPWSTransport<Thermo> Foam::operator-
(
    const IAPWSTransport<Thermo>& pt1,
    const IAPWSTransport<Thermo>& pt2
)
{
    Thermo t
    (
        static_cast<const Thermo&>(pt1) - static_cast<const Thermo&>(pt2)
    );

    scalar molr1 = pt1.nMoles()/t.nMoles();
    scalar molr2 = pt2.nMoles()/t.nMoles();

    return IAPWSTransport<Thermo>
    (
        t
    );
}


template<class Thermo>
inline Foam::IAPWSTransport<Thermo> Foam::operator*
(
    const scalar s,
    const IAPWSTransport<Thermo>& pt
)
{
    return IAPWSTransport<Thermo>
    (
        s*static_cast<const Thermo&>(pt)
    );
}


template<class Thermo>
inline Foam::IAPWSTransport<Thermo> Foam::operator==
(
    const IAPWSTransport<Thermo>& pt1,
    const IAPWSTransport<Thermo>& pt2
)
{
    return pt2 - pt1;
}


// ************************************************************************* //
