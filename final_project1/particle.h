#pragma once
#include <vector>
#include <GL/freeglut.h>
#include "camera.h"

struct Particle {
    Point3 position; // MissilePoint3 사용
    Vector3 velocity;
    float life; // 남은 생명 시간
};

class ParticleSystem {
public:
    ParticleSystem();
    void createExplosion(Point3& position);
    void updateParticles(float deltaTime);
    void drawParticles();
    std::vector<Particle> particles;

private:
};
