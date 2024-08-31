#pragma once

#include <revid_engine/Engine.h>
#include <revid_engine/ServiceLocater.h>
#include <revid_engine/platform/Application.h>

#include <iostream>

int main(int argc, char** argv)
{
    auto* app = CreateApplication();

    app->Run();

    delete app;
}
