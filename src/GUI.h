/* 
 * File:   GUI.h
 * Author: markus
 *
 * Created on 25. Januar 2021, 17:04
 */

#pragma once

#include <chrono>
#include <GLFW/glfw3.h>

#include "World.h"
#include "VulkanInstance.h"

class GUI {
private:
    uint32_t width = 1920;
    uint32_t height = 1200;
    GLFWwindow* window = NULL;

    double cursor_x;
    double cursor_y;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameTime = std::chrono::high_resolution_clock::now();

    World* world;
    VulkanInstance* vulkanInstance;
    const float default_speed = 10 * C;

    void updateMVP();
    void drawFrame();

public:

    GUI(World* world) : world(world) {
    }

    ~GUI() {
        delete vulkanInstance;
        delete world;
    }

    void initGUI();
    void shutdownGUI();
    void mainLoop();

    void onWindowResized(int w, int h);
    void onScrolled(double xoffset, double yoffset);
    void onMouseMoved(double xpos, double ypos);
    void onKeyInput(int key, int scancode, int action, int mods);

    GLFWwindow* getWindow() {
        return window;
    }

};