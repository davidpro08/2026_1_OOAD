#include "Simulator/GridMap.h"

GridMap::GridMap() : width(12), height(12) {
    resetDefault();
}

void GridMap::resetDefault() {
    cells.assign(height, std::vector<CellType>(width, CellType::Empty));

    for (int x = 0; x < width; ++x) {
        setWall(Point(x, 0));
        setWall(Point(x, height - 1));
    }
    for (int y = 0; y < height; ++y) {
        setWall(Point(0, y));
        setWall(Point(width - 1, y));
    }

    setWall(Point(4, 2));
    setWall(Point(4, 3));
    setWall(Point(4, 4));
    setWall(Point(2, 2));
    setWall(Point(2, 3));
    setWall(Point(2, 4));
    setWall(Point(2, 5));
    setWall(Point(2, 6));
    setWall(Point(2, 7));
    setWall(Point(2, 8));
    setWall(Point(2, 9));
    setWall(Point(2, 10));
    setWall(Point(3, 4));
    setWall(Point(7, 5));
    setWall(Point(8, 5));
    setWall(Point(3, 8));
    setWall(Point(4, 8));

    setDust(Point(2, 2));
    setDust(Point(6, 3));
    setDust(Point(9, 4));
    setDust(Point(2, 7));
    setDust(Point(8, 8));
    setDust(Point(5, 9));
}

int GridMap::getWidth() const {
    return width;
}

int GridMap::getHeight() const {
    return height;
}

bool GridMap::isInside(Point point) const {
    return point.x >= 0 && point.y >= 0 && point.x < width && point.y < height;
}

bool GridMap::isWall(Point point) const {
    return !isInside(point) || cells[point.y][point.x] == CellType::Wall;
}

bool GridMap::hasDust(Point point) const {
    return isInside(point) && cells[point.y][point.x] == CellType::Dust;
}

bool GridMap::isCleaned(Point point) const {
    return isInside(point) && cells[point.y][point.x] == CellType::Cleaned;
}

bool GridMap::clean(Point point) {
    if (!hasDust(point)) {
        return false;
    }

    cells[point.y][point.x] = CellType::Cleaned;
    return true;
}

bool GridMap::addDust(Point point) {
    if (!isInside(point) || isWall(point)) {
        return false;
    }

    cells[point.y][point.x] = CellType::Dust;
    return true;
}

bool GridMap::toggleWall(Point point) {
    if (!isInside(point)) {
        return false;
    }

    if (cells[point.y][point.x] == CellType::Wall) {
        cells[point.y][point.x] = CellType::Empty;
    } else {
        cells[point.y][point.x] = CellType::Wall;
    }
    return true;
}

CellType GridMap::getCell(Point point) const {
    if (!isInside(point)) {
        return CellType::Wall;
    }

    return cells[point.y][point.x];
}

std::vector<std::string> GridMap::render(Point robot, Point direction) const {
    std::vector<std::string> lines;
    lines.reserve(height);

    for (int y = 0; y < height; ++y) {
        std::string line;
        line.reserve(width);
        for (int x = 0; x < width; ++x) {
            Point current(x, y);
            if (robot.isEqual(current)) {
                char robotSymbol = 'R';
                if (direction.x == 0 && direction.y == -1) robotSymbol = '^';
                if (direction.x == 0 && direction.y == 1) robotSymbol = 'v';
                if (direction.x == -1 && direction.y == 0) robotSymbol = '<';
                if (direction.x == 1 && direction.y == 0) robotSymbol = '>';
                line.push_back(robotSymbol);
                continue;
            }

            switch (cells[y][x]) {
            case CellType::Wall:
                line.push_back('#');
                break;
            case CellType::Dust:
                line.push_back('*');
                break;
            case CellType::Cleaned:
                line.push_back('x');
                break;
            case CellType::Empty:
            default:
                line.push_back('.');
                break;
            }
        }
        lines.push_back(line);
    }

    return lines;
}

void GridMap::setWall(Point point) {
    if (isInside(point)) {
        cells[point.y][point.x] = CellType::Wall;
    }
}

void GridMap::setDust(Point point) {
    if (isInside(point) && !isWall(point)) {
        cells[point.y][point.x] = CellType::Dust;
    }
}
