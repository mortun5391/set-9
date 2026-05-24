#include "StringSortTester.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <numeric>
#include <stdexcept>

using namespace std;

StringSortTester::StringSortTester(int runs) : runs(runs) {
    algorithms = {
        {"QUICKSORT", StringSortAlgorithms::quickSort},
        {"MERGESORT", StringSortAlgorithms::mergeSort},
        {"STRING_QUICKSORT", StringSortAlgorithms::stringQuickSort},
        {"STRING_MERGESORT_LCP", StringSortAlgorithms::lcpMergeSort},
        {"MSD_RADIX_SORT", StringSortAlgorithms::msdRadixSort},
        {"MSD_RADIX_SORT_WITH_STRING_QS", StringSortAlgorithms::msdRadixQuickSort}
    };
}

void StringSortTester::warmUp(const map<string, vector<string>>& arrays) {
    const vector<string>& data = arrays.at("random");
    vector<string> sample(data.begin(), data.begin() + 300);

    for (const auto& [name, sortFunction] : algorithms) {
        vector<string> copy = sample;
        SortMetrics metrics;
        sortFunction(copy, metrics);
    }
}

TestResult StringSortTester::benchmark(const string& dataset, int n, const string& algorithm,
                                       const SortFunction& sortFunction, const vector<string>& input) {
    vector<string> expected = input;
    sort(expected.begin(), expected.end());

    vector<double> times;
    times.reserve(runs);
    SortMetrics recordedMetrics;

    for (int run = 0; run < runs; run++) {
        vector<string> data = input;
        SortMetrics metrics;

        auto start = chrono::steady_clock::now();
        sortFunction(data, metrics);
        auto finish = chrono::steady_clock::now();

        if (data != expected) {
            throw runtime_error("Incorrect result in " + algorithm + " on " + dataset);
        }

        double microseconds = chrono::duration<double, micro>(finish - start).count();
        times.push_back(microseconds);
        if (run == 0) recordedMetrics = metrics;
    }

    vector<double> sortedTimes = times;
    sort(sortedTimes.begin(), sortedTimes.end());

    double average = accumulate(times.begin(), times.end(), 0.0) / times.size();
    double median = sortedTimes[sortedTimes.size() / 2];

    return {
        dataset,
        n,
        algorithm,
        average,
        sortedTimes.front(),
        median,
        recordedMetrics.charComparisons,
        recordedMetrics.charInspections,
        runs
    };
}

vector<TestResult> StringSortTester::runAll(const map<string, vector<string>>& arrays) {
    warmUp(arrays);
    vector<TestResult> result;

    for (const auto& [dataset, maxArray] : arrays) {
        for (int n = StringGenerator::STEP; n <= StringGenerator::MAX_ARRAY_SIZE; n += StringGenerator::STEP) {
            vector<string> input = StringGenerator::prefix(maxArray, n);
            for (const auto& [name, sortFunction] : algorithms) {
                result.push_back(benchmark(dataset, n, name, sortFunction, input));
            }
        }
    }

    return result;
}

void StringSortTester::saveCsv(const vector<TestResult>& results, const string& path) {
    ofstream out(path);
    if (!out) throw runtime_error("Cannot open CSV output: " + path);

    out << "dataset,n,algorithm,avg_time_us,min_time_us,median_time_us,char_comparisons,char_inspections,runs\n";
    out << fixed << setprecision(3);

    for (const TestResult& result : results) {
        out << result.dataset << ','
            << result.n << ','
            << result.algorithm << ','
            << result.avgTimeUs << ','
            << result.minTimeUs << ','
            << result.medianTimeUs << ','
            << result.charComparisons << ','
            << result.charInspections << ','
            << result.runs << '\n';
    }
}
