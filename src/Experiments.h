#include "World.h"

void zoomInFromEarth(World* world) {
    if (!world->isCurvatureOn()) {
        world->toggleCurveature();
    }
    if (world->isSunOn()) {
        world->toggleSun();
    }
    world->setTexScale(1.0f);
    world->getBlackHole()->setMass(MASS_OF_SUN);
    world->getCamera()->stop();
    world->getCamera()->setPosition(glm::vec3(-AU, 0.0f, 0.0f));
    world->getCamera()->lookAt(world->getBlackHole()->getPosition());
    world->getCamera()->setZoom(1.0f);
    world->getCamera()->setContinuousZoom(2.0f);
}

void moveAlongLargeBlackHole(World* world) {
    if (!world->isCurvatureOn()) {
        world->toggleCurveature();
    }
    if (world->isSunOn()) {
        world->toggleSun();
    }
    world->setTexScale(1.0f);
    world->getBlackHole()->setMass(C * C * RADIUS_OF_SUN / (2 * G));
    world->getCamera()->stop();
    world->getCamera()->setZoom(3.0f);
    world->getCamera()->setContinuousZoom(1.0f);
    world->getCamera()->setPosition(glm::vec3(-AU, 15 * C, 0.0f));
    world->getCamera()->lookAt(world->getBlackHole()->getPosition());
    world->getCamera()->accelerateSideward(C);
}

void visualizeSunsLensEffect(World* world) {
    if (!world->isSunOn()) {
        world->toggleSun();
    }
    world->setTexScale(200.0f);
    world->getBlackHole()->setMass(MASS_OF_SUN);
    world->getCamera()->stop();
    world->getCamera()->setPosition(glm::vec3(0.0f, AU, 0.0f));
    float x = sqrt(0.5) * RADIUS_OF_SUN;
    world->getCamera()->lookAt(glm::vec3(x, 0.0f, x));
    world->getCamera()->setZoom(2500.0f);
    world->getCamera()->setContinuousZoom(1.0f);
    
    // Toggle everytime this function is called to make the movement visible
    world->toggleCurveature();
}