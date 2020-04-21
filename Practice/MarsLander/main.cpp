// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#define SVG
#ifdef SVG
    #include "SVG.h"
#endif // SVG

#include "Surface.h"
#include "Lander.h"
#include "Chromosome.h"

#include <iostream>

using namespace std;

void loadDataFromFile(const string& filename, Surface& surface, Lander& lander) {
    fstream file;

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

int main() {
    Surface surface;
    Lander lander;

    loadDataFromFile("Straight landing.in", surface, lander);

    cout << "Lander information:" << endl;
    cout << lander.position.x << " " << lander.position.y << endl;
    cout << lander.speed.x << " " << lander.speed.y << endl;
    cout << lander.fuel << " " << lander.rotate << " " << lander.power << endl;

    cout << "Surface information:" << endl;
    cout << surface.segments.size() << endl;
    for (const auto& segment : surface.segments) {
        cout << segment.p0.x << " " << segment.p0.y << endl;
    }
    cout << surface.segments.back().p1.x << " " << surface.segments.back().p1.y << endl;

#ifdef SVG
    SVGManager svg;
    svg.print(surface.constructSVGData(svg));
#endif // SVG

    return 0;
}
