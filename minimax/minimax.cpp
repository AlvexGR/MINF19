#include "stdc++.h"
using namespace std;
char board[3][3];
char player = 'X', computer = 'O';
int heuristic = 50;
void init() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      board[i][j] = '-';
    }
  }
}
void printBoard() {
  cout << "-----" << endl;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      cout << board[i][j] << " ";
    }
    cout << endl;
  }
  cout << "-----" << endl;
}
bool isMovesLeft()
{ 
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == '-') {
        return true; 
      }
    }
  }
  return false;
}
int evaluate() {
  for (int row = 0; row < 3; row++) {
    if (board[row][0] == board[row][1] && board[row][1] == board[row][2]) {
      if (board[row][0] == computer) {
        return heuristic;
      } else if (board[row][0] == player) {
        return -heuristic;
      }
    }
  }

  for (int col = 0; col < 3; col++) { 
    if (board[0][col] == board[1][col] && board[1][col] == board[2][col]) { 
      if (board[0][col] == computer) {
        return heuristic;
      } else if (board[0][col] == player) {
        return -heuristic;
      }
    }
  }

  if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
    if (board[0][0] == computer) {
      return heuristic;
    } else if (board[0][0] == player) {
      return -heuristic;
    }
  }

  if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
    if (board[0][2] == computer) {
      return heuristic;
    } else if (board[0][2] == player) {
      return -heuristic;
    }
  }

  return 0;
}
int minimax(int depth, bool isMax) {
  int score = evaluate();
  if (score == heuristic) {
    return score - depth;
  }

  if (score == -heuristic) {
    return score + depth;
  }

  if (isMovesLeft() == false) {
    return 0;
  }

  if (isMax) {
    int best = -1000;

    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (board[i][j] == '-') {
          board[i][j] = computer;
          best = max(best, minimax(depth + 1, !isMax));
          board[i][j] = '-';
        }
      }
    }
    return best;
  }

  int best = 1000;
  for (int i = 0; i < 3; i++) { 
    for (int j = 0; j < 3; j++) { 
      if (board[i][j] == '-') {
        board[i][j] = player;
        best = min(best, minimax(depth + 1, !isMax));
        board[i][j] = '-';
      }
    }
  }
  return best;
}
void computerMakeMove() {
  int bestVal = -1000;
  int x, y;
  x = y = -1;

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == '-') {
        board[i][j] = computer;
        int moveVal = minimax(0, false);
        board[i][j] = '-';
        if (moveVal > bestVal) {
            x = i;
            y = j;
            bestVal = moveVal;
        }
      }
    }
  }
  board[x][y] = computer;
  printBoard();
}
void playerMakeMove() {
  int x, y;
  cout << "Please select a box: " << endl;
  do {
    cin >> x >> y;
    if (x < 0 || x > 3 || y < 0 || y > 3 || board[x][y] != '-') {
      cout << "This is not a valid move" << endl;
    } else {
      break;
    }
  } while (true);
  board[x][y] = player;
  printBoard();
}
int main() {
  init();
  bool turn = true;
  int result = 0;
  printBoard();
  while (true) {
    if (turn) {
      // Player
      playerMakeMove();
    } else {
      // Computer
      computerMakeMove();
    }
    result = evaluate();
    if (!isMovesLeft() || result != 0) {
      break;
    }
    turn = !turn;
  }
  if (result == -heuristic) {
    cout << "X wins" << endl;
  } else if (result == heuristic) {
    cout << "O wins" << endl;
  } else {
    cout << "Tie!" << endl;
  }
  return 0;
}
