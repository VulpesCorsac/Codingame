#pragma once

#include <algorithm>

constexpr float PI = 3.14159265f;

constexpr int POWER_INVALID  = -1;
constexpr int POWER_MIN      = 0;
constexpr int POWER_MAX      = 4;
constexpr int POWER_STEP_MIN = -1;
constexpr int POWER_STEP_MAX = +1;
constexpr int ANGLE_INVALID  = 100;
constexpr int ANGLE_MIN      = -90;
constexpr int ANGLE_MAX      = +90;
constexpr int ANGLE_STEP_MIN = -15;
constexpr int ANGLE_STEP_MAX = +15;

constexpr int ANGLE_RIGHT = 90;

constexpr float MARS_GRAVITY = 3.711f;

constexpr float MAX_SPEED_TO_LAND_HORIZONTAL = 20;
constexpr float MAX_SPEED_TO_LAND_VERTICAL   = 40;

class Lander {
public:
    Lander() noexcept {}

    void changeRotateAngle(int d_angle) noexcept {
        d_angle = std::min(std::max(d_angle, ANGLE_STEP_MIN), ANGLE_STEP_MAX);

        rotate += d_angle;

        rotate = std::min(std::max(ANGLE_MIN, rotate), ANGLE_MAX);
    }

    void changePower(int d_power) noexcept {
        d_power = std::min(std::max(d_power, POWER_STEP_MIN), POWER_STEP_MAX);

        power += d_power;

        power = std::min(std::max(POWER_MIN, power), POWER_MAX);
    }

    void simulate(int d_angle, int d_power) noexcept {
        changeRotateAngle(d_angle);
        changePower(d_power);

        fuel -= power;

        int theta = rotate + ANGLE_RIGHT;
        float rad = theta * PI * 0.5 / ANGLE_RIGHT;

        Point forse_composition(cos(rad), sin(rad));
        Point acceleration = forse_composition * Point{power, power} - Point{0, MARS_GRAVITY};

        position += speed + acceleration * Point{0.5, 0.5};
        speed += acceleration;
    }

    std::vector < std::string > getTitleLines() const noexcept {
        std::vector < std::string > titleLines;

        std::string positionLine = "Shuttle position: (";
        positionLine.append(std::to_string(static_cast < int >(position.x)));
        positionLine.append(", ");
        positionLine.append(std::to_string(static_cast < int >(position.y)));
        positionLine.append(")");
        titleLines.push_back(positionLine);

        std::string rotationLine = "Rotation: ";
        rotationLine.append(std::to_string(rotate));
        titleLines.push_back(rotationLine);

        std::string powerLine = "Power: ";
        powerLine.append(std::to_string(power));
        titleLines.push_back(powerLine);

        std::string hSpeedLine = "HSpeed: ";
        hSpeedLine.append(std::to_string(static_cast < int >(speed.x)));
        titleLines.push_back(hSpeedLine);

        std::string vSpeedLine = "VSpeed: ";
        vSpeedLine.append(std::to_string(static_cast < int >(speed.y)));
        titleLines.push_back(vSpeedLine);

        return titleLines;
    }

public:
    Point position;
    Point speed;

    int fuel   = 500;
    int rotate =   0;
    int power  =   0;
};

