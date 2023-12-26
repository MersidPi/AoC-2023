#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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

int M, N;

void rollNorth (std::vector<std::string> &matrix) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (matrix[i][j] == 'O') {
                matrix[i][j] = '.';
                int k = i;
                for (; k >= 1; k--)
                    if (matrix[k - 1][j] != '.')
                        break;
                matrix[k][j] = 'O';
            }
        }
    }
}

void rollWest (std::vector<std::string> &matrix) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (matrix[i][j] == 'O') {
                matrix[i][j] = '.';
                int k = j;
                for (; k >= 1; k--)
                    if (matrix[i][k - 1] != '.')
                        break;
                matrix[i][k] = 'O';
            }
        }
    }
}

void rollSouth (std::vector<std::string> &matrix) {
    for (int i = M - 1; i >= 0; i--) {
        for (int j = N - 1; j >= 0; j--) {
            if (matrix[i][j] == 'O') {
                matrix[i][j] = '.';
                int k = i;
                for (; k < M - 1; k++)
                    if (matrix[k + 1][j] != '.')
                        break;
                matrix[k][j] = 'O';
            }
        }
    }
}

void rollEast (std::vector<std::string> &matrix) {
    for (int i = M - 1; i >= 0; i--) {
        for (int j = N - 1; j >= 0; j--) {
            if (matrix[i][j] == 'O') {
                matrix[i][j] = '.';
                int k = j;
                for (; k < N - 1; k++)
                    if (matrix[i][k + 1] != '.')
                        break;
                matrix[i][k] = 'O';
            }
        }
    }
}

ll part1 (std::vector<std::string> matrix) {
    M = matrix.size(), N = matrix[0].size();
    ll sum = 0;
    rollNorth(matrix);
    sum = 0;
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            if (matrix[i][j] == 'O')
                sum += M - i;
    return sum;
}

ll part2 (std::vector<std::string> matrix) {
    M = matrix.size(), N = matrix[0].size();
    ll sum = 0;
    std::vector<std::vector<std::string>> matrices;
    ll startOfCycle = 0;
    ll cycleLen = 0;
    for (int i = 0; i < 1000; i++) {
        rollNorth(matrix);
        rollWest(matrix);
        rollSouth(matrix);
        rollEast(matrix);
        for (int k = 0; k < matrices.size(); k++) {
            if (matrix == matrices[k] && k < matrices.size() - 1) {
                startOfCycle = k;
                cycleLen = matrices.size() - k;
                break;
            }
        }
        matrices.push_back(matrix);
    }
    ll index = 1000000000 % cycleLen - 1;
    while (index < startOfCycle)
        index += cycleLen;
    matrix = matrices[index];
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            if (matrix[i][j] == 'O')
                sum += M - i;
    return sum;
}


int main () {
    std::vector<std::string> linesFromInputFile = extractLinesFromInputFile("input.txt");
    ll part1result = part1(linesFromInputFile); std::cout << "\nPart 1: " << part1result << std::endl;
    ll part2result = part2(linesFromInputFile); std::cout << "\nPart 2: " << part2result << std::endl;
    return 0;
}

