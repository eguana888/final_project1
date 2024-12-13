#include "particle.h"
#include <stdlib.h> // rand()
#include <iostream>

ParticleSystem::ParticleSystem() {}

void ParticleSystem::createParticles(Point3& position) {
	particles.clear(); // 기존 파티클 제거
	for (int i = 0; i < 50; ++i) {
		Particle p;
		p.position = Point3(position.x, position.y, position.z);

		// 랜덤 방향 설정
		float angle1 = ((rand() % 180) / 180.0f * 3.14159f); // 0 ~ 180도 랜덤 각도
		float angle2 = ((rand() % 180) / 180.0f * 3.14159f);
		p.velocity = Vector3(
			cos(angle1) * sin(angle2) * ((rand() % 100) / 50.0f + 1.0f), 
			cos(angle2) * ((rand() % 100) / 50.0f + 2.0f),               
			sin(angle1) * sin(angle2) * ((rand() % 100) / 50.0f + 1.0f) 
		);

		p.life = (rand() % 50) / 10.0f + 5.0f; // 5초 ~ 10초 사이의 랜덤 수명
		p.size = (rand() % 10) / 10.0f + 1.0f; // 1.0 ~ 2.0 사이의 크기
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

		// 중력 효과
		p.velocity.y -= deltaTime * 0.2f;

		// 공기 저항 효과
		p.velocity.x *= 0.99f;
		p.velocity.y *= 0.99f;
		p.velocity.z *= 0.99f;

		// 파티클 수명 감소
		p.life -= deltaTime * 0.1f; 
		p.size *= 0.99f; // 파티클 크기를 감소
	}

	// 수명이 끝난 파티클 제거
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

		// 불꽃 색상 설정
		float lifeRatio = p.life / 10.0f; // 0 ~ 1 비율
		float alpha = std::max(0.0f, lifeRatio);
		float red = 1.0f * lifeRatio + 0.5f * (1.0f - lifeRatio);
		float green = 0.5f * lifeRatio;
		float blue = 0.0f; // 불꽃 색상

		glColor4f(red, green, blue, alpha);

		// 파티클 크기 설정
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

		// 물에서의 파란색 계열
		float lifeRatio = p.life / 10.0f; // 0 ~ 1 비율
		float alpha = std::max(0.0f, lifeRatio);
		glColor4f(0.0f, 0.5f, 1.0f, alpha); // 파란색 계열

		// 파티클 크기 설정
		float size = p.size * lifeRatio;
		glutSolidSphere(size, 10, 10);

		glPopMatrix();
	}

	glDisable(GL_BLEND);
	glPopMatrix();
}
