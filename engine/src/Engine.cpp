#include <revid_engine/Engine.h>
#include <revid_engine/ServiceLocater.h>
#include <iostream>

#include "platform/CustomWindow.h"

void RevidEngine::Init()
{
    std::cout << "Initialising Engine\n";

    ServiceLocator::Provide(new CustomWindow());
}
