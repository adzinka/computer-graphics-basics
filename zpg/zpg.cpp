#include "Application.h"

int main(void)
{
    Application* app = new Application();
    app->initialization(); 

    // Loading scene
    app->createShaders();
    app->createModels();

    // Rendering
    app->run(); 

    delete app;
    return 0;
}