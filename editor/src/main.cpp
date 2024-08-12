#include <revid_engine/Engine.h>
#include <revid_engine/ServiceLocater.h>
#include <iostream>

int main()
{
    RevidEngine::Init();
    ServiceLocator::GetWindow()->OpenWindow();

    while(!ServiceLocator::GetWindow()->Update())
    {

    }

}
