#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>
#include <sstream>

namespace fs = std::filesystem;

int search_word_in_file(const std::string& filepath, const std::string& target) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Could not open " << filepath << std::endl;
        return 0;
    }

    int count = 0;
    std::string line;
    int line_number = 0;

    while (std::getline(file, line)) {
        line_number++;

        std::istringstream iss(line);
        std::string word;
        bool found = false;

        while (iss >> word) {
            if (word == target) {
                count++;
                found = true;
            }
        }

        if (found) {
            std::cout << "Found in " << filepath << " at line " << line_number << std::endl;
        }
    }

    return count;
}

int main() {
    std::string folder_path = "./generated_files"; // Folder containing text files
    std::string target_word = "brandy"; // Word to search

    auto start = std::chrono::high_resolution_clock::now();

    int total_count = 0;
    for (const auto& entry : fs::directory_iterator(folder_path)) {
        if (entry.path().extension() == ".txt") {
            total_count += search_word_in_file(entry.path().string(), target_word);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Total occurrences of \"" << target_word << "\": " << total_count << std::endl;
    std::cout << "Time taken: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}
