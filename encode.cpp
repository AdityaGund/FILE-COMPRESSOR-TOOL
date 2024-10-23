#include<iostream>
#include "huffman.hpp"
#include<chrono>

using namespace  std;

int main(int argc,char* argv[]){
    if(argc!=3){
        cout<<"Failed to detect Files";
        exit(1);
    }
    huffman f(argv[1],argv[2]);
    auto start_time = std::chrono::high_resolution_clock::now();
    f.compress();
        auto end_time = std::chrono::high_resolution_clock::now();

    // Calculate duration
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    // Output the duration
    std::cout << "Compression time: " << duration.count() << " milliseconds" << std::endl;
    cout<<"Compressed successfully"<<endl;
    return 0;
}