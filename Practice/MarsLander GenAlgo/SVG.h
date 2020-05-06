#pragma once

#include "SVG_Constants.h"

#include <fstream>
#include <string>
#include <vector>

class SVGManager {
public:
    SVGManager() {
        open();
    }

    ~SVGManager() noexcept {
        close();
    }

    void open() {
        file.open(SVG_HTML_FILE_NAME, std::fstream::out | std::fstream::trunc);

        if (!file.is_open()) {
            throw std::runtime_error("Cannot open SVG file");
        }

        file << FILE_START;
    }

    void close() noexcept {
        if (file.is_open()) {
            file.close();
        }
    }

    void done() noexcept {
        file << FILE_END;
    }

    void print(const std::string& s) noexcept {
        file << s;
    }

    std::string constructStrokeForRGB(int r, int g, int b) const noexcept {
        std::string res = STROKE;
        res.append(RGB_BEGIN);
        res.append(std::to_string(r));
        res.append(",");
        res.append(std::to_string(g));
        res.append(",");
        res.append(std::to_string(b));
        res.append(RGB_END);

        return res;
    }

    std::string constructStrokeWidth(int width) const {
        std::string res = STROKE_WIDTH;
        res.append(std::to_string(width));
        return res;
    }

    std::string constructGId(int id) const {
        std::string res = ID_BEGIN;
        res.append(std::to_string(id));
        res.append("\" ");
        return res;
    }

    std::string constructMultiLineTitle(const std::vector < std::string >& titleLines) const {
        std::string res = TITLE_BEGIN;
        for (size_t lineIdx = 0; lineIdx < titleLines.size(); ++lineIdx) {
            res.append(TSPAN_BEGIN);
            res.append(titleLines[lineIdx]);
            res.append(TSPAN_END);
            res.append("\n");
        }
        res.append(TITLE_END);

        return res;
    }

public:
    std::fstream file;
};
