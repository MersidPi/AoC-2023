#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>

typedef long long ull;

std::vector<std::string> extractLinesFromInputFile (std::string fileName) {
    std::ifstream inputFile(fileName);
    std::vector<std::string> lines;
    std::string inputLine;
    while(std::getline(inputFile, inputLine))
        lines.push_back(inputLine);
    return lines;
}

template <typename T>
std::vector<T> parse (std::istringstream &stream, char ending) {
    std::vector<T> result;
    std::string subStreamString;
    getline(stream, subStreamString, ending);
    std::istringstream subStream(subStreamString);
    T element;
    while (subStream >> element)
        result.push_back(element);
    return result;
}

ull M, N;

enum direction {up, left, down, right};
struct coords {
    ull x, y;
    direction heading;
    bool operator==(const coords& other) const {
        return x == other.x && y == other.y;
    }
};

std::vector<coords> energizedCoords;

void energize (std::vector<std::string> grid, int i, int j, direction d) {
    if (j != -1 && i != -1 && j != N && i != M && std::find_if(energizedCoords.begin(), energizedCoords.end(), 
        [=](coords c) { return c.x == i && c.y == j;}) == energizedCoords.end())
        energizedCoords.push_back({i, j, d});
    if (d == up)
        i--;
    else if (d == left)
        j--;
    else if (d == down)
        i++;
    else if (d == right)
        j++;
    if (!(i >= 0 && j >= 0 && i < M && j < N))
        return;
    if (std::find_if(energizedCoords.begin(), energizedCoords.end(), 
            [=](coords c) { return c.x == i && c.y == j; }) == energizedCoords.end())
        energizedCoords.push_back({i, j, d});
    while (i >= 0 && j >= 0 && i < M && j < N && grid[i][j] == '.') {
        if (d == up)
            i--;
        else if (d == left)
            j--;
        else if (d == down)
            i++;
        else if (d == right)
            j++;
        if (!(i >= 0 && j >= 0 && i < M && j < N) || 
            std::find_if(energizedCoords.begin(), energizedCoords.end(), 
                [=](coords c) { return c.x == i && c.y == j && c.heading == d;}) != energizedCoords.end())
            return;
        if (std::find_if(energizedCoords.begin(), energizedCoords.end(), 
                [=](coords c) { return c.x == i && c.y == j; }) == energizedCoords.end())
            energizedCoords.push_back({i, j, d});
    }
    if (grid[i][j] == '-' && (d == up || d == down)) {
        energize(grid, i, j, right);
        energize(grid, i, j, left);
    }
    else if (grid[i][j] == '-' && (d == up || d == down)) {
        energize(grid, i, j, left);
        energize(grid, i, j, right);
    }
    else if (grid[i][j] == '-' && (d == right)) {
        energize(grid, i, j, right);
    }
    else if (grid[i][j] == '-' && (d == left)) {
        energize(grid, i, j, left);
    }
    else if (grid[i][j] == '|' && (d == right || d == left)) {
        energize(grid, i, j, up);
        energize(grid, i, j, down);
    }
    else if (grid[i][j] == '|' && (d == up)) {
        energize(grid, i, j, up);
    }
    else if (grid[i][j] == '|' && (d == down)) {
        energize(grid, i, j, down);
    }
    else if (grid[i][j] == '\\' && (d == up)) {
        energize(grid, i, j, left);
    }
    else if (grid[i][j] == '\\' && (d == down)) {
        energize(grid, i, j, right);
    }
    else if (grid[i][j] == '\\' && (d == right)) {
        energize(grid, i, j, down);
    }
    else if (grid[i][j] == '\\' && (d == left)) {
        energize(grid, i, j, up);
    }
    else if (grid[i][j] == '/' && (d == up)) {
        energize(grid, i, j, right);
    }
    else if (grid[i][j] == '/' && (d == right)) {
        energize(grid, i, j, up);
    }
    else if (grid[i][j] == '/' && (d == left)) {
        energize(grid, i, j, down);
    }
    else if (grid[i][j] == '/' && (d == down)) {
        energize(grid, i, j, left);
    }
}

ull part1 (std::vector<std::string> linesFromInputFile) {
    M = linesFromInputFile.size(), N = linesFromInputFile[0].size();
    ull sum = 0;
    energize(linesFromInputFile, 0, -1, right);
    sum = energizedCoords.size();
    return sum;
}

ull part2 (std::vector<std::string> linesFromInputFile) {
    M = linesFromInputFile.size(), N = linesFromInputFile[0].size();
    ull sum = 0;
    for (int i = 0; i < M; i++) {
        energizedCoords.resize(0);
        energize(linesFromInputFile, i, -1, right);
        if (energizedCoords.size() > sum)
            sum = energizedCoords.size();
    }
    for (int i = 0; i < M; i++) {
        energizedCoords.resize(0);
        energize(linesFromInputFile, i, N, left);
        if (energizedCoords.size() > sum)
            sum = energizedCoords.size();
    }
    for (int j = 0; j < N; j++) {
        energizedCoords.resize(0);
        energize(linesFromInputFile, -1, j, down);
        if (energizedCoords.size() > sum)
            sum = energizedCoords.size();
    }
    for (int j = 0; j < N; j++) {
        energizedCoords.resize(0);
        energize(linesFromInputFile, M, j, up);
        if (energizedCoords.size() > sum)
            sum = energizedCoords.size();
    }
    return sum;
}


int main () { 
    std::vector<std::string> linesFromInputFile = extractLinesFromInputFile("input.txt");
    ull part1result = part1(linesFromInputFile); std::cout << "\nPart 1: " << part1result << std::endl;
    ull part2result = part2(linesFromInputFile); std::cout << "\nPart 2: " << part2result << std::endl;
    return 0;
}

