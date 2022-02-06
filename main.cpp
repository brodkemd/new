#include "./src.h"

/*
need to remove install directory from path
*/


int main(int argc, char* argv[]) {
    // removes the command string from the number of inputs
    argc-=1;
    
    // converts input arguments to vector of strings for simplicity
    std::vector<std::string> input;
    for (int i = 0; i < argc; i++) {
        std::string temp(argv[i + 1]);

        // if input was a relative path and did not include the "/" at the end it is added
        if (temp == "." || temp == "..") temp+=linker;
        input.push_back(temp);
    }

    // 2 inputs indicate to use premade templates
    if (input.size() == 2) {
        // creates a list of pairs to get data on what template directories there are in this file's local directory
        std::vector<std::pair<std::string, std::string>> options;
        
        // function that loads template directories into the list of pairs
        load_local(options);

        // goes through the list of template directories and finds a match
        for (int i = 0; i < options.size(); i++) {
            std::cout << options[i].first << "\n";
            if (options[i].first == input.front()) {
                // executes the templater with the configuration from the list
                templater(options[i].second, (std::string)"." + linker, TEMPLATE_INDICATOR + options[i].first, input.at(1));
                return 0;
            }
        }
        // if here, did not find in list
        std::cout << "Invalid name for pre-made template\nOptions are:";

        // prints the options
        for (int i = 0; i < options.size(); i++) std::cout << options[i].first << "\n";
    }
    // 4 inputs indicates to use local directory
    else if (input.size() == 4) templater(input.front(), input.at(1), input.at(2), input.at(3));
    else std::cout << "Invalid inputs\n";

    return 0;
}