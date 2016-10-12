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
    // Read the entire file into RAM.
    FILE* file = fopen(filename, "rb");
    fseek(file, 0, SEEK_END);
    long length = ftell(file); 
    rewind(file);
    
    char* buffer = new char[length];
    fread(buffer, 1, length, file);
    fclose(file);
    
    // Loop through all the lines.
    for (long i=0; i<length; i+=8) {
        // Find tree index.
        unsigned int index = (buffer[i]   - 'A') * 10 * 10 * 10 * 26 * 26
                           + (buffer[i+1] - 'A') * 10 * 10 * 10 * 26
                           + (buffer[i+2] - 'A') * 10 * 10 * 10
                           + (buffer[i+3] - '0') * 10 * 10
                           + (buffer[i+4] - '0') * 10
                           + (buffer[i+5] - '0');
        
        // Check if number has already been marked.
        if (leafNodes[index])
            return true;
        
        // Otherwise, mark it.
        leafNodes[index] = true;
    }
    
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
    
    const int times = 10;
    
    // Get start time.
    chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();;
    
    // Perform the program.
    for (int i=0; i<times; ++i) {
        memset(leafNodes, 0, sizeof(bool) * leafNodeCount);
        cout << (duplicates(argv[1]) ? "Dubletter" : "Ej dubblett") << endl;
    }
    
    // Output how long it took.
    double dt = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count();
    cout << "Took " << dt << " ms (average " << dt / times << "ms)" << std::endl;
    
    return 0;
}
