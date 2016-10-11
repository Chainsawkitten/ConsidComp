#include <iostream>
#include <chrono>
#include <cstring>

using namespace std;

/**
 * @brief The amount of leaf nodes in the tree.
 * 26^3 * 10^3
 */
const unsigned int leafNodeCount = 17576000;

/**
 * @brief The tree leaf nodes.
 */
bool leafNodes[leafNodeCount] = {};

/**
 * @brief Checks whether the file contains any duplicate entries.
 * Uses a flat tree structure (implemented in an array).
 * @param filename The name of the file to check.
 * @return Whether there were any duplicates.
 */
bool duplicates(const char* filename) {
    char buffer[8];
    
    FILE* file = fopen(filename, "rb");
    
    while (!feof(file)) {
        // Read next registration number.
        if (fread(buffer, 1, 8, file) != 8)
            break;
        
        // Find tree index.
        unsigned int index = 0;
        index += (buffer[0] - 'A') * 10 * 10 * 10 * 26 * 26;
        index += (buffer[1] - 'A') * 10 * 10 * 10 * 26;
        index += (buffer[2] - 'A') * 10 * 10 * 10;
        index += (buffer[3] - '0') * 10 * 10;
        index += (buffer[4] - '0') * 10;
        index += (buffer[5] - '0');
        
        // Check if number has already been marked.
        if (leafNodes[index]) {
            for (int i=0; i<6; ++i)
                putc(buffer[i], stdout);
            cout << endl;
            
            fclose(file);
            return true;
        }
        
        // Otherwise, mark it.
        leafNodes[index] = true;
    }
    
    fclose(file);
    
    return false;
}

/**
 * @brief Runs the main program.
 * Second command line argument should be the name of the data file to load.
 * @param argc The number of arguments given to the application.
 * @param argv The argument values.
 * @return Return code.
 */
int main(int argc, char** argv) {
    if (argc < 2) {
        return 1;
    }
    
    // Get start time.
    chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();;
    
    // Perform the program.
    cout << (duplicates(argv[1]) ? "Dubletter" : "Ej dubblett") << endl;
    
    // Output how long it took.
    double dt = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count();
    cout << "Took " << dt << " ms" << std::endl;
    
    return 0;
}
