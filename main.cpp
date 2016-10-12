#include <iostream>
#include <chrono>
#include <cstring>
#include <thread>
#include <functional>
#include <atomic>

using namespace std;

/**
 * @brief The amount of leaf nodes in the tree.
 * 26^3 * 10^3
 */
const unsigned int leafNodeCount = 17576000;

/**
 * @brief The tree leaf nodes.
 */
unsigned char leafNodes[leafNodeCount] = {};

/**
 * @brief Whether a duplicate has been found.
 */
bool duplicateFound = false;

/**
 * @brief Which threads are done executing.
 */
bool threadDone[3];

/**
 * @brief Checks whether there are any duplicates in a subset of the buffer.
 * @param buffer File buffer.
 * @param startPos Position in the buffer to start at.
 * @param ensPos Position in the buffer to end at.
 * @param threadIndex The index of the executing thread.
 */
void duplicateThread(const char* buffer, long startPos, long endPos, unsigned char threadIndex) {
    // Loop through all the lines.
    for (long i = startPos; i < endPos; i += 8) {
        // Find tree index.
        unsigned int index = (buffer[i]   - 'A') * 10 * 10 * 10 * 26 * 26
                           + (buffer[i+1] - 'A') * 10 * 10 * 10 * 26
                           + (buffer[i+2] - 'A') * 10 * 10 * 10
                           + (buffer[i+3] - '0') * 10 * 10
                           + (buffer[i+4] - '0') * 10
                           + (buffer[i+5] - '0');
        
        // Mark tree node as our index.
        leafNodes[index] = threadIndex;
    }
    
    // Signal that we're done.
    threadDone[threadIndex] = true;
    
    // Wait for all threads to finish.
    for (unsigned char i=0; i<3; ++i) {
        while (!threadDone[i])
            this_thread::yield();
    }
    
    // Check if someone has modified our tree nodes.
    for (long i = startPos; i < endPos; i += 8) {
        // Find tree index.
        unsigned int index = (buffer[i]   - 'A') * 10 * 10 * 10 * 26 * 26
                           + (buffer[i+1] - 'A') * 10 * 10 * 10 * 26
                           + (buffer[i+2] - 'A') * 10 * 10 * 10
                           + (buffer[i+3] - '0') * 10 * 10
                           + (buffer[i+4] - '0') * 10
                           + (buffer[i+5] - '0');
        
        // Check if someone has modified our tree node.
        if (leafNodes[index] != threadIndex) {
            duplicateFound = true;
            return;
        }
    }
}

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
    
    // Check for duplicates.
    duplicateFound = false;
    long len2 = length / 8;
    long len3 = len2 / 3 * 8;
    thread thread1(std::bind(&duplicateThread, buffer, 0, len3, 0));
    thread thread2(std::bind(&duplicateThread, buffer, len3, len3 * 2, 1));
    thread thread3(std::bind(&duplicateThread, buffer, len3 * 2, length, 2));
    
    // Clean up and return results.
    thread1.join();
    thread2.join();
    thread3.join();
    delete[] buffer;
    return duplicateFound;
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
    
    const int times = 20;
    
    // Get start time.
    chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();;
    
    // Perform the program.
    for (int i=0; i<times; ++i) {
        memset(leafNodes, 0, sizeof(bool) * leafNodeCount);
        memset(threadDone, 0, sizeof(unsigned char) * 3);
        cout << (duplicates(argv[1]) ? "Dubletter" : "Ej dubblett") << endl;
    }
    
    // Output how long it took.
    double dt = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count();
    cout << "Took " << dt << " ms (average " << dt / times << "ms)" << std::endl;
    
    return 0;
}
