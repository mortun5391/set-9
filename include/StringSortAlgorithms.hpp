#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct SortMetrics {
    std::uint64_t charComparisons = 0;
    std::uint64_t charInspections = 0;
};

class StringSortAlgorithms {
public:
    static constexpr int RADIX = 74;
    static constexpr int SOURCE_ALPHABET_SIZE = 74;

    static void quickSort(std::vector<std::string>& a, SortMetrics& metrics);
    static void mergeSort(std::vector<std::string>& a, SortMetrics& metrics);
    static void stringQuickSort(std::vector<std::string>& a, SortMetrics& metrics);
    static void lcpMergeSort(std::vector<std::string>& a, SortMetrics& metrics);
    static void msdRadixSort(std::vector<std::string>& a, SortMetrics& metrics);
    static void msdRadixQuickSort(std::vector<std::string>& a, SortMetrics& metrics);
};
