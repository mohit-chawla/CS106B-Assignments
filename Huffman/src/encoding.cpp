/*
 * author: Mohit Chawla | mc2683@cornell.edu | Cornell Tech Connective Media
 * Program decription:CS106b-assignment-6-Huffman-http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1164//assn/huffman.html
 * date: 29 June 2017
 */
#include <string>
#include "encoding.h"
#include "pqueue.h"
#include "vector.h"

/*
 * Builds a frequency table
 * @param
 * istream&:input
 * @return
 *  Map<int, int>
*/
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> frequencyTable;
    int letterAscii = input.get();
    while(letterAscii!=EOF){
        if(frequencyTable.containsKey(letterAscii)){
         frequencyTable[letterAscii] = frequencyTable[letterAscii] + 1;
        }else{
            frequencyTable.add(letterAscii, 1);
        }
        letterAscii = input.get();
    }
    frequencyTable.add(PSEUDO_EOF, 1);
    return frequencyTable;
}

/*
 * Builds an encoding tree
 * @param
 * const Map<int, int>&:frequencyTable
 * @return
 *  HuffmanNode*
*/
HuffmanNode* buildEncodingTree(const Map<int, int>& frequencyTable) {
    Vector<int> listedLetters = frequencyTable.keys();
    PriorityQueue<HuffmanNode*> pqueue;
    for(Vector<int>::iterator i=listedLetters.begin(); i!=listedLetters.end(); i++){
        pqueue.enqueue( new HuffmanNode(*i, frequencyTable[*i], NULL, NULL) , frequencyTable[*i]);
    }
    while(pqueue.size()>=2){
        //get child nodes
        HuffmanNode* min1 = pqueue.dequeue();
        HuffmanNode* min2 = pqueue.dequeue();
        int newPriority = (min1->count + min2->count);
        HuffmanNode* jointEle = new HuffmanNode(NOT_A_CHAR, newPriority, min1, min2);
        pqueue.enqueue( jointEle , newPriority);
    }
    return pqueue.dequeue();;
}

/*
 * Traverse encoding tree and update encoding map
 * @param
 * HuffmanNode*:encodingTree
 * Vector<int>&:treeTraversalBitSeq
 * Map<int, string>&:encodingMap
 * @return
*/
void DFStreeAndUpdateEncodingMap(HuffmanNode* encodingTree, Vector<int>& treeTraversalBitSeq, Map<int, string>& encodingMap){
    if(encodingTree == NULL){//tree is null
        return;
    }
    if(encodingTree->isLeaf()){ //leaf node reached
        string huffCodeForLetter = "";
        for(Vector<int>::iterator i=treeTraversalBitSeq.begin(); i!= treeTraversalBitSeq.end(); i++)
            huffCodeForLetter+=to_string(*i);
        encodingMap.add(encodingTree->character, huffCodeForLetter);
        return;
    }
    else{
        if(encodingTree->zero!=NULL){ // move left
            treeTraversalBitSeq.add(0);
            DFStreeAndUpdateEncodingMap(encodingTree->zero, treeTraversalBitSeq, encodingMap);
            int lastIndex = treeTraversalBitSeq.size() - 1;
            treeTraversalBitSeq.remove(lastIndex);
        }
        if(encodingTree->one!=NULL){ // move right
            treeTraversalBitSeq.add(1);
            DFStreeAndUpdateEncodingMap(encodingTree->one, treeTraversalBitSeq, encodingMap);
            int lastIndex = treeTraversalBitSeq.size() - 1;
            treeTraversalBitSeq.remove(lastIndex);
        }
    }
}

/*
 * Build encoding map
 * @param
 * HuffmanNode*:encodingTree
 * @return
 * Map<int, string>
*/
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    Vector<int> treeTraversalBitSeq;
    DFStreeAndUpdateEncodingMap(encodingTree, treeTraversalBitSeq, encodingMap);
    return encodingMap;
}

/*
 * Encodes a string/data
 * @param
 * istream&:input
 * const Map<int, string>&:encodingMap
 * obitstream&:output
 * @return
*/
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    int nextChar = input.get();
    while(nextChar!=EOF){
        string bitString = encodingMap[nextChar];
        for(int i=0; i<bitString.length(); i++){
          if(bitString[i] == '1'){
            output.writeBit(1);
          }else{
            output.writeBit(0);
          }
        }
        nextChar = input.get();
    }
    string bitString = encodingMap[PSEUDO_EOF];
    for(int i=0; i<bitString.length(); i++){
      if(bitString[i] == '1'){
        output.writeBit(1);
      }else{
          output.writeBit(0);
      }
    }
}

/*
 * Decode from encoding tree
 * @param
 * Vector<int>:bitSequence
 * HuffmanNode*:encodingTree
 * ostream&:output
 * @return
*/
void decodeBitsFromTree(Vector<int> bitSequence, HuffmanNode* encodingTree, ostream& output){
    int i=0, bitSequenceLen = bitSequence.size();
    if(bitSequenceLen == 0)
        return;
    bool EOFfound = false;

    HuffmanNode* currentNode = encodingTree;
    while(  i<bitSequenceLen && !EOFfound ){
        if(bitSequence[i] == 0){ //Go Left
            if(currentNode->zero)
                currentNode = currentNode->zero;
        }
        else{  // Go Right
            if(currentNode->one)
                currentNode = currentNode->one;
        }
        if( currentNode->isLeaf() ){
            if(currentNode->character == PSEUDO_EOF){
                output.put( (char) PSEUDO_EOF );
                EOFfound = true;
                break;
            }
            output.put( (char) currentNode->character );
            currentNode = encodingTree;
        }
        i++;
    }
}

/*
 * Decode data
 * @param
 * ibitstream&:input
 * HuffmanNode*:encodingTree
 * ostream&:output
 * @return
*/
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    if(encodingTree == NULL){
        return;
    }
    Vector<int> bitSequence;
    int ch = input.readBit();
    while(ch!=EOF){
        bitSequence.add(ch);
        ch = input.readBit();
    }
    decodeBitsFromTree(bitSequence, encodingTree, output);
}

/*
 * Writes header to bit stream
 * @param
 * obitstream&:outfile
 * Map<int, int>&:frequencies
 * @return
*/
void putHeaderToBitstream(obitstream& outfile, Map<int, int>& frequencies) {
    if (!frequencies.containsKey(PSEUDO_EOF)) {
        error("PSEUDO_EOF not defined.");
    }

    outfile << frequencies.size() - 1 << ' ';

    Vector<int> letters = frequencies.keys();
    for(Vector<int>::iterator i=letters.begin(); i!=letters.end(); i++){
        if(*i == PSEUDO_EOF) // Skip EOF
            continue;
        outfile << (char) *i << frequencies[*i] << ' ';
    }
}

/*
 * Compress input
 * @param
 * istream&:input
 * obitstream&:output
 * @return
*/
void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable = buildFrequencyTable(input);
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    Map<int, string> encodingMap = buildEncodingMap(encodingTree);
    putHeaderToBitstream(output, freqTable);
    input.clear();
    input.seekg(0);
    encodeData(input, encodingMap, output);
    freeTree(encodingTree);
}

/*
 * Gets header from bit stream to be decoded
 * @param
 * ibitstream&:infile
 * @return
 * Map<int, int>
*/
Map<int, int> getHeaderFromBitStream(ibitstream& infile) {
    Map<int, int> result;
    int numValues;
    infile >> numValues;
    infile.get();

    for (int i = 0; i < numValues; i++) {
        int ch = infile.get(), frequency;
        infile >> frequency;
        infile.get();
        result[ch] = frequency;
    }
    result[PSEUDO_EOF] = 1;
    return result;
}


/*
 * Decompresses data
 * @param
 * ibitstream&:input
 * ostream&:output
 * @return
*/
void decompress(ibitstream& input, ostream& output) {
    Map<int, int> freqTable = getHeaderFromBitStream(input);
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    decodeData(input, encodingTree, output);
    freeTree(encodingTree);
}


/*
 * Function to free the encoding tree
 * @param
 * HuffmanNode*:node
 * @return
*/
void freeTree(HuffmanNode* node) {
    if(!node)
        return;
    if(node->zero!=NULL)
        freeTree(node->zero);
    if(node->one!=NULL)
        freeTree(node->one);
    delete node;
}

