/* 
 * File:   BlackHole.h
 * Author: markus
 */

#pragma once

#include <glm/glm.hpp>

#include "Constants.h"


class BlackHole {
private:
    float mass;
    glm::vec3 position;

public:

    BlackHole(double mass=SOLAR_MASS, glm::vec3 position=glm::vec3())
    : mass(mass), position(position) {
    }
    
    ~BlackHole() {
    }

    void setMass(float mass) {
        this->mass = mass;
    }
    
    float getMass() const {
        return mass;
    }

    glm::vec3 getPosition() const {
        return position;
    }
    
    float GetSchwarzschildRadius() {
        return 2 * G * mass / (C * C);
    }


};