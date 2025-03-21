#include "app/Application.h"

int main(int argc, char** argv)
{
    auto* app = new Revid::Application("revid");

    app->Run();

    delete app;
}
