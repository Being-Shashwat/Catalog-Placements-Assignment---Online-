
// Shamir Secret Sharing Simplified - Catalog Placements Assignment
// C++ Implementation

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <cmath>
#include <algorithm>
#include "json.hpp"



using namespace std;
using json = nlohmann::json;

// Convert value string in base to decimal
unsigned long long convertToDecimal(const string& value, int base) {
    unsigned long long result = 0;
    for (char digit : value) {
        int val;
        if (isdigit(digit)) val = digit - '0';
        else val = tolower(digit) - 'a' + 10;
        result = result * base + val;
    }
    return result;
}

// Lagrange Interpolation to find f(0) (i.e., constant c)
unsigned long long lagrangeInterpolation(vector<pair<int, unsigned long long>>& points) {
    unsigned long long secret = 0;
    int k = points.size();

    for (int i = 0; i < k; ++i) {
        long double term = points[i].second;
        for (int j = 0; j < k; ++j) {
            if (i != j) {
                term *= (0.0 - points[j].first) / (points[i].first - points[j].first);
            }
        }
        secret += static_cast<unsigned long long>(round(term));
    }
    return secret;
}

// Load test case from JSON file and decode roots
vector<pair<int, unsigned long long>> loadPoints(const string& filename, int& k_out) {
    ifstream file(filename);
    json j;
    file >> j;

    int n = j["keys"]["n"];
    k_out = j["keys"]["k"];

    vector<pair<int, unsigned long long>> points;
    for (auto& element : j.items()) {
        const string& key = element.key();
        const json& val = element.value();

        if (key == "keys") continue;

        int x = stoi(key);
        int base = stoi(val["base"].get<string>());
        string value_str = val["value"];
        unsigned long long y = convertToDecimal(value_str, base);
        points.emplace_back(x, y);
    }

    return points;
}


int main() {
    vector<pair<int, unsigned long long>> points1, points2;
    int k1, k2;

    // First test case
    points1 = loadPoints("testcase1.json", k1);
    vector<pair<int, unsigned long long>> chosen1(points1.begin(), points1.begin() + k1);
    unsigned long long secret1 = lagrangeInterpolation(chosen1);

    // Second test case
    points2 = loadPoints("testcase2.json", k2);
    vector<pair<int, unsigned long long>> chosen2(points2.begin(), points2.begin() + k2);
    unsigned long long secret2 = lagrangeInterpolation(chosen2);

    cout << "Secret from Testcase 1: " << secret1 << endl;
    cout << "Secret from Testcase 2: " << secret2 << endl;

    return 0;
}
