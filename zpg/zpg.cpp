#include "Application.h"

int main(void)
{
    Application* app = new Application();
    app->initialization(); 

    app->setupScene();

    app->run(); 

    delete app;
    return 0;
}