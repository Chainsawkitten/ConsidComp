#include <iostream>
#include <chrono>

using namespace std;

/**
 * @brief Checks whether the file contains any duplicate entries.
 * Uses a flat tree structure (implemented in an array).
 * @param filename The name of the file to check.
 * @return Whether there were any duplicates.
 */
bool duplicates(const char* filename) {
    /// @todo Implement the actual function.
    
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
