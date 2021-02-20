/* 
 * File:   World.h
 * Author: markus
 *
 * Created on 24. Januar 2021, 17:56
 */

#pragma once

#include <vector>

#include "BlackHole.h"
#include "Camera.h"
#include "CelestialBody.h"

class World {
private:
    BlackHole *blackHole;
    Camera *camera;
    std::vector<CelestialBody> celestialBodies;
    
    bool curve = true;
    bool sun = true;
    float texScale = 1.0f;
    float speedUp = 0.0f;

public:

    World() {
        // Create a black hole with our sun's mass
        // Alternatively use "C * C * RADIUS_OF_SUN / (2 * G)" in order to obtain
        // a black hole whose Schwarzschild radius matches the radius of the sun
        float mass = MASS_OF_SUN;         
        blackHole = new BlackHole(mass);
        
        // Place the camerta at Earth's distance
        camera = new Camera(glm::vec3(-AU, 0.0f, 0.0f));

        // Create some further celestial bodies if you wish...
        celestialBodies.push_back(CelestialBody(
                MASS_OF_SUN,
                RADIUS_OF_SUN,
                glm::vec3(),
                glm::vec3()
                ));

        celestialBodies.push_back(CelestialBody(
                MASS_OF_EARTH,
                RADIUS_OF_EARTH,
                glm::vec3(AU, 0.0f, 0.0f),
                glm::vec3(0.0f, 29785.0f, 0.0f)
                ));

        celestialBodies.push_back(CelestialBody(
                MASS_OF_MOON,
                RADIUS_OF_MOON,
                glm::vec3(AU + DISTANCE_EARTH_MOON, 0.0f, 0.0f),
                glm::vec3(0.0f, 29785.0f + 1000.0f, 0.0f)
                ));
    }

    ~World() {
        delete blackHole;
        delete camera;
    }

    BlackHole* getBlackHole() const {
        return blackHole;
    }

    Camera* getCamera() const {
        return camera;
    }

    void updateCelestialBodies(float duration) {
        float delta = speedUp * duration;
        for (int i = 0; i < celestialBodies.size(); i++) {
            for (int j = 0; j < i; j++) {
                glm::vec3 dist = celestialBodies[j].GetPosition() - celestialBodies[i].GetPosition();
                // Formula for gravitational force
                float force = G * celestialBodies[i].GetMass() * celestialBodies[j].GetMass() / glm::dot(dist, dist);
                // Apply force to objects
                celestialBodies[i].acc(force / celestialBodies[i].GetMass() * delta * glm::normalize(dist));
                celestialBodies[j].acc(-force / celestialBodies[j].GetMass() * delta * glm::normalize(dist));
            }
        }
        for (int i = 0; i < celestialBodies.size(); i++) {
            celestialBodies[i].move(delta);
        }
    }

    std::vector<CelestialBody> getCelestialBodies() const {
        return celestialBodies;
    }

    void setSpeedUp(float speedUp) {
        this->speedUp = speedUp;
    }
    
    bool isCurvatureOn() {
        return curve; 
    }
    
    void toggleCurveature() {
        curve = !curve; 
    }
    
    bool isSunOn() {
        return sun; 
    }
    
    void toggleSun() {
        sun = !sun; 
    }

    float getTexScale() {
        return texScale;
    }
    
    void setTexScale(float texScale) {
        this->texScale = texScale;
    }

};

/*
 * The information that is passed to the (fragment) shader
 */
struct WorldUniformBufferObject {
    // position + radius of the black hole
    glm::vec4 blackHole;
    // position + zoom factor of camera
    glm::vec4 camera;
    // view matrix
    glm::mat4 view;
    // activate curvature resulting from gravity
    bool curve;
    // draw the sun
    alignas(4) bool sun;
    // scale up star texture
    alignas(4) float texScale;
};