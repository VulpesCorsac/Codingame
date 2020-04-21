#pragma once

#ifdef SVG
    #include "SVG.h"
#endif // SVG

#include "Segment.h"
#include "Map_Constants.h"

#include <vector>

class Surface {
public:
    void addSegment(const Point& p0, const Point& p1) noexcept {
        return addSegment(Segment(p0, p1));
    }

    void addSegment(const Segment& line) noexcept {
        segments.push_back(line);
    }

    void findLandingSegment() noexcept {
        landing_segment_idx = SEGMENT_ID_INVALID;

        for (size_t i = 0; i < segments.size(); ++i) {
            if (segments[i].p0.x == segments[i].p1.x) {
                landing_segment_idx = i;

                break;
            }
        }
    }

    inline int landingSegment() const noexcept {
        return landing_segment_idx;
    }

    int collide(const Segment& trajectory) const noexcept {
        for (size_t i = 0; i < segments.size(); ++i) {
            if (cross(trajectory, segments[i])) {
                return i;
            }
        }

        return SEGMENT_ID_INVALID;
    }

    float distanceToLandingZone() const noexcept {
        /// TODO;
    }

public:
    std::vector < Segment > segments;
    std::vector < int > landing_zones_direction;

    int landing_segment_idx = SEGMENT_ID_INVALID;

#ifdef SVG
	std::string constructSVGData(const SVGManager& svgManager) const {
        std::string svgStr = POLYLINE_BEGIN;

        for (const auto& segment: segments) {
            float startX = segment.p0.x;
            float startY = MAP_HEIGHT - segment.p0.y;
            float endX = segment.p1.x;
            float endY = MAP_HEIGHT - segment.p1.y;

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
        std::string strokeRGB = svgManager.constructStrokeForRGB(255, 0, 0);
        svgStr.append(strokeRGB);
        svgStr.append(";");

        std::string strokeWidth = svgManager.constructStrokeWidth(10);
        svgStr.append(strokeWidth);
        svgStr.append(ATTRIBUTE_END);

        svgStr.append(POLYLINE_END);

        return svgStr;
	}
#endif // SVG

public:
    static constexpr int SEGMENT_ID_INVALID = -1;

    static constexpr int LANDING_ZONE_DIRECTION_INVALID = -2;
    static constexpr int LANDING_ZONE_DIRECTION_LEFT    = -1;
    static constexpr int LANDING_ZONE_DIRECTION_HERE    = 0;
    static constexpr int LANDING_ZONE_DIRECTION_RIGHT   = +1;
};

