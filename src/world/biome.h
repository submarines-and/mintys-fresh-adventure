#pragma once
#include <glm/glm.hpp>
#include <map>
#include <vector>

enum BiomeType {
    TUNDRA,
    PLAINS,
    SWAMP,
    FOREST,
    RAIN_FOREST,
    DESERT,
};

enum TerrainType {
    DEEP_WATER,
    SHALLOW_WATER,
    SAND,
    GRASS,
    SLOPE,
    ROCK,
    MOUNTAIN,
    SNOW,
};

class Biome {
private:
    BiomeType type;
    float meshHeight, waterHeight;

    std::map<BiomeType, std::map<TerrainType, glm::vec3>> colors = {
        {TUNDRA, {
                     {DEEP_WATER, glm::vec3(60, 95, 190)},
                     {SHALLOW_WATER, glm::vec3(60, 100, 190)},
                     {SAND, glm::vec3(235, 237, 215)},
                     {GRASS, glm::vec3(255, 255, 255)},
                     {SLOPE, glm::vec3(223, 223, 223)},
                     {MOUNTAIN, glm::vec3(90, 65, 60)},
                     {ROCK, glm::vec3(75, 60, 55)},
                     {SNOW, glm::vec3(255, 255, 255)},
                 }},
        {PLAINS, {
                     {DEEP_WATER, glm::vec3(60, 95, 190)},
                     {SHALLOW_WATER, glm::vec3(60, 100, 190)},
                     {SAND, glm::vec3(210, 215, 130)},
                     {GRASS, glm::vec3(95, 165, 30)},
                     {SLOPE, glm::vec3(65, 115, 20)},
                     {MOUNTAIN, glm::vec3(90, 65, 60)},
                     {ROCK, glm::vec3(75, 60, 55)},
                     {SNOW, glm::vec3(255, 255, 255)},
                 }},
        {SWAMP, {
                    {DEEP_WATER, glm::vec3(60, 95, 190)},
                    {SHALLOW_WATER, glm::vec3(60, 100, 190)},
                    {SAND, glm::vec3(161, 176, 134)},
                    {GRASS, glm::vec3(143, 168, 50)},
                    {SLOPE, glm::vec3(95, 107, 51)},
                    {MOUNTAIN, glm::vec3(90, 65, 60)},
                    {ROCK, glm::vec3(75, 60, 55)},
                    {SNOW, glm::vec3(255, 255, 255)},
                }},
        {FOREST, {
                     {DEEP_WATER, glm::vec3(60, 95, 190)},
                     {SHALLOW_WATER, glm::vec3(60, 100, 190)},
                     {SAND, glm::vec3(155, 189, 96)},
                     {GRASS, glm::vec3(90, 138, 4)},
                     {SLOPE, glm::vec3(116, 171, 19)},
                     {MOUNTAIN, glm::vec3(90, 65, 60)},
                     {ROCK, glm::vec3(75, 60, 55)},
                     {SNOW, glm::vec3(255, 255, 255)},
                 }},
        {RAIN_FOREST, {
                          {DEEP_WATER, glm::vec3(60, 95, 190)},
                          {SHALLOW_WATER, glm::vec3(60, 100, 190)},
                          {SAND, glm::vec3(184, 204, 194)},
                          {GRASS, glm::vec3(96, 189, 143)},
                          {SLOPE, glm::vec3(15, 184, 100)},
                          {MOUNTAIN, glm::vec3(90, 65, 60)},
                          {ROCK, glm::vec3(75, 60, 55)},
                          {SNOW, glm::vec3(255, 255, 255)},
                      }},
        {DESERT, {
                     {DEEP_WATER, glm::vec3(60, 95, 190)},
                     {SHALLOW_WATER, glm::vec3(60, 100, 190)},
                     {SAND, glm::vec3(242, 229, 211)},
                     {GRASS, glm::vec3(250, 213, 165)},
                     {SLOPE, glm::vec3(140, 106, 59)},
                     {MOUNTAIN, glm::vec3(90, 65, 60)},
                     {ROCK, glm::vec3(75, 60, 55)},
                     {SNOW, glm::vec3(255, 255, 255)},
                 }},
    };

private:
    BiomeType getBiomeType(float rainfall, float temperature);

public:
    Biome(float rainfall, float temperature, float meshHeight, float waterHeight);
    glm::vec3 getColorAtPoint(float noise);
    std::vector<glm::vec3> getColorAtPoint(std::vector<float> noise);
};