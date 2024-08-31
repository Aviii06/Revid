#include <revid_engine/platform/Application.h>
#include <revid_engine/platform/EntryPoint.h>

class RevidApp : public Revid::Application
{
public:
    RevidApp(String title) : Application(std::move(title))
    {
        // std::cout << "Intialising Revid Application\n";
    }

protected:
    void Update(float deltaTime) override
    {
        // std::cout << "Updating\n";
    }

};

Revid::Application* CreateApplication()
{
    return new RevidApp("RevidApp");
}
