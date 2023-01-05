#pragma once

class Math {
public:
    // Normalize to range (default 0-1)
    static float normalize(float value, float max, float min = 0, float rangeMax = 1)
    {
        return (value - min) / (max - min) * rangeMax;
    }
};