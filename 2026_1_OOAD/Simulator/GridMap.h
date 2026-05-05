#pragma once

#include "Class/Motor.h"

#include <string>
#include <vector>

enum class CellType {
    Empty,
    Wall,
    Dust,
    Cleaned
};

class GridMap {
public:
    GridMap();

    void resetDefault();
    int getWidth() const;
    int getHeight() const;
    bool isInside(Point point) const;
    bool isWall(Point point) const;
    bool hasDust(Point point) const;
    bool isCleaned(Point point) const;
    bool clean(Point point);
    bool addDust(Point point);
    bool toggleWall(Point point);
    CellType getCell(Point point) const;
    std::vector<std::string> render(Point robot, Point direction) const;

private:
    int width;
    int height;
    std::vector<std::vector<CellType>> cells;

    void setWall(Point point);
    void setDust(Point point);
};
