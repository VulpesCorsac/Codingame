// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Game.h"

#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    Surface surface;
    Lander lander;

    loadDataFromFile("Straight landing.in", surface, lander);

    cout << "Lander information:" << endl;
    cout << lander.position.x << " " << lander.position.y << endl;
    cout << lander.speed.x << " " << lander.speed.y << endl;
    cout << lander.fuel << " " << lander.rotate << " " << lander.power << endl;

    cout << "Surface information:" << endl;
    cout << "Landing zone idx: " << surface.landing_segment_idx << endl;
    cout << surface.segments.size() << endl;
    for (size_t i = 0; i < surface.segments.size(); ++i) {
        cout << "Segment " << i << ": ";
        cout << "{" << surface.segments[i].p0.x << ", " << surface.segments[i].p0.y << "} - ";
        cout << "{" << surface.segments[i].p1.x << ", " << surface.segments[i].p1.y << "} - ";
        cout << surface.segments[i].length << " -> ";
        cout << surface.landing_zones_direction[i] << endl;
    }

    /*
    cout << fixed << setprecision(2);
    lander.power = 1;
    for (int frame = 0; frame < 2; ++frame) {
        cout << "Frame " << frame << endl;
        cout << "Position: " << lander.position.x << ", " << lander.position.y << endl;
        cout << "Speed: " << lander.speed.x << ", " << lander.speed.y << endl;
        cout << "Fuel: " << lander.fuel << ", Angle: " << lander.rotate << ", Power: " << lander.power << endl;
        lander.step(0, 0);
    }
    //*/

#ifdef SVG
    SVGManager svg;
    svg.print(surface.constructSVGData(svg));
#endif // SVG

    return 0;
}
