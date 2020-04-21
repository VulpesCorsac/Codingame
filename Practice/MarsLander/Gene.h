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
        rotate = std::max(Lander::ANGLE_MIN, std::min(rotate, Lander::ANGLE_MAX));
        power  = std::max(Lander::POWER_MIN, std::min(power , Lander::POWER_MAX));
    }

public:
    int rotate = Lander::ANGLE_INVALID;
    int power  = Lander::POWER_INVALID;
};

