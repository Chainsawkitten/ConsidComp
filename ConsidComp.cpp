#include <iostream>
#include <thread>
#include <functional>

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
* @brief Buffer that the data file is read into.
*/
char* buffer;

/**
* @brief Whether a duplicate has been found.
*/
bool duplicateFound = false;

/**
* @brief Indices of the leaf nodes.
*/
unsigned int* indices;

/**
* @brief Which threads are done executing.
*/
bool threadDone[8] = {};

/**
* @brief Checks whether there are any duplicates in a subset of the buffer.
* @param startPos Position in the buffer to start at.
* @param endPos Position in the buffer to end at.
* @param threadIndex The index of the executing thread.
*/
void duplicateThread(long startPos, long endPos, unsigned char threadIndex) {
    // Loop through all the lines.
    for (long i = startPos; i < endPos; ++i) {
        // Find tree index.
        long bufferIndex = i * 8;
        indices[i] = (buffer[bufferIndex]     - 'A') * 10 * 10 * 10 * 26 * 26
                   + (buffer[bufferIndex + 1] - 'A') * 10 * 10 * 10 * 26
                   + (buffer[bufferIndex + 2] - 'A') * 10 * 10 * 10
                   + (buffer[bufferIndex + 3] - '0') * 10 * 10
                   + (buffer[bufferIndex + 4] - '0') * 10
                   + (buffer[bufferIndex + 5] - '0');
        
        // Check if this thread has already encountered it.
        if (leafNodes[indices[i]]) {
            threadDone[threadIndex] = true;
            duplicateFound = true;
            return;
        }
        
        // Mark tree node as our index.
        leafNodes[indices[i]] = threadIndex;
    }
    
    // Signal that we're done.
    threadDone[threadIndex] = true;
    
    // Wait for all threads to finish.
    for (unsigned char i = 0; i<8; ++i) {
        while (!threadDone[i])
            this_thread::yield();
    }
    
    // Check if someone has modified our tree nodes.
    for (long i = startPos; i < endPos; ++i) {
        // Check if someone has modified our tree node.
        if (leafNodes[indices[i]] != threadIndex) {
            duplicateFound = true;
            return;
        }
    }
}

/**
* @brief Runs the main program.
* Second command line argument should be the name of the data file to load.
* @param argc The number of arguments given to the application.
* @param argv The argument values.
* @return Return code.
*/
int main(int argc, char** argv) {
    // Disable argument checking for speed.
    //if (argc < 2) {
    //	cout << "No input file given." << endl;
    //	return 0;
    //}
    
    // Clear whether threads have finished between runs.
    // Can be skipped assuming this is run as a program rather than as a function.
    //memset(leafNodes, 0, sizeof(unsigned char) * leafNodeCount);
    //memset(threadDone, 0, sizeof(bool) * 8);
    
    // Read the entire file into RAM.
    FILE* file = fopen(argv[1], "rb");
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);
    
    buffer = new char[length];
    fread(buffer, 1, length, file);
    fclose(file);
    
    // Check for duplicates.
    duplicateFound = false;
    long len2 = length / 8;
    long lengthPerThread = len2 / 8;
    indices = new unsigned int[len2];
    
    // Create 8 threads (manual loop unrolling).
    thread thread1(std::bind(&duplicateThread, 0, lengthPerThread, 1));
    thread thread2(std::bind(&duplicateThread, lengthPerThread, lengthPerThread * 2, 2));
    thread thread3(std::bind(&duplicateThread, lengthPerThread * 2, lengthPerThread * 3, 3));
    thread thread4(std::bind(&duplicateThread, lengthPerThread * 3, lengthPerThread * 4, 4));
    thread thread5(std::bind(&duplicateThread, lengthPerThread * 4, lengthPerThread * 5, 5));
    thread thread6(std::bind(&duplicateThread, lengthPerThread * 5, lengthPerThread * 6, 6));
    thread thread7(std::bind(&duplicateThread, lengthPerThread * 6, lengthPerThread * 7, 7));
    thread thread8(std::bind(&duplicateThread, lengthPerThread * 7, len2, 8));
    
    // Clean up and return results.
    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();
    thread6.join();
    thread7.join();
    thread8.join();
    
    // Leak memory for speed (the heap will be released when the program exits anyhow).
    //delete[] buffer;
    //delete[] indices;
    
    cout << (duplicateFound ? "Dubbletter" : "Ej dubblett") << endl;
    
    return 0;
}
