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
    Foam::hIAPWSThermo

Description

    Thermodynamics package templated on the equation of state, using the
    IAPWS-IF97 water properties with the help of freesteam
    http://freesteam.sourceforge.net/. The original connector was written by
    Christian Lucas for the openfoam-ext project.

    No entries in the subdictionary required. Pressure is taking dynamically
    from the solver.

SourceFiles
    hPolynomialThermoI.H
    hPolynomialThermo.C

\*---------------------------------------------------------------------------*/

#ifndef hIAPWSThermo_H
#define hIAPWSThermo_H

#include "scalar.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of friend functions and operators

template<class EquationOfState>
class hIAPWSThermo;

template<class EquationOfState>
inline hIAPWSThermo<EquationOfState> operator+
(
    const hIAPWSThermo<EquationOfState>&,
    const hIAPWSThermo<EquationOfState>&
);

template<class EquationOfState>
inline hIAPWSThermo<EquationOfState> operator-
(
    const hIAPWSThermo<EquationOfState>&,
    const hIAPWSThermo<EquationOfState>&
);

template<class EquationOfState>
inline hIAPWSThermo<EquationOfState> operator*
(
    const scalar,
    const hIAPWSThermo<EquationOfState>&
);

template<class EquationOfState>
inline hIAPWSThermo<EquationOfState> operator==
(
    const hIAPWSThermo<EquationOfState>&,
    const hIAPWSThermo<EquationOfState>&
);

template<class EquationOfState>
Ostream& operator<<
(
    Ostream&,
    const hIAPWSThermo<EquationOfState>&
);


/*---------------------------------------------------------------------------*\
                      Class hIAPWSThermo Declaration
\*---------------------------------------------------------------------------*/

template<class EquationOfState>
class hIAPWSThermo
:
    public EquationOfState
{
    // Private data

        //- Heat of formation
        //  Note: input in [J/kg], but internally uses [J/kmol]
        static const scalar Hf_=0;

        //- Standard entropy
        //  Note: input in [J/kg/K], but internally uses [J/kmol/K]
        static const scalar Sf_=0;

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
        inline hIAPWSThermo
        (
            const EquationOfState& pt,
            const scalar pMax,
            const scalar pMin
        );


public:

    // Constructors

        //- Construct from Istream
        hIAPWSThermo(Istream& is);

        //- Construct from dictionary
        hIAPWSThermo(const dictionary& dict);

        //- Construct as copy
        inline hIAPWSThermo(const hIAPWSThermo&);

        //- Construct as a named copy
        inline hIAPWSThermo(const word&, const hIAPWSThermo&);


    // Member Functions

        //- Return the instantiated type name
        static word typeName()
        {
            return "hIAPWS<" + EquationOfState::typeName() + '>';
        }

        //- Limit the temperature to be in the range Tlow_ to Thigh_
        inline scalar limit(const scalar) const;


        // Fundamental properties

            //- Heat capacity at constant pressure [J/(kmol K)]
            inline scalar cp(const scalar p, const scalar T) const;

            //- Absolute Enthalpy [J/kmol]
            inline scalar ha(const scalar p, const scalar T) const;

            //- Sensible enthalpy [J/kmol]
            inline scalar hs(const scalar p, const scalar T) const;

            //- Chemical enthalpy [J/kmol]
            inline scalar hc() const;

            //- Entropy [J/(kmol K)]
            inline scalar s(const scalar p, const scalar T) const;


        // I-O

            //- Write to Ostream
            void write(Ostream& os) const;


    // Member operators

        inline hIAPWSThermo& operator=(const hIAPWSThermo&);
        inline void operator+=(const hIAPWSThermo&);
        inline void operator-=(const hIAPWSThermo&);
        inline void operator*=(const scalar);


    // Friend operators

        friend hIAPWSThermo operator+ <EquationOfState>
        (
            const hIAPWSThermo&,
            const hIAPWSThermo&
        );

        friend hIAPWSThermo operator- <EquationOfState>
        (
            const hIAPWSThermo&,
            const hIAPWSThermo&
        );

        friend hIAPWSThermo operator* <EquationOfState>
        (
            const scalar,
            const hIAPWSThermo&
        );

        friend hIAPWSThermo operator== <EquationOfState>
        (
            const hIAPWSThermo&,
            const hIAPWSThermo&
        );


    // Ostream Operator

        friend Ostream& operator<< <EquationOfState>
        (
            Ostream&,
            const hIAPWSThermo&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "hIAPWSThermoI.H"

#ifdef NoRepository
#   include "hIAPWSThermo.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
