#ifndef huffman_hpp
#define huffman_hpp
#include<string>
#include<vector>
#include<fstream>
#include<queue>
using namespace std;

//defining the node for the huffman tree 
//which will basically contains like char,int freq ,string code
struct Node{
    char data;
    unsigned int freq;
    string code;
    Node* left,right;

    Node(){
        left=right=NULL;
    }
};

class huffman{
    protected:
        vector<Node*>freqarr;
        fstream infile,outfile;
        string inFileName,outFileName;

        Node* root;

        class Compare{
            public:
                bool operator()(Node* l,Node* r){
                    return l->freq>r->freq;
                }
        };
        
        priority_queue<Node*,vector<Node*>,Compare>minHeap;

        //for initializing the freqarr 
        void inifreqarr();

        //for like traversin the  constructed nodes for each character to get respective code..
        void traverse(Node*,string);

        //function to convert the binary string to its eqivalent decimal value
        int binToDec(string);

        string decToBinary(int);

        //reconstructing the binary tree while decoding the binary fiile/
        void buildTree(char,string&);

        void createMinHeap();

        void createTree();

        void createCodes();

        void savedEncodedFile();

        void saveDecodedFile();

        void getTree();

    public:
        //constructor for classs
        huffman(string inFileName,string outFileName){
            this->inFileName=inFileName;
            this->outFileName=outFileName;
            create();
        }
        void compress();
        void decompress();
};


#endif