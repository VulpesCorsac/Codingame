#pragma once

#include "Gene.h"
#include "Point.h"
#include "Lander.h"
#include "Map_Constants.h"
#include "Rnd.h"

#ifdef SVG
    #include "SVG.h"
    #include <iostream>
#endif // SVG

#include <algorithm>
#include <vector>

typedef std::vector < Point > Trajectory;

constexpr unsigned int CRASHED_IDX_MASK    = 0b1111'1000'0000'0000'0000'0000'0000'0000;
constexpr int CRASHED_IDX_MASK_OFFSET      = 27;
constexpr int FLAG_SELECTED                = 1 << 0; // 1
constexpr int FLAG_SOLUTION                = 1 << 1; // 2
constexpr int FLAG_CRASHED_ON_LANDING_ZONE = 1 << 2; // 4
constexpr int FLAG_CRASHED                 = 1 << 4; // 8
constexpr int FLAG_COPIED                  = 1 << 5; // 16

constexpr float SCORE_MIN_CRASHED_OUT_OF_LANDING_ZONE               =   0;
constexpr float SCORE_MAX_CRASHED_OUT_OF_LANDING_ZONE               = 100;
constexpr float SCORE_MIN_CRASHED_IN_LANDING_ZONE                   = 100;
constexpr float SCORE_MAX_CRASHED_IN_LANDING_ZONE                   = 200;
constexpr float SCORE_MIN_LANDED_IN_LANDING_ZONE                    = 300;
constexpr float SCORE_MAX_LANDED_IN_LANDING_ZONE                    = 300;
constexpr float SCORE_IN_LANDING_ZONE_SPEED_PENALTY_MULTIPLIER      = 0.5;
constexpr float SCORE_OUT_OF_LANDING_ZONE_SPEED_PENALTY_MULTIPLIER  = 0.1;
constexpr float SCORE_OUT_OF_LANDING_ZONE_SPEED_PENALTY_SHIFT       = 100;

class Chromosome {
public:
    void init(const Lander& lander) noexcept {
        initial_lander = lander;
        reset();
    }

    void reset() noexcept {
        current_lander = initial_lander;

        resetFlag();
    }

    void setFlag(int flag) noexcept {
        flags |= flag;
    }

    void unsetFlag(int flag) noexcept {
        flags &= ~flag;
    }

    bool hasFlag(int flag) const noexcept {
        return flags & flag;
    }

    void resetFlag() noexcept {
        flags = 0;
    }

    void addCrashSegmentIdxToFlags(int idx) noexcept {
        idx <<= CRASHED_IDX_MASK_OFFSET;
        flags |= idx;
    }

    int getCrashedLineIdx() const noexcept {
        unsigned int idx = flags & CRASHED_IDX_MASK;
        idx >>= CRASHED_IDX_MASK_OFFSET;

        return idx;
    }

    void normalizeScore(float sum) noexcept {
        normalized_score = score / sum;
    }

    void insertGene(int idx, const Gene& gene) noexcept {
        genes[idx] = gene;
    }

    const Gene& operator [] (int idx) const noexcept {
        return genes[idx];
    }

    Gene& operator [] (int idx) noexcept {
        return genes[idx];
    }

    float evaluate(const Surface& surface) {
        if (current_lander.position.x < 0 || current_lander.position.x >= MAP_WIDTH ||
            current_lander.position.y < 0 || current_lander.position.y >= MAP_HEIGHT) {
            score = normalized_score = 0;
            return 0;
        }

        if (!hasFlag(FLAG_CRASHED_ON_LANDING_ZONE)) {
            auto distance = surface.distanceToLandingZone(current_lander.position, getCrashedLineIdx());

            score = SCORE_MIN_CRASHED_OUT_OF_LANDING_ZONE + (SCORE_MAX_CRASHED_OUT_OF_LANDING_ZONE - SCORE_MIN_CRASHED_OUT_OF_LANDING_ZONE) * (100 * distance / MAP_MAX_DISTANCE);

            auto speed_penalty = SCORE_OUT_OF_LANDING_ZONE_SPEED_PENALTY_MULTIPLIER * std::max(current_lander.speed.length() - SCORE_OUT_OF_LANDING_ZONE_SPEED_PENALTY_SHIFT, static_cast < float >(0));

            score -= speed_penalty;
        } else {
            auto x_speed_penalty = abs(current_lander.speed.x) > MAX_SPEED_TO_LAND_HORIZONTAL ? (abs(current_lander.speed.x) - MAX_SPEED_TO_LAND_HORIZONTAL) * SCORE_IN_LANDING_ZONE_SPEED_PENALTY_MULTIPLIER : static_cast < float >(0);
            auto y_speed_penalty = abs(current_lander.speed.y) > MAX_SPEED_TO_LAND_VERTICAL   ? (abs(current_lander.speed.y) - MAX_SPEED_TO_LAND_VERTICAL  ) * SCORE_IN_LANDING_ZONE_SPEED_PENALTY_MULTIPLIER : static_cast < float >(0);

            score = std::max(SCORE_MAX_CRASHED_IN_LANDING_ZONE - x_speed_penalty - y_speed_penalty, SCORE_MIN_CRASHED_IN_LANDING_ZONE);
        }

        return score;
    }

    void simulate(const Surface& surface) noexcept {
        #ifdef SVG
            trajectory.clear();
        #endif // SVG

        Lander previous_lander = current_lander;

        last_useful_gene = 0;
        for (const auto& gene : genes) {
            current_lander.simulate(gene.rotate, gene.power);

            #ifdef SVG
                trajectory.push_back(current_lander.position);
            #endif // SVG

            auto crashed_segment_idx = surface.collide(previous_lander.position, current_lander.position);

            if (crashed_segment_idx != SEGMENT_ID_INVALID) {
                setFlag(FLAG_CRASHED);
                addCrashSegmentIdxToFlags(crashed_segment_idx);

                if (crashed_segment_idx == surface.landing_segment_idx) {
                    setFlag(FLAG_CRASHED_ON_LANDING_ZONE);

                    if (canLand(previous_lander, current_lander)) {
                        setFlag(FLAG_SOLUTION);
                        #ifdef SVG
                            for (const auto& item : trajectory) {
                                std::cout << '(' << item.x << ',' << item.y << "),";
                            }
                            std::cout << std::endl;
                        #endif // SVG
                    }
                }

                break;
            }

            ++last_useful_gene;
            previous_lander = current_lander;
        }
    }

    bool canLand(const Lander& before_crash, const Lander& after_crash) const  noexcept {
        bool valid_horizontal_speed_before_crash = abs(before_crash.speed.x) < MAX_SPEED_TO_LAND_HORIZONTAL;
        bool   valid_vertical_speed_before_crash = abs(before_crash.speed.y) < MAX_SPEED_TO_LAND_VERTICAL;
        bool           valid_rotate_before_crash = abs(before_crash.rotate) == 0;

        bool valid_horizontal_speed_after_crash = abs(after_crash.speed.x) < MAX_SPEED_TO_LAND_HORIZONTAL;
        bool   valid_vertical_speed_after_crash = abs(after_crash.speed.y) < MAX_SPEED_TO_LAND_VERTICAL;

        return valid_horizontal_speed_before_crash && valid_vertical_speed_before_crash && valid_rotate_before_crash && valid_horizontal_speed_after_crash && valid_vertical_speed_after_crash;
    }

    bool operator < (const Chromosome& other) const noexcept {
        return score < other.score;
    }

#ifdef SVG
    std::string constructSVGData(const SVGManager& svg) const noexcept {
        std::string svgStr = "";

        svgStr.append(GROUP_BEGIN);
        svgStr.append(NEW_LINE);

        std::vector < std::string > titleLines = current_lander.getTitleLines();
        std::string evaluationLine = "Evaluation: ";
        evaluationLine.append(std::to_string(score));
        titleLines.push_back(evaluationLine);
        std::string title = svg.constructMultiLineTitle(titleLines);
        svgStr.append(title);
        svgStr.append(NEW_LINE);
        svgStr.append(NEW_LINE);

        svgStr.append(POLYLINE_BEGIN);

        for (size_t positionIdx = 0; positionIdx < trajectory.size() - 1; ++positionIdx) {
            Point position = trajectory[positionIdx];
            Point nextPosition = trajectory[positionIdx + 1];

            float startX = position.x;
            float startY = MAP_HEIGHT - position.y;
            float endX = nextPosition.x;
            float endY = MAP_HEIGHT - nextPosition.y;

            svgStr.append(std::to_string(startX));
            svgStr.append(",");
            svgStr.append(std::to_string(startY));
            svgStr.append(" ");
            svgStr.append(std::to_string(endX));
            svgStr.append(",");
            svgStr.append(std::to_string(endY));
            svgStr.append(" ");
        }
        svgStr.append("\" ");

        svgStr.append(STYLE_BEGIN);
        svgStr.append(FILL_NONE);

        std::string strokeRGB = svg.constructStrokeForRGB(255, 255, 255);
        if (hasFlag(FLAG_SELECTED)) {
            strokeRGB = svg.constructStrokeForRGB(0, 0, 255);
        } else if (hasFlag(FLAG_SOLUTION)) {
            strokeRGB = svg.constructStrokeForRGB(0, 255, 0);
        }

        svgStr.append(strokeRGB);
        svgStr.append(";");

        std::string strokeWidth = svg.constructStrokeWidth(10);
        svgStr.append(strokeWidth);
        svgStr.append(ATTRIBUTE_END);

        svgStr.append(ONCLICK_DISPLAY_DATA);

        svgStr.append(POLYLINE_END);
        svgStr.append(GROUP_END);
        svgStr.append(NEW_LINE);
        svgStr.append(NEW_LINE);

        return svgStr;
    }
#endif // SVG

public:
    Lander initial_lander;
    Lander current_lander;

    std::vector < Gene > genes;

    float score            = 0;
    float normalized_score = 0;

    unsigned int flags = 0;

    size_t last_useful_gene = 0;

#ifdef SVG
    Trajectory trajectory;
#endif // SVG
};

