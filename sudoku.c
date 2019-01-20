/////////////////////////////////////////////////////////////////////////////
// INTEGRITY INSTRUCTIONS

// Explicitly state the level of collaboration on this question
// Examples:
//   * I discussed ideas with classmate(s) [include name(s)]
//   * I worked together with classmate(s) in the lab [include name(s)]
//   * Classmate [include name] helped me debug my code
//   * I consulted website [include url]
//   * None
// A "None" indicates you completed this question entirely by yourself
// (or with assistance from course staff)
/////////////////////////////////////////////////////////////////////////////
// INTEGRITY STATEMENT:
// I received help from the following sources:

// None

// Name: Diwen Zhu
// login ID: d55zhu
/////////////////////////////////////////////////////////////////////////////

#include "cs136.h"
#include "sudoku.h"
#include "array.h"

const int DIM = 9;

static const int EMPTY = 0;
static const char BLANK = '-';
static const int MIN = 1;
static const int MAX = 9;

static const int SUCCESS = 0;
static const int ERROR = -1;
static const int ERASE_EMPTY_CELL = -2;
static const int ERASE_FILLED_CELL = -3;
static const int ERROR_NEXT_CELL = -4;

struct sudoku {
  int puzzle[DIM * DIM];
  int sol[DIM * DIM];
};

// distinct_range checks if non-zero numbers in a are distinct
bool distinct_range(const int a[], int len) {
  assert(a);
  assert(len>0&& len<=DIM);
  int b[len];
  for(int i=0; i<len; ++i) {
    b[i] = 0;
  }
  for(int i=0; i<len; ++i) {
    if (a[i]>=MIN && a[i] <= MAX){
      b[a[i]-1]++;
    }
  }
  for(int j=0; j<len; ++j) {
    if (b[j] >1) {
      return false;
    }
  }
  return true;
}

int success_row(const struct sudoku *s, int row) {
  assert(s);
  assert(0 <= row && row <= DIM - 1);

  int a[DIM];
  for(int i=0; i<DIM; ++i) {
    a[i] = s->sol[row*DIM+i];
  }
  if (distinct_range(a, DIM)) {
    return SUCCESS;
  } return ERROR;
}

int success_col(const struct sudoku *s, int col) {
  assert(s);
  assert(0 <= col && col <= DIM - 1);

  int a[DIM];
  for(int i=0; i<DIM; ++i) {
    a[i] = s->sol[i*DIM+col];
  }
  if (distinct_range(a, DIM)) {
    return SUCCESS;
  } return ERROR;
}

int success_box(const struct sudoku *s, int row, int col) {
  assert(s);
  assert(0 <= row && row <= DIM - 1);
  assert(0 <= col && col <= DIM - 1);

  int a[DIM];
  row /= 3;
  col /= 3;
  for(int i=0; i<DIM; ++i) {
    a[i] = s->sol[(row*3+i/3)*DIM+col*3+i%3];
  }
  if (distinct_range(a, DIM)) {
    return SUCCESS;
  } return ERROR;
}

// You need to implement the following four functions.

int fill_cell(struct sudoku *s, int row, int col, int num) {
  assert(s);
  assert(0 <= row && row <= DIM - 1);
  assert(0 <= col && col <= DIM - 1);
  assert(num <= MAX && num >= MIN);

  if (s->sol[row*DIM +col] == EMPTY) {
    s->sol[row*DIM + col] = num;
    if (success_row(s, row) == 0 && success_col(s, col) == 0 && success_box(s, row, col) == 0) {
      return SUCCESS;
    } else {
    s->sol[row*DIM + col] = EMPTY;
    }
  }
  return ERROR;
}

void choices_cell(const struct sudoku *s, int row, int col, 
                  int choices[], int *num_choices) {
  assert(s);
  assert(choices);
  assert(0 <= row && row <= DIM - 1);
  assert(0 <= col && col <= DIM - 1);
  *num_choices = 0;
  int posn = 0;

  for(int i=0; i<DIM; i++) {
    choices[i] = 0;
  }
  if(s->sol[row*DIM+col] == EMPTY) {  
    for(int value=MIN; value<=MAX; ++value) {
      int a[DIM];
      for(int j=0; j<DIM; ++j) {     // same row
        if (j == col) {
          a[j] = value;
        } else {
          a[j] = s->sol[row*DIM + j];
        }
      }
      if (distinct_range(a, DIM)) {
        int b[DIM];
        for(int j=0; j<DIM; ++j) {    // same col
          if (j == row) {
            b[j] = value;
          } else {
            b[j] = s->sol[j*DIM + col];
          }
        }
        if (distinct_range(b, DIM)) {    //same box
          int c[DIM];
          int r = row /= 3;
          int co = col /= 3;
          for(int i=0; i<DIM; ++i) {
            if (r*3+i/3 == row && co*3+i%3 == col) {
              c[i] = value;
            }
            c[i] = s->sol[(r*3+i/3)*DIM+co*3+i%3];
          }
          if (distinct_range(c, DIM)) {
            (*num_choices)++;
            choices[posn] = value;
            posn++;
          }
        }
      }
    }
  }
}

bool solved_puzzle(const struct sudoku *s) {
  assert(s);

  for(int i=0; i<DIM*DIM; ++i) {
    if (s->sol[i]<MIN || s->sol[i] >MAX) {     
      return false;
    }
  }
  for(int i=0; i<DIM; ++i) {
    if (success_row(s, i)!= SUCCESS || success_col(s, i)!= SUCCESS) {
      return false;
    }
  }
  for(int i=0; i<DIM/3; ++i) {
    for(int j=0; j<DIM/3; ++j) {
      if(success_box(s, i*3, j*3)!= SUCCESS) {  
        return false;
      }
    }
  }
  return true;
}

bool solve(struct sudoku *s) {
  if (solved_puzzle(s)) {
    return true;
  } 
  for(int r=0; r<DIM; ++r) {
    for(int c=0; c<DIM; ++c) {
      if(s->sol[r*DIM+c] == EMPTY) {
        // choices_cell(s, r, c, choices, &num_choices);
        //while(num_choices) {
        for(int value = MIN; value<=MAX; ++value) {
          if(fill_cell(s, r, c,value) == 0) {
            return solve(s);
          }
        }
      }
    }
  }
  return false;
}


// There is no need to modify the rest of the functions.

struct sudoku *read_sudoku(void) {
  struct sudoku *s = malloc(sizeof(struct sudoku));
  char c = 0;
  for (int row = 0; row < DIM; ++row) {
    for (int col = 0; col < DIM; ++col) {
      scanf(" %c", &c);
      if (c == BLANK) {
        s->puzzle[row * DIM + col] = 0;
      } else {
        s->puzzle[row * DIM + col] = c - '0';
      }
    }
  }

  // copy puzzle to solution
  reset_sol(s);

  return s;
}

void sudoku_destroy(struct sudoku *s) {
  assert(s);
  free(s);
}

void print_sol(const struct sudoku *s) {
  assert(s);

  printf("\n");
  for (int row = 0; row < DIM; ++row) {
    for (int col = 0; col < DIM; ++col) {
      int num = s->sol[row * DIM + col];
      if (num == EMPTY) {
        printf("%c", BLANK);
      } else {
        printf("%d", num);
      }  
    }
    printf("\n");
  }
  printf("\n");
}

void reset_sol(struct sudoku *s) {
  assert(s);

  for (int row = 0; row < DIM; ++row) {
    for (int col = 0; col < DIM; ++col) {
      s->sol[row * DIM + col] = s->puzzle[row * DIM + col];
    }
  }  
}

// cell_empty(board, row, col) returns true
// if cell (row,col) is empty on board.
// requires: board is a valid sudoku puzzle.
static bool cell_empty(const int board[], int row, int col) {
  assert(board);
  assert(0 <= row && row <= DIM - 1);
  assert(0 <= col && col <= DIM - 1);

  return board[row * DIM + col] == EMPTY;
}

int erase_cell(struct sudoku *s, int row, int col) {
  assert(s);
  assert(0 <= row && row <= DIM - 1);
  assert(0 <= col && col <= DIM - 1);

  if (cell_empty(s->sol, row, col)) {
    return ERASE_EMPTY_CELL;
  }
  if (!cell_empty(s->puzzle, row, col)) {
    return ERASE_FILLED_CELL;
  }
  s->sol[row * DIM + col] = EMPTY;
  return SUCCESS;
}


int next_cell(const struct sudoku *s, int *row, int *col) {
  assert(s);
  assert(row);
  assert(col);

  int choices[DIM];
  int num_choices = 0;
  for (int i = 0; i < DIM; ++i) {
    for (int j = 0; j < DIM; ++j) {
      if (!cell_empty(s->sol, i, j)) continue;
      choices_cell(s, i, j, choices, &num_choices);
      if (num_choices == 1) {
        *row = i;
        *col = j;
        return SUCCESS;
      }
    }
  }
  return ERROR_NEXT_CELL;
}
