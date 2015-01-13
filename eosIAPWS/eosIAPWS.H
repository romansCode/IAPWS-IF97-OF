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
    Foam::eosIAPWS

Description
    Compressible description of the equation of state based on the IAPWS-IF97
    water properties from http://freesteam.sourceforge.net/

    The original connector was written by Christian Lucas for the
    openfoam-ext project.

    No entries in the subdictionary required. Pressure is taking dynamically
    from the solver.

SourceFiles
    eosIAPWSI.H
    eosIAPWS.C


\*---------------------------------------------------------------------------*/

#ifndef eosIAPWS_H
#define eosIAPWS_H

#include "autoPtr.H"
// #include "Polynomial.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of friend functions and operators

template<class Specie>
class eosIAPWS;

template<class Specie>
eosIAPWS<Specie> operator+
(
    const eosIAPWS<Specie>&,
    const eosIAPWS<Specie>&
);

template<class Specie>
eosIAPWS<Specie> operator-
(
    const eosIAPWS<Specie>&,
    const eosIAPWS<Specie>&
);

template<class Specie>
eosIAPWS<Specie> operator*
(
    const scalar,
    const eosIAPWS<Specie>&
);

template<class Specie>
eosIAPWS<Specie> operator==
(
    const eosIAPWS<Specie>&,
    const eosIAPWS<Specie>&
);

template<class Specie>
Ostream& operator<<
(
    Ostream&,
    const eosIAPWS<Specie>&
);


/*---------------------------------------------------------------------------*\
                        Class eosIAPWS Declaration
\*---------------------------------------------------------------------------*/

template<class Specie>
class eosIAPWS
:
    public Specie
{
    // Private data

    // specific gas constant for water [J/kg/K]
    static const scalar RSpecific_ = 0.461526e3;  // #NOTE: for compressibility

    //- max temperature at which clipping occurs [K]
    static const scalar TMax_ = 1073;

    //- min temperature at which clipping occurs [K]
    static const scalar TMin_ = 273.15;

    //- max pressure at which clipping occurs [K]
    const scalar pMax_;

    //- min pressure at which clipping occurs [K]
    const scalar pMin_;


public:

    // Constructors

        //- Construct from components
        inline eosIAPWS
        (
            const Specie& sp,
            const scalar pMax,
            const scalar pMin
        );

        //- Construct from Istream
        eosIAPWS(Istream&);

        //- Construct from dictionary
        eosIAPWS(const dictionary& dict);

        //- Construct as copy
        inline eosIAPWS(const eosIAPWS&);

        //- Construct as named copy
        inline eosIAPWS(const word& name, const eosIAPWS&);

        //- Construct and return a clone
        inline autoPtr<eosIAPWS> clone() const;

        // Selector from Istream
        inline static autoPtr<eosIAPWS> New(Istream& is);

        // Selector from dictionary
        inline static autoPtr<eosIAPWS> New(const dictionary& dict);


    // Member functions

        //- Return the instantiated type name
        static word typeName()
        {
            return "eosIAPWS<" + word(Specie::typeName_()) + '>';
        }


        // Fundamental properties

            //- Is the equation of state is incompressible i.e. rho != f(p)
            static const bool incompressible = false;

            //- Is the equation of state is isochoric i.e. rho = const
            static const bool isochoric = false;

            //- Return density [kg/m^3]
            inline scalar rho(scalar p, scalar T) const;

            //- Return compressibility rho/p [s^2/m^2]
            inline scalar psi(scalar p, scalar T) const;

            //- Return compression factor []
            inline scalar Z(scalar p, scalar T) const;

            //- Return (cp - cv) [J/(kmol K]
            inline scalar cpMcv(scalar p, scalar T) const;


        // IO

            //- Write to Ostream
            void write(Ostream& os) const;


    // Member operators

        inline eosIAPWS& operator=(const eosIAPWS&);
        inline void operator+=(const eosIAPWS&);
        inline void operator-=(const eosIAPWS&);

        inline void operator*=(const scalar);


    // Friend operators

        friend eosIAPWS operator+ <Specie>
        (
            const eosIAPWS&,
            const eosIAPWS&
        );

        friend eosIAPWS operator- <Specie>
        (
            const eosIAPWS&,
            const eosIAPWS&
        );

        friend eosIAPWS operator* <Specie>
        (
            const scalar s,
            const eosIAPWS&
        );

        friend eosIAPWS operator== <Specie>
        (
            const eosIAPWS&,
            const eosIAPWS&
        );


    // Ostream Operator

        friend Ostream& operator<< <Specie>
        (
            Ostream&,
            const eosIAPWS&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeeosIAPWS()                                          \
                                                                             \
defineTemplateTypeNameAndDebugWithName                                       \
(                                                                            \
    eosIAPWS<Specie>,                                         \
    "eosIAPWS",                                            \
    0                                                                        \
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "eosIAPWSI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "eosIAPWS.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
