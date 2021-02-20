/* 
 * File:   main.cpp
 * Author: markus
 */

#include <cstdlib>
#include <iostream>
#include <glm/glm.hpp>

#include "World.h"
#include "GUI.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    World world = World();
    
    GUI* gui = new GUI(&world);
    gui->initGUI();
    gui->mainLoop();
    gui->shutdownGUI();

    return 0;
}