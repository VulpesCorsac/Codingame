#pragma once

#include "Gene.h"
#include "Point.h"
#include "Lander.h"
#include "Map_Constants.h"

#ifdef SVG
    #include "SVG.h"
#endif // SVG

#include <vector>

typedef std::vector < Point > Path;

class Chromosome {
public:

#ifdef SVG
    std::string constructSVGData(const SVGManager& svg) const noexcept {
        std::string svgStr = "";

        svgStr.append(GROUP_BEGIN);
        svgStr.append(NEW_LINE);

        std::vector < std::string > titleLines = current_lander.getTitleLines();
        std::string evaluationLine = "Evaluation: ";
        evaluationLine.append(std::to_string(original_grade));
        titleLines.push_back(evaluationLine);
        std::string title = svg.constructMultiLineTitle(titleLines);
        svgStr.append(title);
        svgStr.append(NEW_LINE);
        svgStr.append(NEW_LINE);

        svgStr.append(POLYLINE_BEGIN);

        for (size_t positionIdx = 0; positionIdx < path.size() - 1; ++positionIdx) {
            Point position = path[positionIdx];
            Point nextPosition = path[positionIdx + 1];

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

        if (hasFlag(SELECTED_FLAG)) {
            strokeRGB = svg.constructStrokeForRGB(0, 0, 255);
        }
        else if (hasFlag(SOLUTION_FLAG)) {
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

    bool operator < (const Chromosome& other) {
        return grade < other.grade;
    }

public:
    Lander initial_lander;
    Lander current_lander;

    std::vector < Gene > genes;

    float grade;

#ifdef SVG
    float original_grade;

    Path path;
#endif // SVG
};

