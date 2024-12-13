#include "particle.h"
#include <stdlib.h> // rand()
#include <iostream>

ParticleSystem::ParticleSystem() {}

void ParticleSystem::createParticles(Point3& position) {
	particles.clear(); // ���� ��ƼŬ ����
	for (int i = 0; i < 50; ++i) {
		Particle p;
		p.position = Point3(position.x, position.y, position.z);

		// ���� ���� ����
		float angle1 = ((rand() % 180) / 180.0f * 3.14159f); // 0 ~ 180�� ���� ����
		float angle2 = ((rand() % 180) / 180.0f * 3.14159f);
		p.velocity = Vector3(
			cos(angle1) * sin(angle2) * ((rand() % 100) / 50.0f + 1.0f), 
			cos(angle2) * ((rand() % 100) / 50.0f + 2.0f),               
			sin(angle1) * sin(angle2) * ((rand() % 100) / 50.0f + 1.0f) 
		);

		p.life = (rand() % 50) / 10.0f + 5.0f; // 5�� ~ 10�� ������ ���� ����
		p.size = (rand() % 10) / 10.0f + 1.0f; // 1.0 ~ 2.0 ������ ũ��
		p.color = Vector3(
			1.0f,
			(rand() % 50) / 100.0f + 0.5f, 
			0.0f
		);
		particles.push_back(p);
	}
}

void ParticleSystem::updateParticles(float deltaTime) {
	for (auto& p : particles) {
		p.position.x += p.velocity.x * deltaTime;
		p.position.y += p.velocity.y * deltaTime;
		p.position.z += p.velocity.z * deltaTime;

		// �߷� ȿ��
		p.velocity.y -= deltaTime * 0.2f;

		// ���� ���� ȿ��
		p.velocity.x *= 0.99f;
		p.velocity.y *= 0.99f;
		p.velocity.z *= 0.99f;

		// ��ƼŬ ���� ����
		p.life -= deltaTime * 0.1f; 
		p.size *= 0.99f; // ��ƼŬ ũ�⸦ ����
	}

	// ������ ���� ��ƼŬ ����
	particles.erase(std::remove_if(particles.begin(), particles.end(),
		[](const Particle& p) { return p.life <= 0.0f; }),
		particles.end());
}

void ParticleSystem::drawParticles() {
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (auto& p : particles) {
		glPushMatrix();
		glTranslatef(p.position.x, p.position.y, p.position.z);

		// �Ҳ� ���� ����
		float lifeRatio = p.life / 10.0f; // 0 ~ 1 ����
		float alpha = std::max(0.0f, lifeRatio);
		float red = 1.0f * lifeRatio + 0.5f * (1.0f - lifeRatio);
		float green = 0.5f * lifeRatio;
		float blue = 0.0f; // �Ҳ� ����

		glColor4f(red, green, blue, alpha);

		// ��ƼŬ ũ�� ����
		float size = p.size * lifeRatio;
		glutSolidSphere(size, 10, 10);

		glPopMatrix();
	}

	glDisable(GL_BLEND);
	glPopMatrix();
}
void ParticleSystem::drawParticlesWater() {
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (auto& p : particles) {
		glPushMatrix();
		glTranslatef(p.position.x, p.position.y, p.position.z);

		// �������� �Ķ��� �迭
		float lifeRatio = p.life / 10.0f; // 0 ~ 1 ����
		float alpha = std::max(0.0f, lifeRatio);
		glColor4f(0.0f, 0.5f, 1.0f, alpha); // �Ķ��� �迭

		// ��ƼŬ ũ�� ����
		float size = p.size * lifeRatio;
		glutSolidSphere(size, 10, 10);

		glPopMatrix();
	}

	glDisable(GL_BLEND);
	glPopMatrix();
}
