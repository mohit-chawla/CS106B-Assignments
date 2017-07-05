/*
 * author: Mohit Chawla | mc2683@cornell.edu | Cornell Tech Connective Media
 * Program decription:CS106b-assignment-6-Boggle-http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1164//assn/boggle.html
 * date: 25 June 2017
 */

#include "Boggle.h"
#include "bogglegui.h"
#include "shuffle.h"
#include "strlib.h"

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

Boggle::Boggle(Lexicon& dictionary, string boardText) : board(4, 4) {
    if (boardText.empty()){
        //generate a randomboard if boardtext is empty
        boardText = getRandomBoard();
    }
    this->boardText = boardText;
    setBoardText(); // Populating the board grid
    this->dictionary = dictionary;
    humanScore = 0;
    computerScore = 0;
}

/*
 * Get a random board
 * @param
 * @return
*/
string Boggle::getRandomBoard() {
    string boardText = "";
    shuffle(CUBES, 16);
    for (string cube : CUBES) {
        boardText += cube[randomInteger(0,5)];
    }
    return toUpperCase(boardText);
}

/*
 * Set user input board text
 * @param
 * @return
*/
void Boggle::setBoardText() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int index = j + (4 * i);
            board[i][j] = boardText[index];
		}
	}
}

/*
 * Get letter of board
 * @param
 * int:row
 * int:col
 * @return
 * char
*/
char Boggle::getLetter(int row, int col) {
	if (board.inBounds(row, col)) {
        return board.get(row, col);
    }
    else{
        error("Boggle::getLetter-- index out of bounds."); // throw 20;
        return EOF;
    }

}

/*
 * Check for word validity
 * @param
 * string:word
 * @return
 * bool
*/
bool Boggle::checkWord(string word) {
    word = toUpperCase(word);
    if (!wordsFound.contains(word) && dictionary.contains(word) && word.length() >= 4 && word.length() <= 16)
        return true;
	else
    return false;
}

/*
 * Function implementing DFD for word search
 * @param
 * string:word
 * Grid<bool>&:visited
 * int:r
 * int:c
 * @return
 * bool
*/
bool Boggle::depthFirstSearch(string word, Grid<bool> & visited, int r, int c) {
    if (word.empty())
          return true;

        for (int dr = -1; dr < 2; dr++) { // adj cubes
            for (int dc = -1; dc < 2; dc++) {
                if (board.inBounds(r+dr,c+dc) && board[r+dr][c+dc] == word[0] && visited[r+dr][c+dc] != 1) {
                    visited[r+dr][c+dc] = true; // visit
                      BoggleGUI::setAnimationDelay(100);
                      BoggleGUI::setHighlighted(r+dr, c+dc, true);
                      if (depthFirstSearch(word.substr(1), visited, r+dr, c+dc))
                        return true;
                    visited[r+dr][c+dc] = false; // backtrack
                    BoggleGUI::setHighlighted(r+dr, c+dc, false);
                }
            }
        }
        return false;
}

/*
 * Search for presence of word entered by human
 * @param
 * string:word
 * @return
 * bool
*/
bool Boggle::humanWordSearch(string word) {
    Grid<bool> visited(4, 4);
    visited.fill(false);//reset visited
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (board[r][c] == word[0]) {
                if (depthFirstSearch(word, visited, r, c)) {
                    wordsFound.add(word);
                    return true;
                }
            }
        }
    }
    return false;
}

/*
 * Get words found
 * @param
 * @return
 * Set<string>
*/
Set<string> Boggle::getWords() {
    return wordsFound;
}

/*
 * Get human score
 * @param
 * @return
*/
int Boggle::getScoreHuman() {
    return humanScore;
}

/*
 * Add human score
 * @param
 * int:n
 * @return
*/
void Boggle::addScoreHuman(int n) {
    humanScore += n;
}
/*
 * Utility function for computer to search all words
 * @param
 * Set<string>&:result
 * Grid<bool>&:visited
 * string:word
 * int:r
 * int:c
 * @return
*/
void Boggle::searchAllWords(Set<string> & result, Grid<bool> & visited, string word, int r, int c) {
    if (dictionary.contains(word) &&
            !wordsFound.contains(word) &&
            word.length() > 3)
        result.add(word);

    if (dictionary.containsPrefix(word)) {
        for (int dr = -1; dr < 2; dr++) {
            for (int dc = -1; dc < 2; dc++) {
                if (board.inBounds(r+dr,c+dc) && visited[r+dr][c+dc] != 1) {
                    visited[r+dr][c+dc] = true; // visit
                      word += board[r+dr][c+dc];
                      searchAllWords(result, visited, word, r+dr, c+dc);
                    visited[r+dr][c+dc] = false; // backtrack
                    word.pop_back();
                }
            }
        }
    }
}

/*
 * Simulate computer's turn
 * @param
 * @return
 * Set<string>
*/
Set<string> Boggle::computerWordSearch() {
    Set<string> result;
    string word;
    Grid<bool> visited(4, 4);
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            word += board[r][c]; // visit
            visited[r][c] = true;
              searchAllWords(result, visited, word, r, c);
            visited[r][c] = false; // backtrack
            word.pop_back();
        }
    }
    return result;
}

/*
 * Get computer's score
 * @param
 * @return
 * int
*/
int Boggle::getScoreComputer() {
    return computerScore;
}

/*
 * Add to computer's score
 * @param
 * int:n
 * @return
*/
void Boggle::addScoreComputer(int n) {
    computerScore += n;
}

/*
 * Get board text
 * @param
 * @return
 * string
*/
string Boggle::getText() {
    return boardText;
}

ostream& operator<<(ostream& out, Boggle & boggle) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            out << boggle.boardText[c + 4 * r];
        }
        out << endl;
    }
    return out;
}
