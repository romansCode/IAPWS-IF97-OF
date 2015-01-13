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

Class
    Foam::IAPWSTransport

Description
    Transport package using the IAPWS-IF97 water properties with the help of
    freesteam http://freesteam.sourceforge.net/. The original connector was
    written by Christian Lucas for the openfoam-ext project.

    No entries in the subdictionary required. Pressure is taking dynamically
    from the solver.

SourceFiles
    IAPWSTransportI.H
    IAPWSTransport.C

\*---------------------------------------------------------------------------*/

#ifndef IAPWSTransport_H
#define IAPWSTransport_H

#include "IAPWSTransport.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of friend functions and operators

template<class Thermo> class IAPWSTransport;

template<class Thermo>
inline IAPWSTransport<Thermo> operator+
(
    const IAPWSTransport<Thermo>&,
    const IAPWSTransport<Thermo>&
);

template<class Thermo>
inline IAPWSTransport<Thermo> operator-
(
    const IAPWSTransport<Thermo>&,
    const IAPWSTransport<Thermo>&
);

template<class Thermo>
inline IAPWSTransport<Thermo> operator*
(
    const scalar,
    const IAPWSTransport<Thermo>&
);

template<class Thermo>
inline IAPWSTransport<Thermo> operator==
(
    const IAPWSTransport<Thermo>&,
    const IAPWSTransport<Thermo>&
);

template<class Thermo>
Ostream& operator<<
(
    Ostream&,
    const IAPWSTransport<Thermo>&
);


/*---------------------------------------------------------------------------*\
                     Class IAPWSTransport Declaration
\*---------------------------------------------------------------------------*/

template<class Thermo>
class IAPWSTransport
:
    public Thermo
{
    // Private data

        //- max temperature at which clipping occurs [K]
        static const scalar TMax_ = 1073;

        //- min temperature at which clipping occurs [K]
        static const scalar TMin_ = 273;

        //- max pressure at which clipping occurs [K]
        const scalar pMax_;

        //- min pressure at which clipping occurs [K]
        const scalar pMin_;

    // Private Member Functions

        //- Construct from components
        inline IAPWSTransport
        (
            const Thermo& t
        );


public:

    // Constructors

        //- Construct copy
        inline IAPWSTransport(const IAPWSTransport&);

        //- Construct as named copy
        inline IAPWSTransport(const word&, const IAPWSTransport&);

        //- Construct from Istream
        IAPWSTransport(Istream& is);

        //- Construct from dictionary
        IAPWSTransport(const dictionary& dict);

        //- Construct and return a clone
        inline autoPtr<IAPWSTransport> clone() const;

        // Selector from Istream
        inline static autoPtr<IAPWSTransport> New(Istream& is);

        // Selector from dictionary
        inline static autoPtr<IAPWSTransport> New(const dictionary& dict);


    // Member functions

        //- Return the instantiated type name
        static word typeName()
        {
            return "IAPWSTransport<" + Thermo::typeName() + '>';
        }

        //- Dynamic viscosity [kg/ms]
        inline scalar mu(const scalar p, const scalar T) const;

        //- Thermal conductivity [W/mK]
        inline scalar kappa(const scalar p, const scalar T) const;

        //- Thermal diffusivity of enthalpy [kg/ms]
        inline scalar alphah(const scalar p, const scalar T) const;

        //- Write to Ostream
        void write(Ostream& os) const;


    // Member operators

        inline IAPWSTransport& operator=(const IAPWSTransport&);
        inline void operator+=(const IAPWSTransport&);
        inline void operator-=(const IAPWSTransport&);
        inline void operator*=(const scalar);


    // Friend operators

        friend IAPWSTransport operator+ <Thermo>
        (
            const IAPWSTransport&,
            const IAPWSTransport&
        );

        friend IAPWSTransport operator- <Thermo>
        (
            const IAPWSTransport&,
            const IAPWSTransport&
        );

        friend IAPWSTransport operator* <Thermo>
        (
            const scalar,
            const IAPWSTransport&
        );

        friend IAPWSTransport operator== <Thermo>
        (
            const IAPWSTransport&,
            const IAPWSTransport&
        );


    // Ostream Operator

        friend Ostream& operator<< <Thermo>
        (
            Ostream&,
            const IAPWSTransport&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "IAPWSTransportI.H"

#ifdef NoRepository
#   include "IAPWSTransport.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
