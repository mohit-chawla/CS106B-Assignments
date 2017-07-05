/*
 * author: Mohit Chawla | mc2683@cornell.edu | Cornell Tech Connective Media
 * Program decription:CS106b-assignment-6-Boggle-http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1164//assn/boggle.html
 * date: 25 June 2017
 */

#include "lexicon.h"
#include "console.h"
#include "simpio.h"
#include "strlib.h"
#include "Boggle.h"
#include "bogglegui.h"

using namespace std;

//Static messages or prompts
static string BOARD_GENERATION_CHOICE_PROMPT = "Do you want to generate a random board? ";
static string INVALID_BOARD_STRING = "That is not a 16-letter board string. Try again.";
static string BOARD_LETTERS_INPUT_PROMPT = "Type the 16 letters to appear on the board: ";
static string DUPLICATE_WORD_PROMPT = "You must enter an unfound 4+ letter word from the dictionary.";
static string NEW_WORD_FOUND_MSG = "You have found a new word! \"";
static string WORD_NOT_FOUND_MSG = "That word can't be formed on this board.";
static string USER_WINS_MSG = "WOW, you defeated me! Congratulations!";
static string COMPUTER_WINS_MSG = "Ha ha ha, I destroyed you. Better luck next time, puny human!";
static string USER_TURN_PROMPT = "It's your turn!";
static string COMPUTER_TURN_PROMPT = "It's my turn!";
static string USER_GET_WORD_PROMPT = "Type a word (or Enter to stop): ";


bool validateBoardConfigurationInput(string boardText) {
    if (boardText.size() != 16) return false;
	for (char c : boardText) {
        if (!isalpha(c)) return false;
	}
	return true;
}

void getRoundDetails(string prompt, Boggle & myBoard) {
    clearConsole();
    cout << prompt << endl;
    cout << myBoard << endl;
    cout << "Your words (" << myBoard.getWords().size() << "): ";
    cout << myBoard.getWords() << endl;
    cout << "Your score: " << myBoard.getScoreHuman() << endl;
}

string getBoardInput() {
    string boardText;
	while (true) {
        boardText = getLine(BOARD_LETTERS_INPUT_PROMPT);
        if (validateBoardConfigurationInput(boardText)) break;
        else cout << INVALID_BOARD_STRING << endl;
	}
    // return uppercase board text
	return toUpperCase(boardText);
}


void updateComputerScore(Boggle & myBoard, Set<string> result) {
    for (string word : result) {
        myBoard.addScoreComputer(word.length() - 3);
        BoggleGUI::recordWord(word, BoggleGUI::COMPUTER);
    }
    BoggleGUI::setScore(myBoard.getScoreComputer(), BoggleGUI::COMPUTER);

    cout << "My words (" << result.size() << "): " << result << endl;
    cout << "My score: " << myBoard.getScoreComputer() << endl;
}

void computerTurn(Boggle & myBoard) {
    //Computer's turn
    cout << endl << COMPUTER_TURN_PROMPT<< endl;
    Set<string> computerWords = myBoard.computerWordSearch();
    updateComputerScore(myBoard, computerWords);
}

void getWinner(Boggle & myBoard) {
    //compare scores and declare winner
    if (myBoard.getScoreComputer() > myBoard.getScoreHuman()) {
        //computer wins
        string result = COMPUTER_WINS_MSG;
        cout << result << endl;
        BoggleGUI::setStatusMessage(result);
    } else {
        //user wins
        string result = USER_WINS_MSG;
        cout << result  << endl;
        BoggleGUI::setStatusMessage(result);
    }
}

string startRound(Boggle & myBoard) {
    // clear highlighting
    string word = toUpperCase(getLine(USER_GET_WORD_PROMPT));
    BoggleGUI::clearHighlighting();

    // Input = Enter, break
    if (word.empty()) {
        return "";
    }
    // Make sure the word is "legal"
    else if (!myBoard.checkWord(word)) {
        return DUPLICATE_WORD_PROMPT;
    }
    // word found, update score,gui
    else if (myBoard.humanWordSearch(word)) {
        myBoard.addScoreHuman(word.length() - 3);
        BoggleGUI::setScore(myBoard.getScoreHuman(), BoggleGUI::HUMAN);
        BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
        return NEW_WORD_FOUND_MSG + word + "\"";
    }
    else {
        return WORD_NOT_FOUND_MSG;
    }
}

void playOneGame(Lexicon& dictionary) {
    string boardText = getYesOrNo(BOARD_GENERATION_CHOICE_PROMPT) ? "" : getBoardInput();
    Boggle myBoard(dictionary, boardText);
    string prompt = USER_TURN_PROMPT;

    BoggleGUI::initialize(4, 4);
    BoggleGUI::labelAllCubes(myBoard.getText());

    // Round loop
  	while (true) {
        BoggleGUI::setStatusMessage(prompt);
        getRoundDetails(prompt, myBoard);
        prompt = startRound(myBoard);
  		if (prompt == "") break;
  	}
      computerTurn(myBoard);
      getWinner(myBoard);
}
