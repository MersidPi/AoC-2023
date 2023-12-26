#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

typedef unsigned long long ull;

class AbstractInterpolator {
    mutable ull location = 0;
protected:
    std::vector<std::pair<double, double>> dataSet;
    ull Locate (double x) const;
public:
    AbstractInterpolator (const std::vector<std::pair<double, double>> &data);
    virtual double operator() (double x) const = 0;
};

ull AbstractInterpolator::Locate (double x) const {
    ull &l = this->location;
    if (l == 0) {
        if (x <= dataSet[0].first) return l;
        if (dataSet[0].first < x && x <= dataSet[1].first) return ++l;
    }
    else if (l == 1) {
        if (dataSet[l - 1].first < x && x <= dataSet[l].first) return l;
        if (dataSet[l].first < x && x <= dataSet[l + 1].first) return ++l;
        if (x <= dataSet[l - 1].first) return --l;
    }
    else if (l == dataSet.size()) {
        if (dataSet[l - 1].first < x) return l;
        if (dataSet[l - 2].first < x && x <= dataSet[l - 1].first) return --l;
    }
    else if (l == dataSet.size() - 1) {
        if (dataSet[l - 1].first < x && x <= dataSet[l].first) return l;
        if (dataSet[l].first < x) return ++l;
    }
    else {
        if (dataSet[l - 1].first < x && x <= dataSet[l].first) return l;
        if (dataSet[l].first < x && x <= dataSet[l + 1].first) return ++l;
        if (dataSet[l - 2].first < x && x <= dataSet[l - 1].first) return --l;
    }
    l = std::lower_bound(dataSet.begin(), dataSet.end(), std::make_pair(x,1.)) - dataSet.begin();
    return l;
}

AbstractInterpolator::AbstractInterpolator (const std::vector<std::pair<double, double>> &data) {
    this->dataSet = data;
    std::sort(dataSet.begin(), dataSet.end(), 
      [](std::pair<double, double> p1, std::pair<double, double> p2) {
        return p1.first < p2.first;
    });
    for (ull i = 0; i < dataSet.size() - 1; i++)
        if (dataSet[i].first == dataSet[i + 1].first) 
            throw std::domain_error("Invalid data set");
}

class PolynomialInterpolator : public AbstractInterpolator {
    std::vector<double> coeff;
    std::vector<double> additionalVals;
public:
    PolynomialInterpolator (const std::vector<std::pair<double, double>> &data);
    double operator() (double x) const override;
    void AddPoint (const std::pair<double, double> &p);
    std::vector<double> GetCoefficients () const;
};

PolynomialInterpolator::PolynomialInterpolator (const std::vector<std::pair<double, double>> &data) : AbstractInterpolator(data) {
    for (std::pair<double, double> pair : dataSet)
        coeff.push_back(pair.second);
    additionalVals.resize(coeff.size());
    additionalVals[0] = coeff[coeff.size() - 1];
    for (ull j = 0; j < dataSet.size() - 1; j++)
        for (ull i = dataSet.size() - 1; i >= j + 1; i--){
            coeff[i] = (coeff[i] - coeff[i - 1]) / (dataSet[i].first - dataSet[i - j - 1].first);
            if (i == dataSet.size() - 1) 
                additionalVals[j + 1] = coeff[i];
        }
}

double PolynomialInterpolator::operator() (double x) const {
    double y = coeff[coeff.size() - 1];
    for (ull i = coeff.size() - 2; i >= 0; i--) 
        y = y * (x - dataSet[i].first) + coeff[i];
    return y;
}

void PolynomialInterpolator::AddPoint (const std::pair<double, double> &p) {
    if (std::find_if(dataSet.begin(), dataSet.end(), [p](std::pair<double, double> p1) { return p.first == p1.first; }) != dataSet.end())
        throw std::domain_error("Invalid point");
    dataSet.push_back(p);
    additionalVals.push_back(0);
    coeff.push_back(0);
    double pom1, pom2;
    pom1 = additionalVals[0];
    additionalVals[0] = p.second;
    for (ull i = 1; i < dataSet.size(); i++) {
        pom2 = additionalVals[i];
        additionalVals[i] = (additionalVals[i - 1] - pom1) / (dataSet[dataSet.size() - 1].first - dataSet[dataSet.size() - 1 - i].first);
        pom1 = pom2;
    }
    coeff[coeff.size() - 1] = additionalVals[additionalVals.size() - 1];
}

std::vector<double> PolynomialInterpolator::GetCoefficients () const {
    ull n = dataSet.size();
    std::vector<double> p(n, 0);
    std::vector<double> w(n);
    p[0] = coeff[0];
    w[0] = 1;
    for (ull i = 1; i < n; i++) {
        w[i] = w[i - 1];
        for (ull j = i - 1; j >= 1; j--)
            w[j] = w[j - 1] - dataSet[i-1].first * w[j];
        w[0] = -dataSet[i-1].first * w[0];
        for (ull j = 0; j < n; j++)
            p[j] += coeff[i] * w[j];
    }
    return p;
}

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

struct Position {
    ull x, y;
};

ull part1 (std::vector<std::string> grid) {
    ull M = grid.size(), N = grid[0].size();
    Position posS = {0, 0};
    for (ull i = 0; i < M; i++) {
        for (ull j = 0; j < N; j++) {
            if (grid[i][j] == 'S') {
                posS = {i, j};
                grid[i][j] = 'O';
                break;
            }
        }
        if (posS.x != 0)
            break;
    }
    ull steps = 64;
    for (ull k = 0; k < steps; k++) {
        for (ull i = 0; i < M; i++) {
            for (ull j = 0; j < N; j++) {
                if (grid[i][j] == 'O') {
                    grid[i][j] = '.';
                    if (i > 0 && grid[i - 1][j] == '.')
                        grid[i - 1][j] = '$';
                    if (i < M - 1 && grid[i + 1][j] == '.')
                        grid[i + 1][j] = '$';
                    if (j > 0 && grid[i][j - 1] == '.')
                        grid[i][j - 1] = '$';
                    if (j < N - 1 && grid[i][j + 1] == '.')
                        grid[i][j + 1] = '$';
                }
            }
        }
        for (ull i = 0; i < M; i++)
            for (ull j = 0; j < N; j++)
                if (grid[i][j] == '$')
                    grid[i][j] = 'O';
    }
    ull counter = 0;
    for (auto x : grid)
        for (auto z : x)
            if (z == 'O') 
                counter++;
    return counter;
}

ull numOfPositions (std::vector<std::string> grid, ull steps) {
    ull M = grid.size(), N = grid[0].size();
    Position posS = {0, 0};
    std::vector<Position> currPos;
    for (ull i = 0; i < M; i++) {
        for (ull j = 0; j < N; j++) {
            if (grid[i][j] == 'S') {
                posS = {i, j};
                currPos.push_back(posS);
                grid[i][j] = 'O';
            }
        }
    }
    for (ull k = 0; k < steps; k++) {
        for (ull i = 0; i < M; i++) {
            for (ull j = 0; j < N; j++) {
                if (grid[i][j] == 'O') {
                    grid[i][j] = '.';
                    if (i > 0 && grid[i - 1][j] == '.')
                        grid[i - 1][j] = '$';
                    if (i < M - 1 && grid[i + 1][j] == '.')
                        grid[i + 1][j] = '$';
                    if (j > 0 && grid[i][j - 1] == '.')
                        grid[i][j - 1] = '$';
                    if (j < N - 1 && grid[i][j + 1] == '.')
                        grid[i][j + 1] = '$';
                }
            }
        }
        for (ull i = 0; i < M; i++)
            for (ull j = 0; j < N; j++)
                if (grid[i][j] == '$')
                    grid[i][j] = 'O';
    }
    ull counter = 0;
    for (auto x : grid)
        for (auto z : x)
            if (z == 'O') 
                counter++;
    return counter;
}

ull part2 (std::vector<std::string> grid) {
    // 26501365(total step count) = 131(grid width/length) * 202300 + 65
    // finding polynomial (quadratic function) that passes through these three points
    // numofpos(grid, 65) gives num of positions after 65 steps, first number 0 means that its the first grid
    // without expanding it, at the end we expand it 202300 times and see the result, magically works :D
    std::vector<std::pair<double, double>> data = {std::make_pair(0, numOfPositions(grid, 65)), 
                                                std::make_pair(1, numOfPositions(grid, 65 + 131)), 
                                                std::make_pair(2, numOfPositions(grid, 65 + 131 + 131))};
    PolynomialInterpolator pi(data);
    auto coeffs = pi.GetCoefficients();
    ull result = 0;
    for (int i = 0; i < coeffs.size(); i++) {
        ull x = 1;
        for (int j = 0; j < i; j++)
            x *= 202300;
        result += coeffs[i] * x;
    }
    return result;
}


int main () { 
    std::vector<std::string> linesFromInputFile = extractLinesFromInputFile("lala.txt"); 
    ull part1result = part1(linesFromInputFile); std::cout << "\nPart 1: " << part1result << std::endl;
    std::vector<std::string> linesFromInputFile2 = extractLinesFromInputFile("input.txt"); 
    ull part2result = part2(linesFromInputFile2); std::cout << "\nPart 2: " << part2result << std::endl;
    return 0;
}

