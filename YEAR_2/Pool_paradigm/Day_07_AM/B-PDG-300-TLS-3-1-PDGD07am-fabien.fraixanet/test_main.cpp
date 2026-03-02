#include <iostream>
#include <string>
#include "Borg.hpp"
#include "Federation.hpp"
#include "WarpSystem.hpp"
#include "Destination.hpp"
#include "Admiral.hpp"
#include "BorgQueen.hpp"

int main ()
{
Federation::Starfleet::Ship UssJanissary (289 , 132 , "Janissary", 6 , 20) ;
Federation::Starfleet::Admiral Pierce ("Pierce") ;
Borg::Ship Cube(25);
Pierce.fire(&UssJanissary , &Cube);
Pierce.fire(&UssJanissary , &Cube);
return 0;
}
