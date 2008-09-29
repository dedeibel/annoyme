#include <iostream>

#include "AlsaOuput.h"

AlsaOuput::AlsaOuput()
{

}

AlsaOuput::~AlsaOuput()
{

}

void AlsaOuput::playSound(Sample sound)
{
   std::cout << "Playing sound " << sound.name << std::endl;
}
