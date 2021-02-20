#define GLFW_INCLUDE_VULKAN

#include "GUI.h"
#include <chrono>
#include <thread>
#include <ostream>
#include <iostream>

#include "Experiments.h"

void GUI::initGUI() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, "Black Hole", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);

    // Register event call backs.
    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int w, int h) {
        static_cast<GUI*> (glfwGetWindowUserPointer(window))->onWindowResized(w, h);
    });
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        static_cast<GUI*> (glfwGetWindowUserPointer(window))->onKeyInput(key, scancode, action, mods);
    });
    glfwSetScrollCallback(window, [](GLFWwindow* window, double x, double y) {
        static_cast<GUI*> (glfwGetWindowUserPointer(window))->onScrolled(x, y);
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
        static_cast<GUI*> (glfwGetWindowUserPointer(window))->onMouseMoved(x, y);
    });

    // This enables fullscreen
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);

    std::cout << "GLFW started.\n";

    vulkanInstance = new VulkanInstance(window);
    vulkanInstance->initVulkan();
}

void GUI::shutdownGUI() {
    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "GLFW terminated.\n";
}

void printFPS() {
    static auto oldTime = std::chrono::high_resolution_clock::now();
    static int fps;
    fps++;

    if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - oldTime) >= std::chrono::seconds{1}) {
        oldTime = std::chrono::high_resolution_clock::now();
        std::cout << "FPS: " << fps << std::endl;
        fps = 0;
    }
}

void GUI::mainLoop() {
    glfwGetCursorPos(window, &cursor_x, &cursor_y);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        updateMVP();
        drawFrame();
        //        printFPS();
    }
}

void GUI::updateMVP() {
    auto frameTime = std::chrono::high_resolution_clock::now();
    float timeSinceLastFrame = std::chrono::duration_cast<std::chrono::milliseconds>(frameTime - lastFrameTime).count() / 1000.0f;
    lastFrameTime = frameTime;
    world->getCamera()->updatePosition(timeSinceLastFrame);
    world->updateCelestialBodies(timeSinceLastFrame);
}

void GUI::drawFrame() {
    vulkanInstance->drawFrame(world);
}

void GUI::onWindowResized(int w, int h) {
    if (w <= 0 || h <= 0) return; // Do nothing
    // TODO recreate swap chain
}

void GUI::onScrolled(double xoffset, double yoffset) {
    world->getCamera()->zoomByFactor(powf(1.1f, yoffset));
}

void GUI::onMouseMoved(double xpos, double ypos) {
    float factor = 0.01f / world->getCamera()->getZoom();
    world->getCamera()->spin(factor * (xpos - cursor_x));
    world->getCamera()->pan(factor * (ypos - cursor_y));
    cursor_x = xpos;
    cursor_y = ypos;
}

void GUI::onKeyInput(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        world->getCamera()->accelerateForeward(default_speed);
    } else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        world->getCamera()->accelerateSideward(-default_speed);
    } else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        world->getCamera()->accelerateForeward(-default_speed);
    } else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        world->getCamera()->accelerateSideward(default_speed);
    } else if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        world->getCamera()->accelerateUpward(-default_speed);
    } else if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        world->getCamera()->accelerateUpward(default_speed);
    } else if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
        world->getCamera()->accelerateForeward(-default_speed);
    } else if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
        world->getCamera()->accelerateSideward(default_speed);
    } else if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
        world->getCamera()->accelerateForeward(default_speed);
    } else if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
        world->getCamera()->accelerateSideward(-default_speed);
    } else if (key == GLFW_KEY_Q && action == GLFW_RELEASE) {
        world->getCamera()->accelerateUpward(default_speed);
    } else if (key == GLFW_KEY_E && action == GLFW_RELEASE) {
        world->getCamera()->accelerateUpward(-default_speed);
    }
    
    else if (key == GLFW_KEY_1 && action == GLFW_RELEASE) {
        world->toggleCurveature();
    } else if (key == GLFW_KEY_2 && action == GLFW_RELEASE) {
        world->toggleSun();
    } else if (key == GLFW_KEY_3 && action == GLFW_RELEASE) {
        world->getCamera()->lookAt(world->getBlackHole()->getPosition());
    }// Start the simulation
    else if (key == GLFW_KEY_4 && action == GLFW_RELEASE) {
        zoomInFromEarth(world);
    } else if (key == GLFW_KEY_5 && action == GLFW_RELEASE) {
        moveAlongLargeBlackHole(world);
    } else if (key == GLFW_KEY_6 && action == GLFW_RELEASE) {
        visualizeSunsLensEffect(world);
    }
}