#pragma once
#include <vector>
#include <GL/freeglut.h>
#include "camera.h"

struct Particle {
    Point3 position; // MissilePoint3 사용
    Vector3 velocity;
    float life; // 남은 생명 시간
    float size;
    Vector3 color;
};

class ParticleSystem {
public:
    ParticleSystem();
    void createParticles(Point3& position);
    void updateParticles(float deltaTime);
    void drawParticles();
    void drawParticlesWater();
    std::vector<Particle> particles;
    bool wet = false;

private:
    float gravity = 9.8f;
};
