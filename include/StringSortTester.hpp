#pragma once

#include "StringGenerator.hpp"
#include "StringSortAlgorithms.hpp"

#include <functional>
#include <map>
#include <string>
#include <vector>

struct TestResult {
    std::string dataset;
    int n = 0;
    std::string algorithm;
    double avgTimeUs = 0;
    double minTimeUs = 0;
    double medianTimeUs = 0;
    std::uint64_t charComparisons = 0;
    std::uint64_t charInspections = 0;
    int runs = 0;
};

class StringSortTester {
public:
    using SortFunction = std::function<void(std::vector<std::string>&, SortMetrics&)>;

    explicit StringSortTester(int runs = 7);

    std::vector<TestResult> runAll(const std::map<std::string, std::vector<std::string>>& arrays);
    static void saveCsv(const std::vector<TestResult>& results, const std::string& path);

private:
    int runs;
    std::vector<std::pair<std::string, SortFunction>> algorithms;

    TestResult benchmark(const std::string& dataset, int n, const std::string& algorithm,
                         const SortFunction& sortFunction, const std::vector<std::string>& input);
    void warmUp(const std::map<std::string, std::vector<std::string>>& arrays);
};
