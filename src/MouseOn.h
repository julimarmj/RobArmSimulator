#pragma once
#include "GLM/glm.hpp"

class Select {
	glm::mat4* model;
	glm::mat4* proj;
	glm::mat4* view;

	Select(glm::mat4* m, glm::mat4* v, glm::mat4* p);
	~Select();

	void init();
	void Render();
	glm::vec3 ClosestPoint(const glm::vec3 A, const glm::vec3 B, const glm::vec3 P, double* t);
	bool RayTest(const glm::vec3, const glm::vec3 start, const glm::vec3 end, glm::vec3* pt, double* t, double epsilon);
	bool RayTestPoints(const glm::vec3& start, const glm::vec3& end, unsigned int* id, double* t, double epsilon);
	void ProcessMouse(int button, int state, int x, int y);
};