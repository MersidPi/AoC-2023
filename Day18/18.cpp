#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>

typedef long long ll;

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

ll part1WithFormulas (std::vector<std::string> linesFromInputFile) {
    std::vector<std::pair<ll, ll>> vertices;
    vertices.push_back({0, 0});
    std::string oldDirection = "U";
    ll s = 0;
    for (auto line : linesFromInputFile) {
        std::istringstream iss(line);
        std::vector<std::string> l = parse<std::string>(iss, '\n');
        std::string direction = l[0];
        ll steps = std::stoi(l[1]);
        if (direction == "R") 
            vertices.push_back({vertices.back().first, vertices.back().second + steps});
        if (direction == "L")
            vertices.push_back({vertices.back().first, vertices.back().second - steps});
        if (direction == "D")
            vertices.push_back({vertices.back().first + steps, vertices.back().second});
        if (direction == "U")
            vertices.push_back({vertices.back().first - steps, vertices.back().second});
        s += steps;
        oldDirection = direction;
    }
    long double sum = 0;
    for (int i = 0; i < vertices.size() - 1; i++)
        sum += (long double)(vertices[i].first * vertices[i + 1].second - vertices[i + 1].first * vertices[i].second) / 2;
    sum = std::abs(sum);
    sum = sum + 1 + s / 2; // pick theorem and shoelace formula
    return sum;
}


ll part1 (std::vector<std::string> linesFromInputFile) {
    ll sum = 0; 
    ll cols = 0, rows = 0, 
        minCols = std::numeric_limits<ll>::max(), maxCols = std::numeric_limits<ll>::min(),
        minRows = std::numeric_limits<ll>::max(), maxRows = std::numeric_limits<ll>::min(),
        R = 0, D = 0, U = 0, L = 0;
    std::vector<std::vector<std::string>> moves; 
    for (auto line : linesFromInputFile) {
        std::istringstream iss(line);
        std::vector<std::string> l = parse<std::string>(iss, '\n');
        moves.push_back(l);
        std::string direction = l[0];
        ll steps = std::stoi(l[1]);
        std::string color = l[2];
        if (direction == "R")
            R += steps, cols += steps;
        if (direction == "L")
            L -= steps, cols -= steps;
        if (direction == "D")
            D += steps, rows += steps;
        if (direction == "U")
            U -= steps, rows -= steps;
        minCols = std::min(cols, minCols);
        maxCols = std::max(cols, maxCols);
        minRows = std::min(rows, minRows);
        maxRows = std::max(rows, maxRows);

    }
    std::vector<std::string> matrix(std::abs(minRows) + maxRows + 1, std::string(std::abs(minCols) + maxCols + 1, '.'));
    ll M = matrix.size(), N = matrix[0].size(), i = std::abs(minRows), j = std::abs(minCols);
    matrix[i][j] = '#';
    for (auto l : moves) {
        std::string direction = l[0];
        ll steps = std::stoi(l[1]);
        std::string color = l[2];
        if (direction == "D")
            matrix[i][j] = 'v';
        while (steps-- != 0) {
            if (direction == "R")
                j++;
            if (direction == "L")
                j--;
            if (direction == "D")
                i++;
            if (direction == "U")
                i--;
            if (direction == "U")
                matrix[i][j] = '^';
            else if (direction == "D")
                matrix[i][j] = 'v';
            else matrix[i][j] = '#';
        }
        if (direction == "D")
            matrix[i][j] = '#';

    }
    for (ll i = 0; i < M; i++) {
        bool inside = false;
        for (ll j = 0; j < N; j++) {
            if (matrix[i][j] == '^')
                inside = true;
            if (matrix[i][j] == 'v')
                inside = false;
            if (inside)
                matrix[i][j] = '#';
        }
    }
    for (ll i = 0; i < M; i++)
        for (ll j = 0; j < N; j++)
            if (matrix[i][j] == 'v' || matrix[i][j] == '#')
                sum++;
    return sum;
}

ll part2 (std::vector<std::string> linesFromInputFile) {
    std::vector<std::pair<ll, ll>> vertices;
    ll borderPoints = 0;
    vertices.push_back({0, 0});
    for (auto line : linesFromInputFile) {
        std::istringstream iss(line);
        std::vector<std::string> l = parse<std::string>(iss, '\n');
        size_t pos;
        ll steps = std::stoi(l[2].substr(2, l[2].size() - 4), &pos, 16);
        ll direction = l[2][7] - '0';
        if (direction == 0)
            vertices.push_back({vertices.back().first, vertices.back().second + steps});
        if (direction == 2)
            vertices.push_back({vertices.back().first, vertices.back().second - steps});
        if (direction == 1)
            vertices.push_back({vertices.back().first + steps, vertices.back().second});
        if (direction == 3)
            vertices.push_back({vertices.back().first - steps, vertices.back().second});
        borderPoints += steps;
    }
    long double area = 0;
    for (int i = 0; i < vertices.size() - 1; i++)
        area += (long double)(vertices[i].first * vertices[i + 1].second - vertices[i + 1].first * vertices[i].second) / 2;
    area = std::abs(area);
    area = area + 1 + borderPoints / 2; // pick theorem and shoelace formula
    return area;
}


int main () { 
    std::vector<std::string> linesFromInputFile = extractLinesFromInputFile("input.txt");
    //ll part11result = part1WithFormulas(linesFromInputFile); std::cout << "\nPart 1: " << part11result << std::endl;
    ll part1result = part1(linesFromInputFile); std::cout << "\nPart 1: " << part1result << std::endl;
    ll part2result = part2(linesFromInputFile); std::cout << "\nPart 2: " << part2result << std::endl;
    return 0;
}

