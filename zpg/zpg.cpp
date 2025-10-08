/**
 * @file zpg.cpp
 *
 * @brief Main function
 *
 * @author Yelizaveta Zhurava ZHU0020
  **/
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