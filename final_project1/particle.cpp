#include "particle.h"
#include <stdlib.h> // rand()
#include <iostream>

ParticleSystem::ParticleSystem() {}

void ParticleSystem::createExplosion(Point3& position) {
	particles.clear(); // 기존 파티클을 제거하고 새로 추가
	// 여러 개의 파티클 생성
	for (int i = 0; i < 10; ++i) {
		Particle p;
		p.position = Point3(position.x, position.y - 10.0f, position.z);
		p.velocity = Vector3(
			((rand() % 100) / 25.0f - 2.0f), // 랜덤한 x 속도 (-2.0 ~ 2.0)
			((rand() % 100) / 25.0f),        // 랜덤한 y 속도 (0.0 ~ 4.0)
			((rand() % 100) / 25.0f - 2.0f)  // 랜덤한 z 속도 (-2.0 ~ 2.0)
		);
		p.life = 5.0f; // 생명 시간 초기화 (1초)
		particles.push_back(p);
	}
}

void ParticleSystem::updateParticles(float deltaTime) {
	for (auto& p : particles) {
		p.position.x += p.velocity.x * deltaTime;
		p.position.y += p.velocity.y * deltaTime;
		p.position.z += p.velocity.z * deltaTime;
		p.life -= deltaTime;

	}
	// 생명 시간이 끝난 파티클 제거
	size_t beforeErase = particles.size();
	particles.erase(std::remove_if(particles.begin(), particles.end(),
		[](const Particle& p) { return p.life <= 0.0f; }),
		particles.end());
	size_t afterErase = particles.size();
}

void ParticleSystem::drawParticles() {
	glPushMatrix();
	glColor3f(1.0f, 0.5f, 0.0f); // 폭발 파티클 색상 (주황색)
	for (const auto& p : particles) {

		glPushMatrix();
		glTranslatef(p.position.x, p.position.y, p.position.z);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4f(1.0f, 0.5f, 0.0f, 0.5f); // 주황색, 50% 투명도
		glutSolidSphere(2.0, 20, 20);

		glDisable(GL_BLEND);
		glPopMatrix();
	}
	glPopMatrix();
}