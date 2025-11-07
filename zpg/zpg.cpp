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
    Application app;
    app.initialization(); 
    app.createAndSetupScenes();
    app.run(); 

    return 0;
}
