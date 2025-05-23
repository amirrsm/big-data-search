#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <sstream>
#include <chrono>
#include <omp.h>
#include <mutex>

namespace fs = std::filesystem;

std::mutex print_mutex;

int search_word_in_file(const std::string& filepath, const std::string& target) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::lock_guard<std::mutex> lock(print_mutex);
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
            int thread_id = omp_get_thread_num();
            std::lock_guard<std::mutex> lock(print_mutex);
            std::cout << "[Thread " << thread_id << "] Found in " << filepath << " at line " << line_number << std::endl;
        }
    }

    return count;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <target_word> <num_threads>" << std::endl;
        return 1;
    }

    std::string folder_path = "./generated_files";
    std::string target_word = argv[1];
    int num_threads = std::stoi(argv[2]);

    std::vector<std::string> files;
    for (const auto& entry : fs::directory_iterator(folder_path)) {
        if (entry.path().extension() == ".txt") {
            files.push_back(entry.path().string());
        }
    }

    int total_count = 0;

    auto start = std::chrono::high_resolution_clock::now();

    omp_set_num_threads(num_threads);
    #pragma omp parallel for reduction(+:total_count) schedule(dynamic)
    for (int i = 0; i < files.size(); ++i) {
        int value = search_word_in_file(files[i], target_word);
        #pragma omp atomic
        total_count += value;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "\nTotal occurrences of \"" << target_word << "\": " << total_count << std::endl;
    std::cout << "Time taken with OpenMP (" << num_threads << " threads): " << elapsed.count() << " seconds" << std::endl;

    return 0;
}
