#include "Spells.hpp"

#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[])
{
    try
    {
        Spells spellsApp;
        return spellsApp.run();
    }
    catch (std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
}
