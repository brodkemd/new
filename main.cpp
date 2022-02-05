#include<iostream>
#include<boost/algorithm/string.hpp>
#include<fstream>
#include<string>
#include<vector>
#include<filesystem>

namespace fs = std::filesystem;

std::string to_replace, replacement;

/*
creates a new directory tree based on the structure of "source_dir" at "destination_dir"
*/

void remove_head(std::string& path) {
    int ind, count, j = 0;
    bool stop = false;
    ind = path.find('/');
    for (int i = ind - 1; i >= 0; i--) {
        if (path[i] == '.') count += 1;
    }
    if (count == ind) {
        // HRERERERERERE
        while (path[j] != (const char *)"/" && !stop) {
            if (path[j] == "/") stop = true;
            j+=1;
        }
    }
    path.erase(path.begin(), path.begin() + j);
}

void templater(fs::path source_dir, fs::path destination_dir, std::string to_replace, std::string replacement) {
    // if the destination path does not exist it is created
    if (!fs::exists(destination_dir)) {
        std::cout << "Making: \"" << destination_dir.string() << "\"\n";
        fs::create_directory(destination_dir);
    }

    std::cout << "Using: \"" << source_dir.string() << "\" as a template\n";
    std::cout << "Replacing: \"" << to_replace << "\" with \"" << replacement << "\" in the above directory tree\n\n";

    std::fstream file;
    std::string contents, temp_path;
    fs::path new_path;

    // recursively iterates through the source directory getting its structure    
    for (const fs::directory_entry& dir_entry : std::filesystem::recursive_directory_iterator(source_dir)) {
        // cleaning up the string
        contents.clear();
        
        // converting to path and replacing substring
        temp_path = dir_entry.path().string();
        boost::replace_all(temp_path, to_replace, replacement);
        new_path = temp_path;
        
        std::cout << "Creating: " << new_path.string();
        
        // creating the appropropriate entry type
        // if the entry in the source tree is a directory
        if (fs::is_directory(dir_entry.path())) {
            fs::create_directory(new_path); // making it
            std::cout << " -> directory\n";
        }
        // if the entry is not a directory
        else {
            std::cout << " -> file\n";
            // reading the data from the entry
            file.open(dir_entry.path().string(), std::ios::in);
            if (file.is_open()) {
                file >> contents;

                // replacing the required strings
                boost::replace_all(contents, to_replace, replacement);
            }
            file.close();

            // creating a new entry at the same location in tree but in the destination directory
            file.open(new_path, std::ios::out);
            if (file.is_open()) file << contents;
            file.close();
        }        
    }
}

void load_local(std::vector<std::pair<std::string, std::string>>& options) {
    options.clear();
    std::pair<std::string, std::string> temp;
    temp = {"c++", "TEMPLATE_CPP"};
    options.push_back(temp);
}

int main(int argc, char* argv[]) {
    argc-=1;
    
    // converts input arguments to vector of strings for simplicity
    std::vector<std::string> input;
    for (int i = 0; i < argc; i++) {
        std::string temp(argv[i + 1]);
        input.push_back(temp);
    }

    if (input.size() == 3) {
        std::vector<std::pair<std::string, std::string>> options;
        for (int i = 0; i < options.size(); i++) {
            if (options[i].first == input.front()) {
                templater(options[i].second, input.front(), options[i].second, input.at(1));
                return 0;
            }
        }
        std::cout << "Invalid name for pre-made template\n";
    }
    else if (input.size() == 4) templater(input.front(), input.at(1), input.at(2), input.at(3));
    else std::cout << "Invalid inputs\n";

    return 0;
}