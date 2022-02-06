#ifndef SRC
#define SRC
    #include<iostream>
    #include<boost/algorithm/string.hpp>
    #include<fstream>
    #include<string>
    #include<vector>
    #include<filesystem>
    #include<climits>

    #define linker (char*)"/"
    #define TEMPLATE_INDICATOR "TEMPLATE_"
    #define INSTALL_DIR "/home/marek/Documents/C++/new/"

    namespace fs = std::filesystem;

    /*
    Uses the inputted path "source_dir" as a template to create a new directory tree
    with all of the same files and structure in a directory with the path "destination_dir" + "replacement"

    All occurrences of "to replace" in any file or name with be replaced in the new directory tree
    */
    void templater(fs::path source_dir, fs::path destination_dir, std::string to_replace, std::string replacement);

    /*
    loads local directories into a list of pairs, first one is the name/type and the second is the 
    corresponding directory
    */
    void load_local(std::vector<std::pair<std::string, std::string>>& options);

#endif