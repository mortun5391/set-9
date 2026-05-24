#include "StringSortAlgorithms.hpp"

#include <algorithm>
#include <array>
#include <utility>

using namespace std;

namespace {

int compareStrings(const string& a, const string& b, int start, SortMetrics& metrics) {
    int i = start;
    while (i < (int)a.size() && i < (int)b.size()) {
        metrics.charComparisons++;
        metrics.charInspections += 2;
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
        i++;
    }

    if (a.size() == b.size()) return 0;
    return a.size() < b.size() ? -1 : 1;
}

int charAt(const string& s, int d, SortMetrics& metrics) {
    if (d == (int)s.size()) return -1;
    metrics.charInspections++;
    return (unsigned char)s[d];
}

int radixCharAt(const string& s, int d, SortMetrics& metrics) {
    if (d == (int)s.size()) return -1;
    metrics.charInspections++;

    static const array<int, 128> rank = [] {
        array<int, 128> result{};
        result.fill(-1);
        const string ordered = "!#%&()*-0123456789:;@ABCDEFGHIJKLMNOPQRSTUVWXYZ^abcdefghijklmnopqrstuvwxyz";
        for (int i = 0; i < (int)ordered.size(); i++) result[(unsigned char)ordered[i]] = i;
        return result;
    }();

    return rank[(unsigned char)s[d]];
}

void quickSortRec(vector<string>& a, int l, int r, SortMetrics& metrics) {
    if (l >= r) return;

    int pivotIndex = l + (r - l) / 2;
    swap(a[l], a[pivotIndex]);
    string pivot = a[l];

    int lt = l, gt = r;
    int i = l + 1;

    while (i <= gt) {
        int cmp = compareStrings(a[i], pivot, 0, metrics);
        if (cmp < 0) {
            swap(a[lt++], a[i++]);
        } else if (cmp > 0) {
            swap(a[i], a[gt--]);
        } else {
            i++;
        }
    }

    quickSortRec(a, l, lt - 1, metrics);
    quickSortRec(a, gt + 1, r, metrics);
}

void mergeSortRec(vector<string>& a, vector<string>& tmp, int l, int r, SortMetrics& metrics) {
    if (r - l <= 1) return;

    int m = (l + r) / 2;
    mergeSortRec(a, tmp, l, m, metrics);
    mergeSortRec(a, tmp, m, r, metrics);

    int i = l, j = m, pos = l;
    while (i < m && j < r) {
        if (compareStrings(a[i], a[j], 0, metrics) <= 0) {
            tmp[pos++] = move(a[i++]);
        } else {
            tmp[pos++] = move(a[j++]);
        }
    }
    while (i < m) tmp[pos++] = move(a[i++]);
    while (j < r) tmp[pos++] = move(a[j++]);

    for (int p = l; p < r; p++) {
        a[p] = move(tmp[p]);
    }
}

void stringQuickSortRec(vector<string>& a, int l, int r, int d, SortMetrics& metrics) {
    if (l >= r) return;

    int pivotIndex = l + (r - l) / 2;
    swap(a[l], a[pivotIndex]);
    int pivot = charAt(a[l], d, metrics);

    int lt = l, gt = r;
    int i = l + 1;

    while (i <= gt) {
        int cur = charAt(a[i], d, metrics);
        if (pivot != -1 && cur != -1) {
            metrics.charComparisons++;
        }

        if (cur < pivot) {
            swap(a[lt++], a[i++]);
        } else if (cur > pivot) {
            swap(a[i], a[gt--]);
        } else {
            i++;
        }
    }

    stringQuickSortRec(a, l, lt - 1, d, metrics);
    if (pivot != -1) {
        stringQuickSortRec(a, lt, gt, d + 1, metrics);
    }
    stringQuickSortRec(a, gt + 1, r, d, metrics);
}

struct LcpComparison {
    int cmp = 0;
    int lcp = 0;
};

LcpComparison lcpCompare(const string& a, const string& b, int known, SortMetrics& metrics) {
    int i = known;
    while (i < (int)a.size() && i < (int)b.size()) {
        metrics.charComparisons++;
        metrics.charInspections += 2;
        if (a[i] < b[i]) return {-1, i};
        if (a[i] > b[i]) return {1, i};
        i++;
    }

    if (a.size() == b.size()) return {0, i};
    return {a.size() < b.size() ? -1 : 1, i};
}

void lcpMerge(vector<string>& a, vector<string>& tmp, vector<int>& lcp,
              vector<int>& tmpLcp, int l, int m, int r, SortMetrics& metrics) {
    int i = l, j = m, pos = l;
    int leftLcp = 0, rightLcp = 0;

    while (i < m && j < r) {
        if (leftLcp > rightLcp) {
            tmp[pos] = move(a[i]);
            tmpLcp[pos++] = leftLcp;
            i++;
            if (i < m) leftLcp = lcp[i];
        } else if (rightLcp > leftLcp) {
            tmp[pos] = move(a[j]);
            tmpLcp[pos++] = rightLcp;
            j++;
            if (j < r) rightLcp = lcp[j];
        } else {
            LcpComparison comparison = lcpCompare(a[i], a[j], leftLcp, metrics);
            if (comparison.cmp <= 0) {
                tmp[pos] = move(a[i]);
                tmpLcp[pos++] = leftLcp;
                i++;
                rightLcp = comparison.lcp;
                if (i < m) leftLcp = lcp[i];
            } else {
                tmp[pos] = move(a[j]);
                tmpLcp[pos++] = rightLcp;
                j++;
                leftLcp = comparison.lcp;
                if (j < r) rightLcp = lcp[j];
            }
        }
    }

    while (i < m) {
        tmp[pos] = move(a[i]);
        tmpLcp[pos++] = leftLcp;
        i++;
        if (i < m) leftLcp = lcp[i];
    }
    while (j < r) {
        tmp[pos] = move(a[j]);
        tmpLcp[pos++] = rightLcp;
        j++;
        if (j < r) rightLcp = lcp[j];
    }

    tmpLcp[l] = 0;
    for (int p = l; p < r; p++) {
        a[p] = move(tmp[p]);
        lcp[p] = tmpLcp[p];
    }
}

void lcpMergeSortRec(vector<string>& a, vector<string>& tmp, vector<int>& lcp,
                     vector<int>& tmpLcp, int l, int r, SortMetrics& metrics) {
    if (r - l <= 1) {
        if (r - l == 1) lcp[l] = 0;
        return;
    }

    int m = (l + r) / 2;
    lcpMergeSortRec(a, tmp, lcp, tmpLcp, l, m, metrics);
    lcpMergeSortRec(a, tmp, lcp, tmpLcp, m, r, metrics);
    lcpMerge(a, tmp, lcp, tmpLcp, l, m, r, metrics);
}

void msdRadixSortRec(vector<string>& a, vector<string>& tmp, int l, int r, int d,
                     bool useCutoff, SortMetrics& metrics) {
    if (l >= r) return;
    if (useCutoff && r - l + 1 < StringSortAlgorithms::SOURCE_ALPHABET_SIZE) {
        stringQuickSortRec(a, l, r, d, metrics);
        return;
    }

    array<int, StringSortAlgorithms::RADIX + 2> count{};
    for (int i = l; i <= r; i++) {
        int c = radixCharAt(a[i], d, metrics);
        count[c + 2]++;
    }

    for (int i = 0; i < StringSortAlgorithms::RADIX + 1; i++) {
        count[i + 1] += count[i];
    }
    array<int, StringSortAlgorithms::RADIX + 2> bounds = count;

    for (int i = l; i <= r; i++) {
        int c = radixCharAt(a[i], d, metrics);
        tmp[l + count[c + 1]++] = move(a[i]);
    }

    for (int i = l; i <= r; i++) {
        a[i] = move(tmp[i]);
    }

    for (int c = 0; c < StringSortAlgorithms::RADIX; c++) {
        int from = l + bounds[c + 1];
        int to = l + bounds[c + 2] - 1;
        if (from < to) {
            msdRadixSortRec(a, tmp, from, to, d + 1, useCutoff, metrics);
        }
    }
}

} // namespace

void StringSortAlgorithms::quickSort(vector<string>& a, SortMetrics& metrics) {
    if (!a.empty()) quickSortRec(a, 0, (int)a.size() - 1, metrics);
}

void StringSortAlgorithms::mergeSort(vector<string>& a, SortMetrics& metrics) {
    vector<string> tmp(a.size());
    mergeSortRec(a, tmp, 0, (int)a.size(), metrics);
}

void StringSortAlgorithms::stringQuickSort(vector<string>& a, SortMetrics& metrics) {
    if (!a.empty()) stringQuickSortRec(a, 0, (int)a.size() - 1, 0, metrics);
}

void StringSortAlgorithms::lcpMergeSort(vector<string>& a, SortMetrics& metrics) {
    if (a.empty()) return;
    vector<string> tmp(a.size());
    vector<int> lcp(a.size()), tmpLcp(a.size());
    lcpMergeSortRec(a, tmp, lcp, tmpLcp, 0, (int)a.size(), metrics);
}

void StringSortAlgorithms::msdRadixSort(vector<string>& a, SortMetrics& metrics) {
    if (a.empty()) return;
    vector<string> tmp(a.size());
    msdRadixSortRec(a, tmp, 0, (int)a.size() - 1, 0, false, metrics);
}

void StringSortAlgorithms::msdRadixQuickSort(vector<string>& a, SortMetrics& metrics) {
    if (a.empty()) return;
    vector<string> tmp(a.size());
    msdRadixSortRec(a, tmp, 0, (int)a.size() - 1, 0, true, metrics);
}
