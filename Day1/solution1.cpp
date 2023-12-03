#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::string> extractLinesFromInputFile (std::string fileName) {
    std::ifstream inputFile(fileName);
    std::vector<std::string> lines;
    std::string inputLine;
    while(std::getline(inputFile, inputLine))
        lines.push_back(inputLine);
    return lines;
}

const std::vector<std::string> numsWord = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

int part1 (std::vector<std::string> linesFromInputFile) {
    int sum = 0;
    for (std::string line : linesFromInputFile) {
        int lineLength = line.size();
        int firstNumber = 0, secondNumber = 0;
        for (int i = 0; i < lineLength; i++) {
            if (line[i] >= '0' && line[i] <= '9') {
                firstNumber = line[i] - '0';
                break;
            }
        }
        for (int i = lineLength - 1; i > -1; i--) {
            if (line[i] >= '0' && line[i] <= '9') {
                secondNumber = line[i] - '0';
                break;
            }
        }
        sum += firstNumber * 10 + secondNumber;
    }
    return sum;
}

int part2 (const std::vector<std::string>& linesFromInputFile) {
    int sum = 0;
    for (std::string line : linesFromInputFile) {
        int lineLength = line.size();
        int firstNumber = 0, firstNumberIndex = lineLength;
        bool foundFirstWord = false;
        for (int i = 0; i < lineLength; i++) {
            if (line[i] >= '0' && line[i] <= '9') {
                firstNumber = line[i] - '0';
                firstNumberIndex = i;
                break;
            }
            for (int j = 0; j < 10; j++) {
                if (numsWord[j].size() > lineLength - i)
                    continue;
                if (line.compare(i, numsWord[j].length(), numsWord[j]) == 0 && i <= firstNumberIndex) {
                    firstNumber = j;
                    firstNumberIndex = i;
                    foundFirstWord = true;
                    break;
                }
            }
            if (foundFirstWord)
                break;
        }
        int lastNumber = 0, lastNumberIndex = -1;
        bool foundLastWord = false;
        for (int i = lineLength - 1; i > -1; i--) {
            if (line[i] >= '0' && line[i] <= '9') {
                lastNumber = line[i] - '0';
                lastNumberIndex = i;
                break;
            }
            for (int j = 0; j < 10; j++) {
                if (numsWord[j].size() > lineLength - i)
                    continue;
                if (line.compare(i, numsWord[j].length(), numsWord[j]) == 0 && i >= lastNumberIndex) {
                    lastNumber = j;
                    lastNumberIndex = i;
                    foundLastWord = true;
                    break;
                }
            }
            if (foundLastWord)
                break;
        }
        sum += firstNumber * 10 + lastNumber;
    }
    return sum;
}

int main () {
    std::vector<std::string> linesFromInputFile = extractLinesFromInputFile("input.txt");
    int part1result = part1(linesFromInputFile);
    std::cout << "\nPart 1: " << part1result << std::endl;
    int part2result = part2(linesFromInputFile);
    std::cout << "\nPart 2: " << part2result << std::endl;
    return 0;
}