#pragma once
#include <glm/glm.hpp>
#include <random>

class Math {
public:
    static float normalize(float input, float min, float max)
    {
        return (input - min) / (max - min);
    }

    static float normalize(float input, float min, float max, float range)
    {
        return normalize(input, min, max) * range;
    }

    static int random(int min, int max)
    {
        return rand() % (max - min + 1) + min;
    }

    static float random(float min, float max)
    {
        return (min + 1) + (((float)rand()) / (float)RAND_MAX) * (max - (min + 1));
    }

    static float baryCentric(glm::vec3 v1, glm::vec3 p2, glm::vec3 v3, glm::vec2 position)
    {
        float det = (p2.z - v3.z) * (v1.x - v3.x) + (v3.x - p2.x) * (v1.z - v3.z);
        float l1 = ((p2.z - v3.z) * (position.x - v3.x) + (v3.x - p2.x) * (position.y - v3.z)) / det;
        float l2 = ((v3.z - v1.z) * (position.x - v3.x) + (v1.x - v3.x) * (position.y - v3.z)) / det;
        float l3 = 1.0f - l1 - l2;

        return l1 * v1.y + l2 * p2.y + l3 * v3.y;
    }
};