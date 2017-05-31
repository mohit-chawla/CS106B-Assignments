/*
 * author: Mohit Chawla | mc2683@cornell.edu | Cornell Tech Connective Media
 * Program
 * decription:CS106b-assignment-2-serafini-Ngrams-http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1164//assn/serafini.html
 * date: 29 May 2017
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "hashmap.h"
#include "Vector.h"
#include "random.h"

using namespace std;

//GLOBAL-CONSTANTS
static string INPUT_FILE_NAME_PROMPT =  "Input file name? ";
static string BAD_FILE_REPROMPT = "Unable to open that file.  Try again.";
static string N_INPUT_PROMPT = "Value of N? ";
static string N_INPUT_ERR_MSG = "N must be 2 or greater.";
static string NUM_WORDS_PROMPT = "# of random words to generate (0 to quit)? ";
static string EXIT_MSG = "Exiting.";

//function prototypes
Vector<string> getWindow(Vector<string> allWords, int start, int size);
string nextWord(Vector<string> allWords, int start);
void printIntro();

int main() {
    printIntro();
    string ipFileName;
    int N; //N-grams
    int numWords = -1;
    ipFileName = getLine(INPUT_FILE_NAME_PROMPT);
    while (!fileExists(ipFileName)) {
      // case when user-specified file DNE
      cout << BAD_FILE_REPROMPT << endl;
      ipFileName = getLine(INPUT_FILE_NAME_PROMPT);
    }
    HashMap<Vector<string>,Vector<string>> hmap;
    Vector<string> allWords;
    string word;
    ifstream fin(ipFileName);
    while(fin>>word){
        allWords.add(word);
    }

    N = getInteger(N_INPUT_PROMPT);
    while(N<2){
        cout<<N_INPUT_ERR_MSG<<endl;
        N = getInteger(N_INPUT_PROMPT);
    }
    for(int i=0;i<allWords.size();i++){
        Vector<string> window = getWindow(allWords,i,N-1);
        string nextword = nextWord(allWords,i+N-1);
        Vector<string> temp;
        if(hmap.containsKey(window)){
            temp = hmap.get(window);
        }
        temp.add(nextword);
        hmap.put(window,temp);
    }
    while(numWords!=0){
        cout<<endl;
        numWords = getInteger(NUM_WORDS_PROMPT);
        while(numWords<N && numWords!=0){
            cout<<"Must be at least "<<N<<" words."<<endl;
            numWords = getInteger(NUM_WORDS_PROMPT);
            if(numWords ==0){
                break;
            }
        }
        if(numWords!=0){
            cout<<"... ";
            int startKey = randomInteger(0,hmap.size()-1);
            Vector<Vector<string>> allKeys  = hmap.keys();
            int wordsLeft = numWords;
            Vector<string> initialRandString = allKeys.get(startKey);
            for(int i=0;i<initialRandString.size();i++){
                if(i==initialRandString.size()-1)
                    cout<<initialRandString.get(i);
                else
                    cout<<initialRandString.get(i)<<" ";
            }
            wordsLeft -= N-1;
            Vector<string> thisWindow = allKeys.get(startKey);
            while(wordsLeft>0){
                Vector<string> nextWordVector = hmap.get(thisWindow);
                //cout<<nextWordVector.toString()<<endl;
                int rand = randomInteger(0,nextWordVector.size()-1);
                string newWord = nextWordVector.get(rand);
                cout<<" "<<newWord;
                //slide the window
                thisWindow.remove(0);
                thisWindow.add(newWord);
                wordsLeft--;
            }
            cout<<" ..."<<endl;
        }
    }
    cout << "Exiting." << endl;
    return 0;
}

/*
 * Get window of given size
 * @param
 *  Vector<string>:allWords
 *  int:start
 *  int:size
 * @return
 *  Vector<string>:window
*/
Vector<string> getWindow(Vector<string> allWords, int start, int size){
    Vector<string> window;
    int end = allWords.size();
    for(int i=0;i<size;i++){
        if(start+i>=end)
            start = -i;
        window.add(allWords.get(start+i));
    }
    cout<<"window:"<<window.toString()<<endl;
    return window;
}

/*
 * Get next word from the vectoe
 * @param
 *  Vector<string>:allWords
 *  int:start
 * @return
 *  string
*/
string nextWord(Vector<string> allWords, int start){
    return allWords.get(start%allWords.size());
}

/*
 * Get window of given size
 * @param
 * @return
 *  void
*/
void printIntro(){
    cout<<"Welcome to CS 106B Random Writer ('N-Grams')."<<endl;
    cout<<"This program makes random text based on a document."<<endl;
    cout<<"Give me an input file and an 'N' value for groups"<<endl;
    cout<<"of words, and I'll create random text for you."<<endl;
    cout<<endl;
}
