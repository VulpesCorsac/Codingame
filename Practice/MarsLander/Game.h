#pragma once

#define SVG
#ifdef SVG
    #include "SVG.h"
#endif // SVG

#include "Surface.h"
#include "Lander.h"
#include "Chromosome.h"

#include <iostream>

class Game {

};

void loadDataFromFile(const std::string& filename, Surface& surface, Lander& lander) {
    std::fstream file;

    file.open(filename.c_str(), std::fstream::in);
    if (file.is_open()) {
        int N;
        file >> N;
        Point a, b;
        file >> a.x >> a.y;
        for (int i = 1; i < N; ++i) {
            file >> b.x >> b.y;
            surface.addSegment(a, b);
            a = b;
        }
        surface.findLandingSegment();

        file >> lander.position.x >> lander.position.y;
        file >> lander.speed.x >> lander.speed.y;
        file >> lander.fuel >> lander.rotate >> lander.power;

        file.close();
    } else {
        throw std::runtime_error("Cannot load surface from file");
    }
}
