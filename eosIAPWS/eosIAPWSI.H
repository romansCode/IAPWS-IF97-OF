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

#include "eosIAPWS.H"
#include "IAPWS-IF97.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Specie>
inline Foam::eosIAPWS<Specie>::eosIAPWS
(
    const Specie& sp,
    const scalar pMax,
    const scalar pMin
)
:
    Specie(sp),
    pMax_(pMax),
    pMin_(pMin)
{}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Specie>
inline Foam::eosIAPWS<Specie>::eosIAPWS
(
    const eosIAPWS<Specie>& ip
)
:
    Specie(ip),
    pMax_(ip.pMax_),
    pMin_(ip.pMin_)
{}


template<class Specie>
inline Foam::eosIAPWS<Specie>::eosIAPWS
(
    const word& name,
    const eosIAPWS<Specie>& ip
)
:
    Specie(name, ip),
    pMax_(ip.pMax_),
    pMin_(ip.pMin_)
{}


template<class Specie>
inline Foam::autoPtr<Foam::eosIAPWS<Specie> >
Foam::eosIAPWS<Specie>::clone() const
{
    return autoPtr<eosIAPWS<Specie> >
    (
        new eosIAPWS<Specie>(*this)
    );
}


template<class Specie>
inline Foam::autoPtr<Foam::eosIAPWS<Specie> >
Foam::eosIAPWS<Specie>::New(Istream& is)
{
    return autoPtr<eosIAPWS<Specie> >
    (
        new eosIAPWS<Specie>(is)
    );
}


template<class Specie>
inline Foam::autoPtr<Foam::eosIAPWS<Specie> >
Foam::eosIAPWS<Specie>::New(const dictionary& dict)
{
    return autoPtr<eosIAPWS<Specie> >
    (
        new eosIAPWS<Specie>(dict)
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Specie>
inline Foam::scalar Foam::eosIAPWS<Specie>::rho
(
    scalar p,
    scalar T
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
    return rho_pT(pLim,TLim);
}


template<class Specie>
inline Foam::scalar Foam::eosIAPWS<Specie>::psi
(
    scalar p,
    scalar T
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

    return psiH_pT(pLim,TLim);  // #NOTE: for compressibility

}


template<class Specie>
inline Foam::scalar Foam::eosIAPWS<Specie>::Z
(
    scalar p,
    scalar T
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
    return pLim/rho_pT(pLim,TLim)/RSpecific_/TLim; // #NOTE: for compressibility
}


template<class Specie>
inline Foam::scalar Foam::eosIAPWS<Specie>::cpMcv
(
    scalar p,
    scalar T
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
    return (cp_pT(pLim,TLim)-cv_pT(pLim,TLim));
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Specie>
inline Foam::eosIAPWS<Specie>&
Foam::eosIAPWS<Specie>::operator=
(
    const eosIAPWS<Specie>& ip
)
{
    Specie::operator=(ip);

    return *this;
}


template<class Specie>
inline void Foam::eosIAPWS<Specie>::operator+=
(
    const eosIAPWS<Specie>& ip
)
{
    scalar molr1 = this->nMoles();

    Specie::operator+=(ip);

    molr1 /= this->nMoles();
    scalar molr2 = ip.nMoles()/this->nMoles();

}


template<class Specie>
inline void Foam::eosIAPWS<Specie>::operator-=
(
    const eosIAPWS<Specie>& ip
)
{
    scalar molr1 = this->nMoles();

    Specie::operator-=(ip);

    molr1 /= this->nMoles();
    scalar molr2 = ip.nMoles()/this->nMoles();
}


template<class Specie>
inline void Foam::eosIAPWS<Specie>::operator*=(const scalar s)
{
    Specie::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Specie>
Foam::eosIAPWS<Specie> Foam::operator+
(
    const eosIAPWS<Specie>& ip1,
    const eosIAPWS<Specie>& ip2
)
{
    scalar nMoles = ip1.nMoles() + ip2.nMoles();
    scalar molr1 = ip1.nMoles()/nMoles;
    scalar molr2 = ip2.nMoles()/nMoles;

    return eosIAPWS<Specie>
    (
        static_cast<const Specie&>(ip1)
      + static_cast<const Specie&>(ip2)
    );
}


template<class Specie>
Foam::eosIAPWS<Specie> Foam::operator-
(
    const eosIAPWS<Specie>& ip1,
    const eosIAPWS<Specie>& ip2
)
{
    scalar nMoles = ip1.nMoles() + ip2.nMoles();
    scalar molr1 = ip1.nMoles()/nMoles;
    scalar molr2 = ip2.nMoles()/nMoles;

    return eosIAPWS<Specie>
    (
        static_cast<const Specie&>(ip1)
      - static_cast<const Specie&>(ip2)
    );
}


template<class Specie>
Foam::eosIAPWS<Specie> Foam::operator*
(
    const scalar s,
    const eosIAPWS<Specie>& ip
)
{
    return eosIAPWS<Specie>
    (
        s*static_cast<const Specie&>(ip)
    );
}


template<class Specie>
Foam::eosIAPWS<Specie> Foam::operator==
(
    const eosIAPWS<Specie>& ip1,
    const eosIAPWS<Specie>& ip2
)
{
    return ip2 - ip1;
}


// ************************************************************************* //
