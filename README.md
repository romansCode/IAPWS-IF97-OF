IAPWS-IF97-OF
=============

IAPWS-IF97 water properties for OpenFOAM (R). Description and wiki page can be found in the openfoam wiki http://openfoamwiki.net/index.php/Contrib/IAPWS-IF97-OF

* Installation

  * Prerequisites

    1. Full installation of OpenFOAM >=2.2 from www.openfoam.org
    2. Installed freesteam >=2.0 from http://freesteam.sourceforge.net/ 
  
  * Installation
  
    1. git clone https://github.com/romansCode/IAPWS-IF97-OF.git
    2. cd IAPWS-IF97-OF
    3. wclean && wmake libso

* Usage
  
  1. add libIAPWSRangeThermo.so and libfreesteam.so to the libraries in controlDict
     libs
     {
       "libIAPWSRangeThermo.so"
       "libfreesteam.so"
     }
  2. use the thermophysicalProperties file provided in example folder
