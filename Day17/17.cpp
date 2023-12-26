#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <utility>
#include <queue>
#include <set>
#include <map>

typedef long long ull;

std::vector<std::string> extractLinesFromInputFile (std::string fileName) {
    std::ifstream inputFile(fileName);
    std::vector<std::string> lines;
    std::string inputLine;
    while(std::getline(inputFile, inputLine))
        lines.push_back(inputLine);
    return lines;
}
class NodeMap {
public:
    ull row, col, direction, directionCounter;
    bool operator<(const NodeMap &o) const {
        if (row != o.row) 
            return row < o.row;
        if (col != o.col) 
            return col < o.col;
        if (direction != o.direction) 
            return direction < o.direction;
        return directionCounter < o.directionCounter;
    }
};

class NodeQueue {
public:
    ull distance, row, col, direction, directionCounter;
    bool operator<(const NodeQueue &o) const {
        return distance > o.distance;
    }
};

ull part1 (std::vector<std::string> matrix) {
    ull M = matrix.size(), N = matrix[0].size();
    std::priority_queue<NodeQueue> queue;
    std::map<NodeMap, int> distances;
    queue.push({0,0,0,0,0});
    while (!queue.empty()) {
        NodeQueue node = queue.top();
        queue.pop();
        if (distances.find(NodeMap{node.row, node.col, node.direction, node.directionCounter}) != distances.end())
            continue;
        distances[NodeMap{node.row, node.col, node.direction, node.directionCounter}] = node.distance;
        std::vector<int> dx{1,-1,0,0};
        std::vector<int> dy{0,0,1,-1};
        for (int i = 0; i < 4; i++) {
            NodeQueue newNode = {node.distance,  // distance
                                 node.row + dx[i], // row
                                 node.col + dy[i], // col
                                 i, // direction, 0-down, 1-up, 2-right, 3-left
                                 0 // directionCounter
                                };
            if (!(newNode.row >= 0 && newNode.row < M && newNode.col >= 0 && newNode.col < N))
                continue;
            newNode.distance += int(matrix[newNode.row][newNode.col] - '0');
            if (newNode.direction == node.direction)
                newNode.directionCounter = node.directionCounter + 1;
            else newNode.directionCounter = 1;
            if (node.direction == 0 && newNode.direction != 1 || // so it doesnt go backwards
                node.direction == 1 && newNode.direction != 0 ||
                node.direction == 2 && newNode.direction != 3 ||
                node.direction == 3 && newNode.direction != 2) 
            {
                if (newNode.directionCounter <= 3) {
                    if (distances.find(NodeMap{newNode.row, newNode.col, newNode.direction, newNode.directionCounter}) != distances.end())
                        continue;
                    queue.push(newNode);
                }
            }
        }
    }
    ull sum = std::numeric_limits<ull>::max();
    for (auto x : distances)
        if (x.first.row == M - 1 && x.first.col == N - 1 && x.second < sum)
            sum = x.second;
    return sum;
}

ull part2 (std::vector<std::string> matrix) {
    ull M = matrix.size(), N = matrix[0].size();
    std::priority_queue<NodeQueue> queue;
    std::map<NodeMap, int> distances;
    queue.push({0,0,0,0,0});
    while (!queue.empty()) {
        NodeQueue node = queue.top();
        queue.pop();
        if (distances.find(NodeMap{node.row, node.col, node.direction, node.directionCounter}) != distances.end())
            continue;
        distances[NodeMap{node.row, node.col, node.direction, node.directionCounter}] = node.distance;
        std::vector<int> dx{1,-1,0,0};
        std::vector<int> dy{0,0,1,-1};
        for (int i = 0; i < 4; i++) {
            NodeQueue newNode = {node.distance,  // distance
                                 node.row + dx[i], // row
                                 node.col + dy[i], // col
                                 i, // direction, 0-down, 1-up, 2-right, 3-left
                                 0 // directionCounter
                                };
            if (!(newNode.row >= 0 && newNode.row < M && newNode.col >= 0 && newNode.col < N))
                continue;
            newNode.distance += int(matrix[newNode.row][newNode.col] - '0');
            if (newNode.direction == node.direction)
                newNode.directionCounter = node.directionCounter + 1;
            else newNode.directionCounter = 1;
            if (node.direction == 0 && newNode.direction != 1 || // so it doesnt go backwards
                node.direction == 1 && newNode.direction != 0 ||
                node.direction == 2 && newNode.direction != 3 ||
                node.direction == 3 && newNode.direction != 2) 
            {
                if (newNode.row >= 0 && newNode.row < M && newNode.col >= 0 && newNode.col < N && 
                    newNode.directionCounter <= 10 &&
                    (newNode.direction == node.direction ||
                     node.directionCounter >= 4 || node.directionCounter == 0)) {
                    if (distances.find(NodeMap{newNode.row, newNode.col, newNode.direction, newNode.directionCounter}) != distances.end())
                        continue;
                    queue.push(newNode);
                }
            }
        }
    }
    ull sum = std::numeric_limits<ull>::max();
    for (auto x : distances)
        if (x.first.row == M - 1 && x.first.col == N - 1 && x.second < sum && x.first.directionCounter >= 4)
            sum = x.second;
    return sum;
}


int main () { 
    std::vector<std::string> linesFromInputFile = extractLinesFromInputFile("input.txt");
    ull part1result = part1(linesFromInputFile); std::cout << "\nPart 1: " << part1result << std::endl;
    ull part2result = part2(linesFromInputFile); std::cout << "\nPart 2: " << part2result << std::endl;
    return 0;
}

