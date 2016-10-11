#include <iostream>
#include <chrono>

using namespace std;

// Runs the main program.
// Second command line argument should be the name of the data file to load.
int main(int argc, char** argv) {
    if (argc < 2) {
        return 1;
    }
    
    // Get start time.
    chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();;
    
    // Perform the program.
    cout << argv[1] << endl;
    
    // Output how long it took.
    double dt = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count();
    cout << "Took " << dt << " ms" << std::endl;
    
    return 0;
}
