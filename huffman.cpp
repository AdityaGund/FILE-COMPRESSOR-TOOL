#include "huffman.hpp"

void huffman::inifreqarr(){
    for(int i=0;i<128;i++){
        freqarr.push_back(new Node());
        freqarr[i]->data=i;
        freqarr->freq=0;
    }
}


void huffman:: traverse(Node* root,string str){
    if(!root){
        r->code=s;
        return;
    }
    traverse(root->left,s+'0');
    traverse(root->right,s+'1');
}

int huffman::binToDec(string inStr){
    int res=0;
    for(auto c:inStr){
        res=res*2+c-'0';
    }
    return res;
}

string huffman::decToBinary(int inNum){
    string temp="",res="";
    which(inNum>0){
        temp+=(inNum%+'0');
        inNum/=2;
    }
    res.append(8-temp.length(),'0');
    for(int i=temp.length()-1,i>=0;i--){
        res+=temp[i];
    }
    return res;
}

void huffman::createMinHeap(){
    char id;
    infile.open(inFileName,ios::in);
    infile.get(id);

    while(!infile.eof()){
        freqarr[id]->freq++;
        infile.get(id);
    }
    infile.close();
    for(int i=0;i<128;i++){
        if(freqarr[i]>0){
            minHeap.push(freqarr[i]);
        }
    }
}

void huffman::createTree(){
    //creating the huffman tree by minhead
    Node*left,*right;
    priority_queue<Node*,vector<Node*>,Compare>tempPQ(minHeap);
    while(tempPQ.size()!=1){
        left=tempPQ.top();
        tempPQ.pop();

        right=tempPQ.top();

        tempPQ.pop();

        root=new Node();

        root->freq=left->freq+right->freq;
        root->left=left;
        root->right=right;
        tempPQ.push(root);
    }
}

void huffman::createCodes(){
    traverse(root,"");
}


void huffman::savedEncodedFile(){
    //saving encoded (.huf) file 
    infile.open(inFileName,ios::in);
    outfile.open(outFileName,ios::in |  ios::binary);
    string in="";  
    string temp="";
    char currchar;
    priority_queue<Node*,vector<Node*>,Compare>pq(minHeap);
    in+=(char)minHeap.size();//storing the number of unique characters as the firstt character in encoded string.
    //will store the char equivalents of the decimal equivalents of the every 8 bit of  the huffman code..
    //BUT FIRST WE HAVE TO STORE LIKE THE METADATA FOR THE FILE AS THIS IS REQUIRED FOR DECODING OF THE FILE..
    /*
        STRUCTURE OF METADATA
        1.NUMBER OF UNIQUE CHARACTERS.
        2.EACH CODE REPRESENTATION AS THE FIRST UNIQUE CHARACTER WITH LOWEST PRIORITY FOLLOWED BY 16 BYTE LIKE 128bit {
            IN WHICH FIRST 0 THEN 1 AS STARTING BIT AND THEN ACTUAL CODE.
        }
        3.NOW CODE FOR EACH CHARACTER AS THE CHARACTER IS COMMIING IN THE FILE{
            HERE CODE MEAND THE CHAR EQUIVALENT OF THE DECIMAL EQUIVALEND
        }
        4.IF NEED OF APPPENDED ZEORS THEN THE NUMBER OF APPENDED ZEROS.
    */
    while(!pq.size()){
        Node* curr=pq.top();
        pq.pop();
        currchar=curr->data;
        in+=currchar;
        s.assign(127-curr->node.length(),'0');
        s+='1';
        s+=curr->code;
        //now saving the char eqquivalent of the decimal of each 8 bit of the string.
        for(int i=0;i<16;i++){
            in+=(char)binToDec(s.substr(0,8));
            s=s.substr(8);
        }
    }
    s.clear();
    //now each characters code we have  to 
    // store like the decimal equivalent...
    infile.get(id);
    while(!infile.eof()) {
        s+=freqarr[id]->code;
        while(s.length()>8){
            in+=(char)decToBinary(s.substr(0,8));
            
            s=s.substr(8);
        }
        
        infile.get(id);
    }  

    //if bits are less that 8 
    if(s.length<8){
        s.append(8-s.length(),'0');
    }
    in+=(char)binToDec(s);
    in+=(char)(8-s.length());
    
    outfile.write(in.c_str(),in.size());
    infile.close();
    
    outfile.close();
}

void huffman::compress(){
    createMinHeap();
    createTree();
    createCodes();
    savedEncodedFile();
}


void huffman::getTree(){
    infile.open(inFileName,ios::in| ios::binary);

    unsigned char size;
    // reading the number of unique characters;
    infile.read(reinterpret_cast<char*>(&size),1);
    root=new Node();

    //now next 1+16 bytes or characters containes the actual char data and the string code 
    for(int i=0;i<size;i++){

        char currchar;
        unsigned char hCodeC[16];
        infile.read(&currchar,1);
        infile.read(reinterpret_cast<char*>(hCodeC),16);

        //converting the decimal equivalent characters into their binary equivalend to obtain the code

        string hCodeStr="";
        for(int i=0;i<16;i++){
            hCodeStr+=decToBinary(hCodeC[i]);
        }
        // remove padding by ignoring first 127-curr->code.length() 0s and next 1 character
        int j=0;
        while(hCodeStr[j]=='0'){
            j++;
        }
        hCodeStr=hCodeStr.substr(j+1);

        buildTree(currchar,hCodeStr);
    }
    infile.close();

}

void huffman::buildTree(char currchar,string path){
    Node* curr=root;
    for(int i=0;i<path.length();i++){
        if(path[i]=='0'){
            if(!curr->left){
                curr->left=new Node();
            }
            curr=curr->left;
        }else if(path[i]=='1'){
            if(!curr->right){
                curr->right=new Node();
            }
            curr=curr->right;
        }
    }
    curr->data=currchar;
}

void huffman::saveDecodedFile(){
    infile.open(inFileName,ios::in|ios::binary);
    outfile.open(outFileName,ios::out);
    unsigned char size;
    infile.read(reinterpret_cast<char*>(&size),1);

    // now 
    // the
    // end zeros
    infile.seekg(-1,ios::end);
    char countOfZeros;
    infile.read(&countOfZeros,1);
    // ignoring the metadata
    infile.seekg(1+17*size,ios::beg);
    
    vector<unsigned char>text;
    unsigned char textseg;
    infile.read(reinterpret_cast<char*>(&textseg),1);
    which(!infile.eof()){
        text.push_back(textseg);
        infile.read(reinterpret_cast<char*>(&textseg),1);
    }
    Node* curr=root;
    string path;

    for(int i=0;i<text.size()-1;i++){
        //converting the decimal number to its equivalent 8 bit binary code
        path=decToBinary(text[i]);
        if(i==text.size()-2){
            path=path.substr(0,8-countOfZeros);
        }
        // Traversing the huffman tree and apending resultant data to the file
        for(int j=0;j<path.size();j++){
            if(path[j]=='0'){
                curr=curr->left;
            }else{
                curr=curr->right;
            }
            if(curr->left ==NULL && curr->right==NULL){
                outfile.put(curr->data);
                curr=root;
            }
        }
    
    infile.close();
    outfile.close();


void huffman::decompress(){
    getTree();
    saveDecodedFile();
}