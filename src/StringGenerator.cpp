#include "StringGenerator.hpp"

#include <algorithm>

using namespace std;

StringGenerator::StringGenerator(unsigned int seed) : rng(seed) {}

const string& StringGenerator::alphabet() {
    // По условию алфавит содержит 74 символа; завершающая точка в тексте
    // условия трактуется как знак пунктуации, поэтому спецсимволов 12.
    static const string value =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789"
        "!@#%:;^&*()-";
    return value;
}

string StringGenerator::randomString(int minLength, int maxLength) {
    uniform_int_distribution<int> lengthDist(minLength, maxLength);
    uniform_int_distribution<int> charDist(0, (int)alphabet().size() - 1);

    int length = lengthDist(rng);
    string result;
    result.reserve(length);

    for (int i = 0; i < length; i++) {
        result += alphabet()[charDist(rng)];
    }

    return result;
}

vector<string> StringGenerator::randomArray(int n) {
    vector<string> result(n);
    for (string& s : result) {
        s = randomString();
    }
    return result;
}

vector<string> StringGenerator::commonPrefixArray(int n) {
    string sharedPrefix = randomString(70, 70);
    vector<string> result;
    result.reserve(n);

    for (int i = 0; i < n; i++) {
        result.push_back(sharedPrefix + randomString(10, MAX_LENGTH - (int)sharedPrefix.size()));
    }

    shuffle(result.begin(), result.end(), rng);
    return result;
}

void StringGenerator::makeNearlySorted(vector<string>& data) {
    sort(data.begin(), data.end());

    // В каждом блоке из 100 строк меняются местами две соседние строки.
    // Поэтому любой исследуемый префикс 100, 200, ..., 3000
    // остаётся почти отсортированным, а не случайно полностью отсортированным.
    uniform_int_distribution<int> positionDist(0, STEP - 2);
    for (int block = 0; block < (int)data.size(); block += STEP) {
        int pos = block + positionDist(rng);
        swap(data[pos], data[pos + 1]);
    }
}

map<string, vector<string>> StringGenerator::generateMaxArrays() {
    map<string, vector<string>> result;

    vector<string> base = randomArray(MAX_ARRAY_SIZE);
    result["random"] = base;

    result["reverse"] = base;
    sort(result["reverse"].begin(), result["reverse"].end(), greater<string>());

    result["nearly_sorted"] = base;
    makeNearlySorted(result["nearly_sorted"]);

    result["common_prefix"] = commonPrefixArray(MAX_ARRAY_SIZE);

    return result;
}

vector<string> StringGenerator::prefix(const vector<string>& data, int n) {
    return vector<string>(data.begin(), data.begin() + n);
}
