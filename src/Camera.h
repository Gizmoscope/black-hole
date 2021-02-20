/* 
 * File:   Camera.h
 * Author: markus
 *
 * Created on 24. Januar 2021, 17:53
 */

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <ostream>

#include "Constants.h"

class Camera {
private:
    glm::vec3 position;
    float spin_angle;
    float pan_angle;

    float foreward_speed = 0.0f;
    float sideward_speed = 0.0f;
    float upward_speed = 0.0f;
    
    float continuousZoom = 1.0f; // zoom per second

    float zoom = 1.0f;

public:

    Camera(glm::vec3 position)
    : position(position), spin_angle(0.0), pan_angle(0.0f) {
    }

    ~Camera() {
    }

    void spin(float angle) {
        spin_angle += angle;
    }

    void pan(float angle) {
        pan_angle += angle;
        if(pan_angle < 0.01) {
            pan_angle = 0.01;
        } else if(pan_angle > 3.14) {
            pan_angle = 3.14;
        }
    }
    
    void accelerateForeward(float amount) {
        foreward_speed += amount;
    }

    void accelerateSideward(float amount) {
        sideward_speed += amount;
    }

    void accelerateUpward(float amount) {
        upward_speed += amount;
    }
    
    void setPosition(glm::vec3 position) {
        this->position = position;
    }
    
    void setContinuousZoom(float factor) {
        this->continuousZoom = factor;
    }

    glm::vec3 getPosition() {
        return position;
    }

    void stop() {
        foreward_speed = sideward_speed = upward_speed = 0.0f;
    }
    
    void lookAt(glm::vec3 object) {
        glm::vec3 direction = glm::normalize(position - object);
        this->spin_angle = -atan2(direction.x, direction.y);
        this->pan_angle = acos(direction.z);
    }
    
    void zoomByFactor(float factor) {
        zoom *= factor;
    }
    
    float setZoom(float zoom) {
        this->zoom = zoom;
    }
    
    float getZoom() {
        // use "distance_from_object / constant_factor" to achieve a vertigo effect
        return zoom; 
    }
    
    glm::mat4 getViewMatrix(float aspectRatio) {
        glm::mat4 view = glm::rotate(glm::mat4(1.0f), pan_angle, glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::rotate(glm::mat4(1.0f), spin_angle, glm::vec3(0.0f, 0.0f, 1.0f)) * view;
        view = view * glm::scale(glm::mat4(1.0f), glm::vec3(aspectRatio, -1.0f, 1.0f));
        return view;
    }
    
    void updatePosition(float duration) {
        // Apply camera orientation to the speed vector
        glm::vec4 d = getViewMatrix(1.0f) * glm::vec4(sideward_speed, -upward_speed, -foreward_speed, 0.0f);
        position += glm::vec3(d.x, d.y, d.z) * duration;
        zoom *= pow(continuousZoom, duration);
    }

};