#pragma once

#include "Lander.h"

#include <algorithm>

class Gene {
public:
    Gene() {

    }

    Gene(int rotate, int power) noexcept
        : rotate(rotate)
        , power(power) {
        fixGene();
    }

    void fixGene() noexcept {
        rotate = std::max(ANGLE_MIN, std::min(rotate, ANGLE_MAX));
        power  = std::max(POWER_MIN, std::min(power , POWER_MAX));
    }

public:
    int rotate = ANGLE_INVALID;
    int power  = POWER_INVALID;
};

