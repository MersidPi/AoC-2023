#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
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

int M, N;

struct lens {
    std::string name;
    int num;
};

ull part1 (std::vector<std::string> linesFromInputFile) {
    M = linesFromInputFile.size(), N = linesFromInputFile[0].size();
    ull sum = 0;
    for (auto line : linesFromInputFile) {
        std::istringstream iss(line);
        std::string word;
        std::vector<std::vector<lens>> boxes(256);
        while (std::getline(iss, word, ',')) {
            ull currVal = 0;
            for (auto x : word)
                currVal = ((currVal + int(x)) * 17) % 256; 
            sum += currVal;
        }
    }
    return sum;
}

ull part2 (std::vector<std::string> linesFromInputFile) {
    M = linesFromInputFile.size(), N = linesFromInputFile[0].size();
    ull sum = 0;
    std::vector<std::vector<lens>> boxes(256);
    for (auto line : linesFromInputFile) {
        std::istringstream iss(line);
        std::string word;
        while (std::getline(iss, word, ',')) {
            ull currVal = 0;
            std::string name;
            int num;
            bool remove = false;
            for (auto x : word) {
                if (x >= 'a' && x <= 'z'){
                    currVal = ((currVal + int(x)) * 17) % 256; 
                    name.push_back(x);
                }
                if (x >= '0' && x <= '9')
                    num = x - '0';
                if (x == '-')
                    remove = true;
            }
            bool exists = false;
            if (remove)
                boxes[currVal].erase(std::remove_if(boxes[currVal].begin(), boxes[currVal].end(), [=](lens l1) {
                    return l1.name == name;
                }), boxes[currVal].end());
            else {
                for (auto &x : boxes[currVal]) {
                    if (x.name == name) {
                        x.num = num;
                        exists = true;
                        break;
                    }
                }
                if (!exists)
                    boxes[currVal].push_back({name, num});
            }
        }
    }
    sum = 0;
    for (int i = 0; i < boxes.size(); i++)
        for (int j = 0; j < boxes[i].size(); j++)
            sum += (i + 1) * (j + 1) * boxes[i][j].num;
    return sum;
}


int main () { 
    std::vector<std::string> linesFromInputFile = extractLinesFromInputFile("input.txt");
    ull part1result = part1(linesFromInputFile); std::cout << "\nPart 1: " << part1result << std::endl;
    ull part2result = part2(linesFromInputFile); std::cout << "\nPart 2: " << part2result << std::endl;
    return 0;
}

