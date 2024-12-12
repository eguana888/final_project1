#include "particle.h"
#include <stdlib.h> // rand()
#include <iostream>

ParticleSystem::ParticleSystem() {}

void ParticleSystem::createExplosion(Point3& position) {
	particles.clear(); // ���� ��ƼŬ�� �����ϰ� ���� �߰�
	// ���� ���� ��ƼŬ ����
	for (int i = 0; i < 10; ++i) {
		Particle p;
		p.position = Point3(position.x, position.y - 10.0f, position.z);
		p.velocity = Vector3(
			((rand() % 100) / 25.0f - 2.0f), // ������ x �ӵ� (-2.0 ~ 2.0)
			((rand() % 100) / 25.0f),        // ������ y �ӵ� (0.0 ~ 4.0)
			((rand() % 100) / 25.0f - 2.0f)  // ������ z �ӵ� (-2.0 ~ 2.0)
		);
		p.life = 5.0f; // ���� �ð� �ʱ�ȭ (1��)
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
	// ���� �ð��� ���� ��ƼŬ ����
	size_t beforeErase = particles.size();
	particles.erase(std::remove_if(particles.begin(), particles.end(),
		[](const Particle& p) { return p.life <= 0.0f; }),
		particles.end());
	size_t afterErase = particles.size();
}

void ParticleSystem::drawParticles() {
	glPushMatrix();
	glColor3f(1.0f, 0.5f, 0.0f); // ���� ��ƼŬ ���� (��Ȳ��)
	for (const auto& p : particles) {

		glPushMatrix();
		glTranslatef(p.position.x, p.position.y, p.position.z);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4f(1.0f, 0.5f, 0.0f, 0.5f); // ��Ȳ��, 50% ����
		glutSolidSphere(2.0, 20, 20);

		glDisable(GL_BLEND);
		glPopMatrix();
	}
	glPopMatrix();
}