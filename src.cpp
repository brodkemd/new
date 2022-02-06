#include "./src.h"

/*
Removes the head directory from the inputted path string
*/
void remove_head(std::string& path) {
    int ind, stop = 0, count = 0, j = 0;

    // finds the first position of a linker char and counts how many "." 
    // there are up until the first linker char
    ind = path.find(linker);
    for (int i = ind - 1; i >= 0; i--) { if (path[i] == '.') count += 1; }

    /* counts how many characters the head path is
    exp: ./head/tail -> 6
    exp: head/tail -> 4
    */
    if (count == ind) {
         while (path[j] != *linker && !stop) {
            if (path[j] == *linker) stop = 1;
            j+=1;
        }
    }
    // if the path starts with either .. or . then it is removed
    path.erase(path.begin(), path.begin() + j + 1);

    // removes the head path name
    path.erase(path.begin(), path.begin() + path.find(linker) + 1);
}

/*
Uses the inputted path "source_dir" as a template to create a new directory tree
with all of the same files and structure in a directory with the path "destination_dir" + "replacement"

All occurrences of "to replace" in any file or name with be replaced in the new directory tree
*/
void templater(fs::path source_dir, fs::path destination_dir, std::string to_replace, std::string replacement) {
    // creates the correct destination name, could change this in the future
    destination_dir+=replacement;

    // if the destination path does not exist it is created
    if (!fs::exists(destination_dir)) fs::create_directory(destination_dir);
    
    // displaying a warning if the directory exists
    else {
        std::cout << "WARNING: The directory tree \"" << destination_dir.string() 
        << "\" exists. If you continue this directory tree could be damaged\nPress Enter to Continue(ctrl-c to stop):";
        std::cin.ignore();

    }

    std::fstream file;
    std::string contents, temp_path, line;
    fs::path new_path;

    // recursively iterates through the source directory getting its structure    
    for (const fs::directory_entry& dir_entry : std::filesystem::recursive_directory_iterator(source_dir)) {       
        // converting to path and replacing substring
        temp_path = dir_entry.path().string();

        // removes the head from the path
        remove_head(temp_path);
        boost::replace_all(temp_path, to_replace, replacement);
        
        // creates the proper new path
        new_path = destination_dir;
        new_path+=(linker + temp_path);
        
        std::cout << "Creating: " << new_path.string();

        // if the entry in the source tree is a directory
        if (fs::is_directory(dir_entry.path())) {
            fs::create_directory(new_path);
        }

        // if the entry is not a directory, only account for files right now
        else {
            // cleaning up the string thats stores file contents
            contents.clear();

            // opening the entry for reading
            file.open(dir_entry.path().string(), std::ios::in);
            if (file.is_open()) {
                // reading the file's contents
                while (getline(file, line)) contents += (line + "\n");
                
                // replacing the required strings
                boost::replace_all(contents, to_replace, replacement);
            }
            file.close();

            // creating a new entry at the same location in tree but in the destination directory
            file.open(new_path, std::ios::out);
            
            // writing the contents back to the file
            if (file.is_open()) file << contents;
            file.close();
        }        
    }
}


/*
loads local directories into a list of pairs, first one is the name/type and the second is the 
corresponding directory
*/
void load_local(std::vector<std::pair<std::string, std::string>>& options) {
    // clearing inputted list for good measure
    options.clear();
    std::pair<std::string, std::string> temp;
    fs::path path;
    // iterating through the contents of this file's directory
    for (auto const& dir_entry : std::filesystem::directory_iterator{INSTALL_DIR}) {
        path = dir_entry.path();
        // if the entry is a directory, and starts with the proper string define in src.h
        if (fs::is_directory(path) && path.string().find(TEMPLATE_INDICATOR) != ULLONG_MAX) {
            // getting the name of template type by removes the TEMPLATE indicator string from the directory name
            temp.first = path.string();
            temp.first.erase(0, temp.first.rfind(TEMPLATE_INDICATOR) + sizeof(TEMPLATE_INDICATOR)/sizeof(char) - 1);
            
            // getting the path of the absolute path of the entry
            temp.second = path.string();

            // adding it to the list
            options.push_back(temp);
        }
    }
}
