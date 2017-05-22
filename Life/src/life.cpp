/*
 * author: Mohit Chawla | mc2683@cornell.edu | Cornell Tech Connective Media
 * Program
 * decription:CS106b-assignment-1-the-game-of-life-http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1164//assn/life.html
 * date: 21 May 2017
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "strlib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "lifegui.h"
using namespace std;

// Global-constants
static string USER_ACTION_PROMPT = "a)nimate, t)ick, q)uit? ";
static string GET_FRAMES_PROMPT = "How many frames? ";
static string GAME_TERMINATION_PROMPT = "Have a nice Life!";
static string INVALID_CHOICE_PROMPT = "Invalid choice; please try again.";
static string BAD_FILE_REPROMPT = "Unable to open that file.  Try again.";
static string GRID_FILE_NAME_PROMPT = "Grid input file name? ";

// Function prototypes
void copyGrid(Grid<char> &destGrid, Grid<char> srcGrid, int nRows, int nCols);
bool isSafe(int r, int c, int nRows, int nCols);
int getNeighbourCount(Grid<char> grid, int r, int c, int nRows, int nCols);
void nextGeneration(Grid<char> &grid, int nRows, int nCols);
void printGameIntro();
void getUserInput(int &nRows, int &nCols, string &fileName, std::ifstream &in);
void getGridFromFile(int nRows, int nCols, Grid<char> &grid, std::ifstream &in);

int main() {
  ifstream in;
  ofstream out;
  string ipFileName;
  string thisLine;
  Grid<char> grid;
  int nRows;
  int nCols;
  string userAction = "n";

  // print game intro
  printGameIntro();

  // get user input
  getUserInput(nRows, nCols, ipFileName, in);
  // get grid from user-specified file
  getGridFromFile(nRows, nCols, grid, in);
  cout << grid.toString2D("", "", "", "\n") << endl;

  while (!equalsIgnoreCase(userAction, charToString('q'))) {
    userAction = getLine(USER_ACTION_PROMPT);
    userAction = toLowerCase(userAction);
    if (userAction == "a") {
      // animation loop
      int frames = getInteger(GET_FRAMES_PROMPT);
      while (frames--) {
        nextGeneration(grid, nRows, nCols);
        cout << grid.toString2D("", "", "", "\n") << endl;
      }
    } else if (userAction == "t") {
      // tick forward
      nextGeneration(grid, nRows, nCols);
      cout << grid.toString2D("", "", "", "\n") << endl;
    } else if (userAction == "q") {
      // quit, do nothing or break
      cout << GAME_TERMINATION_PROMPT << endl;
    } else {
      // invalid user input
      cout << INVALID_CHOICE_PROMPT << endl;
    }
  }
  // close the input filestream at the end
  in.close();
  return 0;
}

void getUserInput(int &nRows, int &nCols, string &ipFileName,
                  std::ifstream &in) {
  string temp;
  ipFileName = getLine(GRID_FILE_NAME_PROMPT);
  while (!fileExists(ipFileName)) {
    // case when user-specified file DNE
    cout << BAD_FILE_REPROMPT << endl;
    ipFileName = getLine(GRID_FILE_NAME_PROMPT);
  }
  // Proceed only if the specified file exists
  openFile(in, ipFileName.c_str());
  getLine(in, temp);
  nRows = stringToInteger(temp);
  getLine(in, temp);
  nCols = stringToInteger(temp);
}

void getGridFromFile(int nRows, int nCols, Grid<char> &grid,
                     std::ifstream &in) {
  string temp;
  grid.resize(nRows, nCols);
  for (int i = 0; i < nRows; i++) {
    getLine(in, temp);
    for (int j = 0; j < nCols; j++) {
      grid.set(i, j, temp[j]);
    }
  }
}

void copyGrid(Grid<char> &destGrid, Grid<char> srcGrid, int nRows, int nCols) {
  for (int i = 0; i < nRows; i++) {
    for (int j = 0; j < nCols; j++) {
      destGrid.set(i, j, srcGrid.get(i, j));
    }
  }
}

bool isSafe(int r, int c, int nRows, int nCols) {
  if (r < 0 || c < 0 || r == nRows || c == nCols)
    return false;
  return true;
}

int getNeighbourCount(Grid<char> grid, int r, int c, int nRows, int nCols) {
  int count = 0;
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      if (isSafe(r + i, c + j, nRows, nCols) && !(i == 0 && i == j))
        count += (grid.get(r + i, c + j) == 'X') ? 1 : 0;
    }
  }
  return count;
}

void nextGeneration(Grid<char> &grid, int nRows, int nCols) {
  int thisCellNeighbours = 0;
  // use a tempGrid as all operations
  // occur simaltaneously in a generation
  Grid<char> tempGrid(nRows, nCols);
  for (int i = 0; i < nRows; i++) {
    for (int j = 0; j < nCols; j++) {
      thisCellNeighbours = getNeighbourCount(grid, i, j, nRows, nCols);
      if (thisCellNeighbours <= 1)
        tempGrid.set(i, j, '-');
      else if (thisCellNeighbours == 2)
        tempGrid.set(i, j, grid.get(i, j));
      else if (thisCellNeighbours == 3)
        tempGrid.set(i, j, 'X');
      else if (thisCellNeighbours >= 4)
        tempGrid.set(i, j, '-');
    }
  }
  // copy tempgrid to grid
  copyGrid(grid, tempGrid, nRows, nCols);
}

void printGameIntro() {
  cout << "Welcome to the CS 106B Game of Life," << endl;
  cout << "a simulation of the lifecycle of a bacteria colony." << endl;
  cout << "Cells (X) live and die by the following rules:" << endl;
  cout << "- A cell with 1 or fewer neighbors dies." << endl;
  cout << "- Locations with 2 neighbors remain stable." << endl;
  cout << "- Locations with 3 neighbors will create life." << endl;
  cout << "- A cell with 4 or more neighbors dies." << endl;
  cout << endl;
}
