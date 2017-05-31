/*
 * author: Mohit Chawla | mc2683@cornell.edu | Cornell Tech Connective Media
 * Program
 * decription:CS106b-assignment-2-serafini-wordladder-http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1164//assn/serafini.html
 * date: 29 May 2017
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "stack.h"
#include "queue.h"
#include "filelib.h"
#include "hashmap.h"

using namespace std;

//STATIC-CONSTANTS
static string DICTIONARY_FILE_NAME_PROMPT = "Dictionary file name? ";
static string BAD_FILE_REPROMPT = "Unable to open that file.  Try again.";
static string WORD_1_PROMPT = "Word #1 (or Enter to quit):";
static string WORD_2_PROMPT = "Word #2 (or Enter to quit):";
static string INVALID_INPUT_MSG = "Invalid input.";
static string TERMINATION_MSG = "Have a nice day.";
static string SAME_LEN_ERR_MSG = "The two words must be the same length.";
static string DICT_DNE_ERR_MSG = "The two words must be found in the dictionary.";
static string DUPLICATE_WRODS_ERR_MSG = "The two words must be different.";

//function prototypes
bool existsInDictionary(string word,HashMap<string,bool> &dictMap);
void loadDictionary(string ipFileName,std::ifstream &in, HashMap<string,bool> &dictMap);
bool validInput(string word1, string word2,HashMap<string,bool> &dictMap);
Stack<string> stackCopy(Stack<string> stack);
void printAnswer(string word2, Stack<string> stack, string word1);
void testCaseReset(Vector<string>& usedKeys, HashMap<string,bool>&dictMap);
void printIntro();

int main() {

    string ipFileName;
    string word1;
    string word2;
    HashMap<string,bool> dictMap ;
    //Used to keep track of keys used for each Tc, cleared after each Tc
    Vector<string> usedKeys;
    ifstream in;
    //print program intro
    printIntro();

    ipFileName = getLine(DICTIONARY_FILE_NAME_PROMPT);
    while (!fileExists(ipFileName)) {
      // case when user-specified file DNE
      cout << BAD_FILE_REPROMPT << endl;
      ipFileName = getLine(DICTIONARY_FILE_NAME_PROMPT);
    }
    //load the dictionary from file into HashMap
    loadDictionary(ipFileName,in,dictMap);

    while(1){
        word1 = getLine(WORD_1_PROMPT);
        if(word1.length()==0)
            break;
        word1 = toLowerCase(word1);
        word2 = getLine(WORD_2_PROMPT);
        if(word2.length()==0)
            break;
        word2 = toLowerCase(word2);
        if(validInput(word1,word2,dictMap)){
            //calculate length at once to avoid recomputations later
            int length = (int)word1.length();
            Queue<Stack<string>> queue;
            Stack<string> initialStack;
            initialStack.add(word1);
            queue.add(initialStack);
            usedKeys.add(word1);
            bool solved = false;
            while(!queue.isEmpty() && !solved){
               //partialLadder dequeued from main queue
               Stack<string> partialLadder = queue.dequeue();
               //get word at the top of the partialLadder(Stack)
               string topWord = partialLadder.top();
               for(int i=0;i<length;i++){
                   for(int j=97;j<123;j++){
                       if(topWord[i]!=(char)j){
                           //temp string stores variations of the original topWord
                           string temp = topWord;
                           temp[i] = (char)j;
                           if(existsInDictionary(temp,dictMap) && !dictMap.get(temp)){
                                //word exists in dict and unused;
                                dictMap.put(temp,true); //use this word
                                usedKeys.add(temp);
                                if(equalsIgnoreCase(temp,word2)){
                                    printAnswer(word2,partialLadder,word1);
                                    solved =true;
                                    break;
                                }else{
                                    //copy partialLadder, push new word, queue it
                                    Stack<string> partialLadderCopy = stackCopy(partialLadder);
                                    partialLadderCopy.push(temp);
                                    queue.enqueue(partialLadderCopy);
                                }
                           }
                       }
                   }
               }
            }
            if(!solved){
                //case when no solution exists
                cout<<"No word ladder found from "<<word2<<" back to "<<word1<<"."<<endl;
            }
        }
        //reset operations after each Tc
        testCaseReset(usedKeys,dictMap);
    }
    cout << TERMINATION_MSG << endl;
    return 0;
}



/*
* Test if a word exists in the dictionary
* @param
*   word:string
*   dictMap:HashMap<string,bool>&
* @return
*   bool
*/
bool existsInDictionary(string word,HashMap<string,bool> &dictMap){
   return dictMap.containsKey(word);
}

/*
* Loads the dictionary into the HashMap
* @param
*   ipFileName:string
*   in:ifstream
*   dictMap:HashMap<string,bool>&
* @return
*
*/
void loadDictionary(string ipFileName,std::ifstream &in, HashMap<string,bool> &dictMap){
    string thisWord;
    openFile(in, ipFileName.c_str());
    while(!in.eof()){
        getLine(in, thisWord);
        dictMap.add(thisWord,false);
    }
}

/*
* Used for input validation
* @param
*   word1:string
*   word2:string
*   dictMap:HashMap<string,bool>
* @return
*   bool
*/
bool validInput(string word1, string word2,HashMap<string,bool> &dictMap){
    if(equalsIgnoreCase(word1,word2)){
        cout<<DUPLICATE_WRODS_ERR_MSG<<endl;
        return false;
    }
    if(word1.length()!=word2.length()){
        cout<<SAME_LEN_ERR_MSG<<endl;
        return false;
    }
    if(!existsInDictionary(word1,dictMap) || !existsInDictionary(word2,dictMap)){
        cout<<DICT_DNE_ERR_MSG<<endl;
        return false;
    }
       return true;
}

/*
* Copies the stack
* @param
*   stack:HashMap<string,bool>&
* @return
*   HashMap<string,bool>
*/
Stack<string> stackCopy(Stack<string> stack){
    return stack;
}

/*
* Prints the ladder using the stack
* @param
*   stack:HashMap<string,bool>
*   string:word1
*   string:word2
* @return
*/
void printAnswer(string word2, Stack<string> stack, string word1){
    cout<<"A ladder from "<<word2<<" back to "<<word1<<":"<<endl;
    cout<<word2<<" ";
    while(!stack.isEmpty()){
        cout<<stack.pop()<<" ";
    }
    cout<<endl;
}

/*
* Encapsulates reset operations to be done after each Tc
* @param
*   usedKeys:Vector<string>&
*   dictMap:HashMap<string,bool>&
* @return
*/
void testCaseReset(Vector<string>& usedKeys, HashMap<string,bool>&dictMap){
    int size = usedKeys.size();
    for(int i=0;i<size;i++){
        dictMap.put(usedKeys.get(i),false);
    }
    usedKeys.clear();
}

/*
* Prints program introduction
* @param
* @return
*/
void printIntro(){
    cout<<"Welcome to CS 106B Word Ladder."<<endl;
    cout<<"Please give me two English words, and I will change the"<<endl;
    cout<<"first into the second by changing one letter at a time."<<endl;
    cout<<endl;
}
