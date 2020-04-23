#pragma once

#ifdef SVG
    #include "SVG.h"
#endif // SVG

#include "Segment.h"
#include "Map_Constants.h"

#include <vector>

constexpr int SEGMENT_ID_INVALID = -1;

constexpr int LANDING_ZONE_DIRECTION_INVALID = -2;
constexpr int LANDING_ZONE_DIRECTION_LEFT    = -1;
constexpr int LANDING_ZONE_DIRECTION_HERE    = 0;
constexpr int LANDING_ZONE_DIRECTION_RIGHT   = +1;

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
            if (segments[i].p0.y == segments[i].p1.y) {
                landing_segment_idx = i;

                break;
            }
        }
    }

    void fillLandingZoneDirection() noexcept {
        landing_zones_direction.assign(segments.size(), LANDING_ZONE_DIRECTION_INVALID);

        auto current_direction = (segments[0].p0.x > segments[landing_segment_idx].p0.x) ? LANDING_ZONE_DIRECTION_LEFT : LANDING_ZONE_DIRECTION_RIGHT;

        for (int idx = 0; idx < landing_segment_idx; ++idx) {
            landing_zones_direction[idx] = current_direction;
        }

        landing_zones_direction[landing_segment_idx] = LANDING_ZONE_DIRECTION_HERE;

        current_direction = current_direction == LANDING_ZONE_DIRECTION_LEFT ? LANDING_ZONE_DIRECTION_RIGHT : LANDING_ZONE_DIRECTION_LEFT;

        for (size_t idx = landing_segment_idx+1; idx < segments.size(); ++idx) {
            landing_zones_direction[idx] = current_direction;
        }
     }

    inline int landingSegmentID() const noexcept {
        return landing_segment_idx;
    }

    int collide(const Point& p1, const Point& p2) const noexcept {
        return collide({p1, p2});
    }

    int collide(const Segment& trajectory) const noexcept {
        for (size_t i = 0; i < segments.size(); ++i) {
            if (cross(trajectory, segments[i])) {
                return i;
            }
        }

        return SEGMENT_ID_INVALID;
    }

    float distanceToLandingZone(const Point& point, int segment_idx) const noexcept {
        if (segment_idx != SEGMENT_ID_INVALID) {
            if (segment_idx == landing_segment_idx) {
                return std::min(distance(point, segments[segment_idx].p0), distance(point, segments[segment_idx].p1));
            } else {
                auto point_to_segment_edge = landing_zones_direction[segment_idx] == LANDING_ZONE_DIRECTION_LEFT ? segments[segment_idx].p0 : segments[segment_idx].p1;

                auto ans = distance(point, point_to_segment_edge);

                auto d_idx = landing_zones_direction[segment_idx] == LANDING_ZONE_DIRECTION_LEFT ? LANDING_ZONE_DIRECTION_LEFT : LANDING_ZONE_DIRECTION_RIGHT;
                segment_idx += d_idx;

                while (landing_zones_direction[segment_idx] != LANDING_ZONE_DIRECTION_HERE) {
                    ans += segments[segment_idx].length;
                    segment_idx += d_idx;
                }

                return ans;
            }
        } else {
            return MAP_MAX_DISTANCE;
        }
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
};

