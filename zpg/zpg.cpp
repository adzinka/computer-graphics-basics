#include "Application.h"

int main(void)
{
    Application* app = new Application();
    app->initialization(); 

    app->createAndSetupScenes();

    app->run(); 

    delete app;
    return 0;
}