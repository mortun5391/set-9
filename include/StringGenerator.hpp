#pragma once

#include <map>
#include <random>
#include <string>
#include <vector>

class StringGenerator {
public:
    static constexpr int MIN_LENGTH = 10;
    static constexpr int MAX_LENGTH = 200;
    static constexpr int MAX_ARRAY_SIZE = 3000;
    static constexpr int STEP = 100;
    static constexpr int ALPHABET_SIZE = 74;

    explicit StringGenerator(unsigned int seed = 20260524);

    std::map<std::string, std::vector<std::string>> generateMaxArrays();
    static std::vector<std::string> prefix(const std::vector<std::string>& data, int n);
    static const std::string& alphabet();

private:
    std::mt19937 rng;

    std::string randomString(int minLength = MIN_LENGTH, int maxLength = MAX_LENGTH);
    std::vector<std::string> randomArray(int n);
    std::vector<std::string> commonPrefixArray(int n);
    void makeNearlySorted(std::vector<std::string>& data);
};
