#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <set>
#include <unordered_set>

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

struct Rule {
    char parameter, sign;
    ull value;
    std::string nextWorkflow;
};

struct Workflow {
    std::string name;
    std::vector<Rule> rules;
};

struct Part {
    ull x, m, a, s;
};

ull part1 (std::vector<std::string> linesFromInputFile) {
    ull i = 0;
    std::vector<std::string> wf;
    while (linesFromInputFile[i] != "")
        wf.push_back(linesFromInputFile[i++]);
    std::vector<std::string> partsStr;
    i++;
    while (i < linesFromInputFile.size())
        partsStr.push_back(linesFromInputFile[i++]);
    std::vector<Workflow> workflows(wf.size());
    for (ull i = 0; i < wf.size(); i++) {
        for (ull j = 0; j < wf[i].size(); j++)
            if (wf[i][j] == '{' || wf[i][j] == ',' || wf[i][j] == '}')
                wf[i][j] = ' ';
        std::istringstream iss(wf[i]);
        iss >> workflows[i].name;
        std::string ruleStr;
        while (iss >> ruleStr) {
            if (ruleStr.size() == 1 || ruleStr[1] != '>' && ruleStr[1] != '<') { // for default 
                workflows[i].rules.push_back(Rule{0,0,0,ruleStr});
                break;
            }
            Rule r;
            r.parameter = ruleStr[0];
            r.sign = ruleStr[1];
            r.value = std::stoi(ruleStr.substr(2));
            ull j = 0;
            while (ruleStr[j++] != ':');
            r.nextWorkflow = ruleStr.substr(j);
            workflows[i].rules.push_back(r);
        }
    }
    std::vector<Part> parts(partsStr.size());
    for (ull i = 0; i < partsStr.size(); i++) {
        for (ull j = 0; j < partsStr[i].size(); j++)
            if (partsStr[i][j] == '{' || partsStr[i][j] == 'x' || partsStr[i][j] == '=' || partsStr[i][j] == ',' || partsStr[i][j] == 'm' || partsStr[i][j] == 'a' || partsStr[i][j] == 's' || partsStr[i][j] == '}')
                partsStr[i][j] = ' ';
        std::istringstream iss(partsStr[i]);
        iss >> parts[i].x >> parts[i].m >> parts[i].a >> parts[i].s;
    }
    ull sum = 0;
    for (ull i = 0; i < parts.size(); i++) {
        std::string currentWorkflow = "in";
        while (currentWorkflow != "A" && currentWorkflow != "R") {
            ull wfIndex = 0;
            while (workflows[wfIndex].name != currentWorkflow)
                wfIndex++;
            for (ull j = 0; j < workflows[wfIndex].rules.size(); j++) {
                Rule r = workflows[wfIndex].rules[j];
                if (r.parameter == 'x' && r.sign == '>' && parts[i].x > r.value ||
                    r.parameter == 'x' && r.sign == '<' && parts[i].x < r.value ||
                    r.parameter == 'm' && r.sign == '>' && parts[i].m > r.value ||
                    r.parameter == 'm' && r.sign == '<' && parts[i].m < r.value ||
                    r.parameter == 'a' && r.sign == '>' && parts[i].a > r.value ||
                    r.parameter == 'a' && r.sign == '<' && parts[i].a < r.value ||
                    r.parameter == 's' && r.sign == '>' && parts[i].s > r.value ||
                    r.parameter == 's' && r.sign == '<' && parts[i].s < r.value) {
                        currentWorkflow = r.nextWorkflow;
                        break;
                }
                else if (r.parameter == 0) {
                    currentWorkflow = r.nextWorkflow;
                    break;
                }
            }
            if (currentWorkflow == "A")
                sum += parts[i].x + parts[i].m + parts[i].a + parts[i].s;
        }
    }
    return sum;
}

void limit(const std::vector<Workflow> &workflows, std::vector<std::pair<Part,Part>> &bounds, Part lower, Part upper, std::string currWF) {
    if (currWF == "R")
        return;
    else if (currWF == "A") {
        bounds.push_back({lower, upper});
        return;
    }
    ull wfIndex = 0;
    while (workflows[wfIndex].name != currWF)
        wfIndex++;
    for (ull i = 0; i < workflows[wfIndex].rules.size(); i++) {
        Rule r = workflows[wfIndex].rules[i];
        Part oldLower = lower, oldUpper = upper;
        if (r.parameter == 'x' && r.sign == '>' && lower.x < r.value + 1)
            lower.x = r.value + 1;
        else if (r.parameter == 'x' && r.sign == '<' && upper.x > r.value - 1)
            upper.x = r.value - 1;
        else if (r.parameter == 'm' && r.sign == '>' && lower.m < r.value + 1)
            lower.m = r.value + 1;
        else if (r.parameter == 'm' && r.sign == '<' && upper.m > r.value - 1)
            upper.m = r.value - 1;
        else if (r.parameter == 'a' && r.sign == '>' && lower.a < r.value + 1)
            lower.a = r.value + 1;
        else if (r.parameter == 'a' && r.sign == '<' && upper.a > r.value - 1)
            upper.a = r.value - 1;
        else if (r.parameter == 's' && r.sign == '>' && lower.s < r.value + 1)
            lower.s = r.value + 1;
        else if (r.parameter == 's' && r.sign == '<' && upper.s > r.value - 1)
            upper.s = r.value - 1;
        limit(workflows, bounds, lower, upper, r.nextWorkflow);
        lower = oldLower;
        upper = oldUpper;
        if (r.parameter == 'x' && r.sign == '>' && upper.x > r.value)
            upper.x = r.value;
        else if (r.parameter == 'x' && r.sign == '<' && lower.x < r.value)
            lower.x = r.value;
        else if (r.parameter == 'm' && r.sign == '>' && upper.m > r.value)
            upper.m = r.value;
        else if (r.parameter == 'm' && r.sign == '<' && lower.m < r.value)
            lower.m = r.value;
        else if (r.parameter == 'a' && r.sign == '>' && upper.a > r.value)
            upper.a = r.value;
        else if (r.parameter == 'a' && r.sign == '<' && lower.a < r.value)
            lower.a = r.value;
        else if (r.parameter == 's' && r.sign == '>' && upper.s > r.value)
            upper.s = r.value;
        else if (r.parameter == 's' && r.sign == '<' && lower.s < r.value)
            lower.s = r.value;
    }
}

ull part2 (std::vector<std::string> linesFromInputFile) {
    ull i = 0;
    std::vector<std::string> wf;
    while (linesFromInputFile[i] != "")
        wf.push_back(linesFromInputFile[i++]);
    std::vector<Workflow> workflows(wf.size());
    for (ull i = 0; i < wf.size(); i++) {
        for (ull j = 0; j < wf[i].size(); j++)
            if (wf[i][j] == '{' || wf[i][j] == ',' || wf[i][j] == '}')
                wf[i][j] = ' ';
        std::istringstream iss(wf[i]);
        iss >> workflows[i].name;
        std::string ruleStr;
        while (iss >> ruleStr) {
            if (ruleStr.size() == 1 || ruleStr[1] != '>' && ruleStr[1] != '<') { // for default 
                workflows[i].rules.push_back(Rule{0,0,0,ruleStr});
                break;
            }
            Rule r;
            r.parameter = ruleStr[0];
            r.sign = ruleStr[1];
            r.value = std::stoi(ruleStr.substr(2));
            ull j = 0;
            while (ruleStr[j++] != ':');
            r.nextWorkflow = ruleStr.substr(j);
            workflows[i].rules.push_back(r);
        }
    }
    std::vector<std::pair<Part,Part>> bounds;
    limit(workflows, bounds, Part{1,1,1,1}, Part{4000,4000,4000,4000}, "in");
    ull sum = 0;
    for (auto x : bounds)
        sum += (x.second.x - x.first.x + 1) * (x.second.m - x.first.m + 1) * (x.second.a - x.first.a + 1) * (x.second.s - x.first.s + 1);
    return sum;
}


int main () { 
    std::vector<std::string> linesFromInputFile = extractLinesFromInputFile("input.txt");
    ull part1result = part1(linesFromInputFile); std::cout << "\nPart 1: " << part1result << std::endl;
    ull part2result = part2(linesFromInputFile); std::cout << "\nPart 2: " << part2result << std::endl;
    return 0;
}

