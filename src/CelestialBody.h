#include <glm/glm.hpp>
#include <ostream>

class CelestialBody {
private:
    float mass;
    float radius;
    glm::vec3 velocity;
    glm::vec3 position;

public:

    CelestialBody(float mass, float radius, glm::vec3 position, glm::vec3 velocity)
    : mass(mass), radius(radius), position(position), velocity(velocity) {
    }
    
    float GetMass() const {
        return mass;
    }

    float GetRadius() const {
        return radius;
    }

    glm::vec3 GetPosition() const {
        return position;
    }

    glm::vec3 GetVelocity() const {
        return velocity;
    }

    void acc(glm::vec3 delta) {
        velocity += delta;
    }
    
    void move(float duration) {
        position += duration * velocity;
    }

};