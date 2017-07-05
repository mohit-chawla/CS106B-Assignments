#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
#include "set.h"
using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    char getLetter(int row, int col);
    bool checkWord(string word);
    bool humanWordSearch(string word);
    Set<string> computerWordSearch();
    int getScoreHuman();
    int getScoreComputer();

    friend ostream& operator<<(ostream& out, Boggle& boggle);

    string getText();
    Set<string> getWords();
    void addScoreComputer(int n);
    void addScoreHuman(int n);

private:
    Lexicon dictionary;
    Grid<char> board;
    Set<string> wordsFound;
    string boardText;
    int humanScore;
    int computerScore;
    bool depthFirstSearch(string word, Grid<bool> & visited, int r, int c);
    void searchAllWords(Set<string> & result, Grid<bool> & checked, string word, int r, int c);
    string getRandomBoard();
    void setBoardText();
};

#endif // _boggle_h
