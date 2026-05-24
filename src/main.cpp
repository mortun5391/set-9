#include "StringGenerator.hpp"
#include "StringSortTester.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

void solve(int argc, char* argv[]) {
    string outputPath = "data/results.csv";
    string inputsDirectory = "data/generated_arrays";
    int runs = 7;
    unsigned int seed = 20260524;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--out" && i + 1 < argc) outputPath = argv[++i];
        else if (arg == "--inputs" && i + 1 < argc) inputsDirectory = argv[++i];
        else if (arg == "--runs" && i + 1 < argc) runs = stoi(argv[++i]);
        else if (arg == "--seed" && i + 1 < argc) seed = stoul(argv[++i]);
    }

    StringGenerator generator(seed);
    auto arrays = generator.generateMaxArrays();

    filesystem::create_directories(inputsDirectory);
    for (const auto& [name, data] : arrays) {
        ofstream out(filesystem::path(inputsDirectory) / (name + "_3000.txt"));
        out << data.size() << '\n';
        for (const string& value : data) out << value << '\n';
    }

    StringSortTester tester(runs);
    auto results = tester.runAll(arrays);

    filesystem::path path(outputPath);
    if (path.has_parent_path()) filesystem::create_directories(path.parent_path());
    StringSortTester::saveCsv(results, outputPath);

    cout << "Measurements completed: " << results.size() << '\n';
    cout << "CSV saved to: " << outputPath << '\n';
    cout << "Generated arrays saved to: " << inputsDirectory << '\n';
    cout << "Alphabet size: " << StringGenerator::alphabet().size() << '\n';
}

int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve(argc, argv);

    return 0;
}
