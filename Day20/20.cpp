#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <map>
#include <queue>
#include <numeric>

typedef unsigned long long ull;

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

enum class Pulse{low, high};

struct FF {
    bool on;
    std::vector<std::string> dst;
};

struct Con {
    std::vector<std::string> dst;
    std::map<std::string, Pulse> recievedPulses;
};

struct Transfer {
    std::string src;
    Pulse pulse;
    std::string dst;
};

ull part1 (std::vector<std::string> linesFromInputFile) {
    std::map<std::string, FF> FFs;
    std::map<std::string, Con> Cons;
    std::vector<std::string> broadcastTo;
    for (auto line : linesFromInputFile) {
        std::istringstream iss(line);
        std::string s;
        iss >> s;
        std::string name = s.substr(1);
        char type = s[0];
        iss >> s; // this should be ->
        std::vector<std::string> dst;
        while (iss >> s && s.back() == ',')
            dst.push_back(s.substr(0, 2));
        dst.push_back(s);
        if (type == '%')
            FFs[name] = FF{false, dst};
        else if (type == '&')
            Cons[name] = Con{dst};
        else if (type == 'b')
            broadcastTo = dst;
    }
    for (auto ff : FFs)
        for (auto d : ff.second.dst)
            if (Cons.find(d) != Cons.end())
                Cons[d].recievedPulses[ff.first] = Pulse::low;
    std::queue<Transfer> pulses;
    ull lowCount = 0, highCount = 0;
    for (int i = 0; i < 1000; i++) {
        lowCount++;
        for (auto x : broadcastTo)
            pulses.push({"BROADCAST", Pulse::low, x});
        while (!pulses.empty()) {
            Pulse pulse = pulses.front().pulse;
            if (pulse == Pulse::low)
                lowCount++;
            else highCount++;
            std::string nameSrc = pulses.front().src;
            std::string nameDst = pulses.front().dst;
            pulses.pop();
            if (FFs.find(nameDst) != FFs.end() && pulse == Pulse::low) {
                if (FFs[nameDst].on)
                    for (auto x : FFs[nameDst].dst)
                        pulses.push({nameDst, Pulse::low, x});
                else
                    for (auto x : FFs[nameDst].dst)
                        pulses.push({nameDst, Pulse::high, x});
                FFs[nameDst].on = !FFs[nameDst].on;
            }
            else if (Cons.find(nameDst) != Cons.end()) {
                bool allHigh = true;
                Cons[nameDst].recievedPulses[nameSrc] = pulse;
                for (auto x : Cons[nameDst].recievedPulses)
                    if (x.second == Pulse::low) {
                        allHigh = false;
                        break;
                    }
                if (allHigh)
                    for (auto x : Cons[nameDst].dst)
                        pulses.push({"CON", Pulse::low, x});
                else 
                    for (auto x : Cons[nameDst].dst)
                        pulses.push({"CON", Pulse::high, x});
            }
        }
        for (auto x : Cons)
            for (auto y : x.second.recievedPulses)
                y.second = Pulse::low;
    }
    return lowCount * highCount;
}

ull lcmOfNums (std::vector<ull> v) {
    ull x = v.back();
    v.resize(v.size() - 1);
    if (v.size() == 0)
        return x;
    return std::lcm(x, lcmOfNums(v));
}

ull part2 (std::vector<std::string> linesFromInputFile) {
    std::map<std::string, FF> FFs;
    std::map<std::string, Con> Cons;
    std::vector<std::string> broadcastTo;
    for (auto line : linesFromInputFile) {
        std::istringstream iss(line);
        std::string s;
        iss >> s;
        std::string name = s.substr(1);
        char type = s[0];
        iss >> s; // this should be ->
        std::vector<std::string> dst;
        while (iss >> s && s.back() == ',')
            dst.push_back(s.substr(0, 2));
        dst.push_back(s);
        if (type == '%')
            FFs[name] = FF{false, dst};
        else if (type == '&')
            Cons[name] = Con{dst};
        else if (type == 'b')
            broadcastTo = dst;
    }
    for (auto ff : FFs)
        for (auto d : ff.second.dst)
            if (Cons.find(d) != Cons.end())
                Cons[d].recievedPulses[ff.first] = Pulse::low;
    for (auto ff : Cons)
        for (auto d : ff.second.dst)
            if (d == "lb" && Cons.find(d) != Cons.end()) // lb Con sends signal to rx
                Cons[d].recievedPulses[ff.first] = Pulse::low;
    std::vector<ull> wantedNums;
    std::queue<Transfer> pulses;
    ull lowCount = 0, highCount = 0;
    for (int i = 1; i < 1000000000; i++) {
        lowCount++;
        for (auto x : broadcastTo)
            pulses.push({"BROADCAST", Pulse::low, x});
        while (!pulses.empty()) {
            Pulse pulse = pulses.front().pulse;
            if (pulse == Pulse::low)
                lowCount++;
            else highCount++;
            std::string nameSrc = pulses.front().src;
            std::string nameDst = pulses.front().dst;
            pulses.pop();
            if (FFs.find(nameDst) != FFs.end() && pulse == Pulse::low) {
                if (FFs[nameDst].on)
                    for (auto x : FFs[nameDst].dst)
                        pulses.push({nameDst, Pulse::low, x});
                else
                    for (auto x : FFs[nameDst].dst)
                        pulses.push({nameDst, Pulse::high, x});
                FFs[nameDst].on = !FFs[nameDst].on;
            }
            else if (Cons.find(nameDst) != Cons.end()) {
                bool allHigh = true;
                Cons[nameDst].recievedPulses[nameSrc] = pulse;
                for (auto x : Cons[nameDst].recievedPulses)
                    if (x.second == Pulse::low) {
                        allHigh = false;
                        break;
                    }
                if (allHigh)
                    for (auto x : Cons[nameDst].dst)
                        pulses.push({"CON", Pulse::low, x});
                else {
                    for (auto x : Cons[nameDst].dst)
                        pulses.push({"CON", Pulse::high, x});
                    if (Cons["lb"].recievedPulses.find(nameDst) != Cons["lb"].recievedPulses.end()) {// lb is con that sends signal to rx
                        wantedNums.push_back(i);
                        if (wantedNums.size() == 4)
                            return lcmOfNums(wantedNums);
                    }
                }
            }
        }
        for (auto x : Cons)
            for (auto y : x.second.recievedPulses)
                y.second = Pulse::low;
    }
    return 0;
}

int main () { 
    std::vector<std::string> linesFromInputFile = extractLinesFromInputFile("input.txt");
    ull part1result = part1(linesFromInputFile); std::cout << "\nPart 1: " << part1result << std::endl;
    ull part2result = part2(linesFromInputFile); std::cout << "\nPart 2: " << part2result << std::endl;
    return 0;
}

