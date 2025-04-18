#include "app/Application.h"

int main(int argc, char** argv)
{
    auto* app = new RevidEditor::Application("Revid Editor");

    app->Run();

    delete app;
}
