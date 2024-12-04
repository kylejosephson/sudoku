/***********************************************************************
* Program:
*    Project Extra,  Sudoku Extra Credit 
*    Brother Wagstaff, CS124
* Author:
*    Kyle Josephson
* Summary: 
*    This programs will outline the main sudoku game without all 
*    the checks.  The user will enter the file where the sudoku board 
*    is then the instructions and the board will display.  The user can
*    then enter numbers and play sudoku. When the user quits, the 
*    user will create a save file so they cab play it later.
*
*    Estimated:  3.0 hrs   
*    Actual:     6.0 hrs
*     I had a lot of problems with everything from losing a computer
*     to trying to work through all the error checking.  I finally
*     did it though.  Sorry it took so long.
*     
************************************************************************/

#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
using namespace std;

void getFilename(char[]);
void interact(int[][9],int[][9]);
void saveFilename(int[][9], int[][9], int);
void displayBoard(int[][9],int[][9]);
void editBoard(int[][9], int[][9]);
void computeValue(int[][9]);
bool checkSquare(int[][9], int, int, int);
bool checkRow(int[][9], int, int, int);
bool checkColumn(int[][9], int, int, int);
char getCoordinates(int, int);
bool checkArray(int[][9], int, int, int);

/**********************************************************************
 *MAIN
 *   The main function will call a function to get the file name.
 *   Then it will create the array from the file name.
 *   Then it will call the function to interact with the user.
 *   Then it will call a function to save the game.
 ***********************************************************************/
int main()
{
   int numberOfSaves = 0;
   int count = 0;
   bool setValue = true;
   char fileName[256];
   int boardArray[18][9] = {};
   int greenArray[18][9] = {};
   ifstream finSave("saves.txt");
   if (finSave.fail())
      numberOfSaves = 0;
   else
      finSave >> numberOfSaves;
   finSave.close();   
   getFilename(fileName);
   ifstream fin(fileName);
   if (fin.fail())
      cout << "ERROR: Unable to open file " << fileName << ".\n";
   for (int row = 0; row < 9; row++)
   {
      for (int col = 0; col < 9; col++)
      {
         fin >> boardArray[row][col];        
      }      
   }   
   if (numberOfSaves > 1)
   {
      for (int row = 9; row < 18; row++)
      {
         for (int col = 0; col < 9; col++)
         {
            fin >> greenArray[row][col];
         }         
      }
      for (int row = 0; row < 9; row++)
      {
         for (int col = 0; col < 9; col++)
         {
            greenArray[row][col] = greenArray[row + 9][col];
         }         
      }      
   }   
   else
   {
      for (int row = 0; row < 9; row++)
      {
         for (int col = 0; col < 9; col++)
         {
            greenArray[row][col] = boardArray[row][col];
         }         
      }      
   }   
   fin.close();
   for (int row = 0; row < 9; row++)
   {
      for (int col = 0; col < 9; col++)
      {
         if (boardArray[row][col] != 0)
         {
            setValue = checkArray(boardArray, boardArray[row][col],
                                  row, col);
            if (setValue == false)
            {
               cout << "ERROR: Duplicate value \'" << boardArray[row][col]
                    << "\' in inside square represented by \'"
                    << getCoordinates(row, col) << row + 1 <<  "\'\n";
               return 0;
            }            
         }            
      }      
   }   
   interact(boardArray, greenArray);
   saveFilename(boardArray, greenArray, numberOfSaves);
   for (int end = 0; end < 1000000000; end++)
      ;
   return 0;
}

/***********************************************************************
 * GET FILE NAME
 *   This function will get the file name from the user and return it
 *   to main.
 **********************************************************************/
void getFilename(char fileName[])
{
   cout << "Where is your board located? ";
   cin >> fileName;
}

/**********************************************************************
 * INTERACT
 *   This function will let the user choose one command to direct
 *   the game.  It will loop through the menu until the user selects
 *   the right response to exit the loop and return to main.
 *********************************************************************/
void interact(int boardArray[][9], int greenArray[][9])
{
   bool loop = true;
   char menuChoice;
   cout << "Options:\n";
   cout << "   ?  Show these instructions\n";
   cout << "   D  Display the board\n";
   cout << "   E  Edit one square\n";
   cout << "   S  Show the possible values for a square\n";
   cout << "   Q  Save and Quit\n";
   cout << endl;
   displayBoard(boardArray, greenArray);
   while (loop)
   {
      cin.ignore();
      cout << "\n> ";
      cin >> menuChoice;
      menuChoice = toupper(menuChoice);
      switch (menuChoice)
      {
         case '?':
            cout << "Options:\n";
            cout << "   ?  Show these instructions\n";
            cout << "   D  Display the board\n";
            cout << "   E  Edit one square\n";
            cout << "   S  Show the possible values for a square\n";
            cout << "   Q  Save and Quit\n";
            cout << endl;
            loop = true;
            break;
         case 'D':
            displayBoard(boardArray, greenArray);
            break;
         case 'E':
            editBoard(boardArray, greenArray);
            break;
         case 'S':
            computeValue(boardArray);
            break;
         case 'Q':
            loop = false;
            break;
         default:
            cout << "ERROR: Invalid command\n";
            loop = true;               
      }      
   }
   return;   
}

/***********************************************************************
 * SAVE FILE NAME
 *   This function will save the user's sudoku board to a file that
 *   the user will give the name of.
 **********************************************************************/
void saveFilename(int boardArray[][9], int greenArray[][9],
                  int numberOfSaves)
{
   char saveFilename[256];
   bool save = false;
   numberOfSaves = numberOfSaves + 1;
   ofstream foutSave("saves.txt");
   if (foutSave.fail())
      cout << "ERROR: Couldn't save saves.txt\n";
   foutSave << numberOfSaves;
   foutSave.close();
   cout << "What file would you like to write your board to: ";
   cin >> saveFilename;
   ofstream fout;
   while (save == false)
   {
      fout.open(saveFilename);           
      if (fout.fail())
      {
         save = false;
         cout << "ERROR! Cannot save file!\n";
         cout << "Reenter file name: ";
         cin >> saveFilename;
      }
      else
         save = true;
   }
   for (int row = 0; row < 9; row++)
   {
      for (int col = 0; col < 9; col++)
      {
         
         fout << boardArray[row][col];         
         fout << (col == 8 ? "\n" : " ");
      }      
   }
   for (int row = 0; row < 9; row++)
   {
      for (int col = 0; col < 9; col++)
      {
         greenArray[row + 9][col] = greenArray[row][col];
      }      
   }
   for (int row = 9; row < 18; row++)
   {
      for (int col = 0; col < 9; col++)
      {
         fout << greenArray[row][col];
         fout << (col == 8 ? "\n" : " ");
      }      
   }   
   fout.close();
   cout << "Board written successfully\n";
   return;
}

/********************************************************************
 * DISPLAY BOARD
 *   This function will display the current sudoku board on the
 *   screen.
 ******************************************************************/
void displayBoard(int boardArray[][9], int greenArray[][9])
{
   cout << "   A B C D E F G H I\n";
   for (int row = 0; row < 9; row++)
   {
      if (row == 3 || row == 6)
         cout << "   -----+-----+-----\n" ;
      cout << row + 1 << "  ";
      for (int col = 0; col < 9; col++)
      {
         if (boardArray[row][col] == 0)
         {
            cout << " ";
         }
         else
            if (boardArray[row][col] == greenArray[row][col])
               cout << system("Color 02") << boardArray[row][col]
                    << system("Color 07");
            else
               cout << boardArray[row][col];
         if (col != 8)
            cout << ((col == 2 || col == 5) ? "|" : " ");
         else
            cout << "\n";         
      }      
   }   
}

/***********************************************************************
 * EDIT BOARD
 *   This function will allow the user to edit the sudoku board.  The
 *   user will pick a coordinate on the board and if it is available
 *   they will choose a number to put in that spot.  The function will
 *   call 3 other functions to see if the number can be used.  If it
 *   can, then the function will add the number to the right spot
 *   in the array.  If the number cannot be used, then the function
 *   will display a message and return to the previous function.
 **********************************************************************/
void editBoard(int boardArray[][9], int greenArray[][9])
{
   bool loop = true;
   bool setValueSq = true;
   bool setValueR  = true;
   bool setValueC  = true;
   char coordinates[2];
   int value = 0;
   int cRow = 0;
   int cCol = 0;
   while (loop)
   {
      cout << "What are the coordinates of the square: ";
      cin >> coordinates;
      if (isdigit(coordinates[0]))
      {
         char temp;
         temp = coordinates[0];
         coordinates[0] = coordinates[1];
         coordinates[1] = temp;
      }
      if (isalpha(coordinates[0]) && isdigit(coordinates[1]))
      {        
         loop = false;
      }
      else
      {
         cout << "ERROR: Square \'" << coordinates[0] << coordinates[1]
              << "\' is invalid\n";
         loop = true;
      }
   }
   coordinates[0] = toupper(coordinates[0]);
   switch (coordinates[0])
   {
      case 'A':
         cCol = 0;
         break;
      case 'B':
         cCol = 1;
         break;
      case 'C':
         cCol = 2;
         break;
      case 'D':
         cCol = 3;
         break;
      case 'E':
         cCol = 4;
         break;
      case 'F':
         cCol = 5;
         break;
      case 'G':
         cCol = 6;
         break;
      case 'H':
         cCol = 7;
         break;
      case 'I':
         cCol = 8;
         break;
      default:
         cout << "ERROR: Square \'" << coordinates[0] << coordinates[1]
              << "\' is invalid\n";
         return;
   }
   switch (coordinates[1])
   {
      case '1':
         cRow = 0;
         break;
      case '2':
         cRow = 1;
         break;
      case '3':
         cRow = 2;
         break;
      case '4':
         cRow = 3;
         break;
      case '5':
         cRow = 4;
         break;
      case '6':
         cRow = 5;
         break;
      case '7':
         cRow = 6;
         break;
      case '8':
         cRow = 7;
         break;
      case '9':
         cRow = 8;
         break;
      default:
         cout << "ERROR: Square \'" << coordinates[0] << coordinates[1]
              << "\' is invalid";
         return;
   }
   if (boardArray[cRow][cCol] != 0)
   {
      if (boardArray[cRow][cCol] == greenArray[cRow][cCol])
      {
         cout << "ERROR: Square \'" << coordinates[0]
              << coordinates[1] << "\' is read-only\n";
         return;
      }
   }
   cout << "What is the value at \'" << coordinates[0]
        << coordinates[1] << "\': ";
   cin >> value;
   if (value > 9 || value < 1)
   {
      cout << "ERROR: Value \'" << value << "\' in square \'"
           << coordinates[0] << coordinates[1] << "\' is invalid\n";
      return;
   }
   setValueSq = checkSquare(boardArray, value, cRow, cCol);
   setValueR  = checkRow(boardArray, value, cRow, cCol);
   setValueC  = checkColumn(boardArray, value, cRow, cCol);
   if (setValueSq == true && setValueR == true && setValueC == true)
      boardArray[cRow][cCol] = value;
   else
      cout << "ERROR: Value \'" << value << "\' in square \'"
           << coordinates[0] << coordinates[1] << "\' is invalid\n";
   return;   
}

/***********************************************************************
 * COMPUTE VALUE
 *    This function will check a spot on the board to see which values
 *    can be used in a certain coordinate.  The function will get the
 *    coordinates from the user and then call 3 functions to see
 *    which numbers can go in the coordinate.  It will display the
 *    results on the screen.
 **********************************************************************/
void computeValue(int boardArray[][9])
{
   int displayCount = 1;
   bool displayValueSq = true;
   bool displayValueR  = true;
   bool displayValueC  = true;
   char coordinates[2];
   int value = 0;
   int cRow = 0;
   int cCol = 0;
   cout << "What are the coordinates of the square: ";
   cin >> coordinates;
   if (isdigit(coordinates[0]))
   {
      char temp;
      temp = coordinates[0];
      coordinates[0] = coordinates[1];
      coordinates[1] = temp;
   }
   coordinates[0] = toupper(coordinates[0]);
   switch (coordinates[0])
   {
      case 'A':
         cCol = 0;
         break;
      case 'B':
         cCol = 1;
         break;
      case 'C':
         cCol = 2;
         break;
      case 'D':
         cCol = 3;
         break;
      case 'E':
         cCol = 4;
         break;
      case 'F':
         cCol = 5;
         break;
      case 'G':
         cCol = 6;
         break;
      case 'H':
         cCol = 7;
         break;
      case 'I':
         cCol = 8;
         break;
      default:
         cout << "ERROR: Square \'" << coordinates[0] << coordinates[1]
              << "\' is invalid";
         return;
   }
   switch (coordinates[1])
   {
      case '1':
         cRow = 0;
         break;
      case '2':
         cRow = 1;
         break;
      case '3':
         cRow = 2;
         break;
      case '4':
         cRow = 3;
         break;
      case '5':
         cRow = 4;
         break;
      case '6':
         cRow = 5;
         break;
      case '7':
         cRow = 6;
         break;
      case '8':
         cRow = 7;
         break;
      case '9':
         cRow = 8;
         break;
      default:
         cout << "ERROR: Square \'" << coordinates[0] << coordinates[1]
              << "\' is invalid";
   }
   if (boardArray[cRow][cCol] != 0)
   {
      cout << "ERROR: Square \'" << coordinates[0]
           << coordinates[1] << "\' is filled\n";
      return;
   }   
   cout << "The possible values for \'" << coordinates[0]
        << coordinates[1] << "\' are: ";
   for (int i = 1; i <= 9; i++)
   {
      displayValueSq = true;
      displayValueR  = true;
      displayValueC  = true;
      displayValueSq = checkSquare(boardArray, i, cRow, cCol);
      displayValueR  = checkRow(boardArray, i, cRow, cCol);
      displayValueC  = checkColumn(boardArray, i, cRow, cCol);
      if (displayValueSq == true && displayValueR == true &&
          displayValueC == true)
      {         
         if (displayCount == 1)
         {
            cout << i;
            displayCount = displayCount + 1;
         }
         else
         {
            cout << ", " << i;
            displayCount = displayCount + 1;
         }
      }
   }
   cout << endl;
   return;   
}

/************************************************************************
 *CHECK SQUARE
 *   This function will determine if the value sent to it is in the
 *   square area of the sudoku board.  It will return true if the number
 *   is not present and false if it is present.
 ***********************************************************************/
bool checkSquare(int boardArray[][9], int value, int cRow, int cCol)
{
   bool notFound = true;
   // Square 1
   if (cRow < 3 && cCol < 3)
   {
      for (int row = 0; row < 3; row++)
      {
         for (int col = 0; col < 3; col++)
         {
            if (value == boardArray[row][col])
               return false;
         }         
      }      
   }
   // Square 2
   if (cRow < 3 && (cCol > 2 && cCol < 6))
   {
      for (int row = 0; row < 3; row++)
      {
         for (int col = 3; col < 6; col++)
         {
            if (value == boardArray[row][col])
               return false;
         }         
      }      
   }
   // Square 3
   if (cRow < 3 && cCol > 5)
   {
      for (int row = 0; row < 3; row++ )
      {
         for (int col = 6; col < 9; col++)
         {
            if (value == boardArray[row][col])
               return false;
         }         
      }      
   }   
   // Square 4
   if ((cRow > 2 && cRow < 6) && (cCol < 3))
   {
      for (int row = 3; row < 6; row++)
      {
         for (int col = 0; col < 3; col++)
         {
            if (value == boardArray[row][col])
               return false;
         }         
      }      
   }
   // Square 5
   if ((cRow > 2 && cRow < 6) && (cCol > 2 && cCol < 6))
   {
      for (int row = 3; row < 6; row++)
      {
         for (int col = 3; col < 6; col++)
         {
            if (value == boardArray[row][col])
               return false;
         }         
      }      
   }
   // Square 6
   if ((cRow > 2 && cRow < 6) && cCol > 5)
   {
      for (int row = 3; row < 6; row++)
      {
         for (int col = 6; col < 9; col++)
         {
            if (value == boardArray[row][col])
               return false;
         }         
      }      
   }
   // Square 7
   if (cRow > 5 && cCol < 3)
   {
      for (int row = 6; row < 9; row++)
      {
         for (int col = 0; col < 3; col++)
         {
            if (value == boardArray[row][col])
               return false;
         }         
      }      
   }
   // Square 8
   if (cRow > 5 && (cCol > 2 && cCol < 6))
   {
      for (int row = 6; row < 9; row++)
      {
         for (int col = 3; col < 6; col++)
         {
            if (value == boardArray[row][col])
               return false;
         }         
      }      
   }
   // Square 9
   if ( cRow > 5 && cCol > 5)
   {
      for (int row = 6; row < 9; row++)
      {
         for (int col = 6; col < 9; col++)
         {
            if (value == boardArray[row][col])
               return false;
         }         
      }      
   }   
   return notFound;
}

/***********************************************************************
 * CHECK ROW
 *   This function will determine if the value given is in the same
 *   row on the sudoku board.  If the function found the same number
 *   it will return false.  If the value is not found, it will return
 *   true.
 **********************************************************************/
bool checkRow(int boardArray[][9], int value, int cRow, int cCol)
{
   bool notFound = true;
   for (int col = 0; col < 9; col++)
   {
      if (value == boardArray[cRow][col])
         return false;
   }   
   return notFound;
}

/*********************************************************************
 * CHECK COLUMN
 *   This function will determine if the given value is in the
 *   column of the sudoku board.  It will return true if the number
 *   is not present and false if it is present.
 ******************************************************************/
bool checkColumn(int boardArray[][9], int value, int cRow, int cCol)
{
   bool notFound = true;
   for (int row = 0; row < 9; row++)
   {
      if (value == boardArray[row][cCol])
         return false;
   }   
   return notFound;
}

/******************************************************************
 * GET COORDINATES
 *   This function will take the array row and column values and
 *   return the coordinates on the sudoku board.
 *****************************************************************/
char getCoordinates(int row, int col)
{
   char coor;
   switch (col)
   {
      case 0:
         coor = 'A';
         break;
      case 1:
         coor = 'B';
         break;
      case 2:
         coor = 'C';
         break;
      case 3:
         coor = 'D';
         break;
      case 4:
         coor = 'E';
         break;
      case 5:
         coor = 'F';
         break;
      case 6:
         coor = 'G';
         break;
      case 7:
         coor = 'H';
         break;
      case 8:
         coor = 'I';
         break;
      default:
         coor = 'X';
   }
      
   return coor;
   
}

/*************************************************************************
* CHECK ARRAY
*   This function will check the incoming array that is read from the
*   file to see if there is an other values in the array that is not
*   suppose to be there.  If the board fails the test the function will
*   return false.
**************************************************************************/
bool checkArray(int boardArray[][9], int value, int cRow, int cCol)
{
   bool notfound = true;
   int count = 0;
   // Check the square first.
   // Square 1
   if (cRow < 3 && cCol < 3)
   {
      for (int row = 0; row < 3; row++)
      {
         for (int col = 0; col < 3; col++)
         {
            if (value == boardArray[row][col])
               count = count + 1;
         }         
      }      
   }
   // Square 2
   if (cRow < 3 && (cCol > 2 && cCol < 6))
   {
      for (int row = 0; row < 3; row++)
      {
         for (int col = 3; col < 6; col++)
         {
            if (value == boardArray[row][col])
               count = count + 1;
         }         
      }      
   }
   // Square 3
   if (cRow < 3 && cCol > 5)
   {
      for (int row = 0; row < 3; row++ )
      {
         for (int col = 6; col < 9; col++)
         {
            if (value == boardArray[row][col])
               count = count + 1;
         }         
      }      
   }   
   // Square 4
   if ((cRow > 2 && cRow < 6) && (cCol < 3))
   {
      for (int row = 3; row < 6; row++)
      {
         for (int col = 0; col < 3; col++)
         {
            if (value == boardArray[row][col])
               count = count + 1;
         }         
      }      
   }
   // Square 5
   if ((cRow > 2 && cRow < 6) && (cCol > 2 && cCol < 6))
   {
      for (int row = 3; row < 6; row++)
      {
         for (int col = 3; col < 6; col++)
         {
            if (value == boardArray[row][col])
               count = count + 1;
         }         
      }      
   }
   // Square 6
   if ((cRow > 2 && cRow < 6) && cCol > 5)
   {
      for (int row = 3; row < 6; row++)
      {
         for (int col = 6; col < 9; col++)
         {
            if (value == boardArray[row][col])
               count = count + 1;
         }         
      }      
   }
   // Square 7
   if (cRow > 5 && cCol < 3)
   {
      for (int row = 6; row < 9; row++)
      {
         for (int col = 0; col < 3; col++)
         {
            if (value == boardArray[row][col])
               count = count + 1;
         }         
      }      
   }
   // Square 8
   if (cRow > 5 && (cCol > 2 && cCol < 6))
   {
      for (int row = 6; row < 9; row++)
      {
         for (int col = 3; col < 6; col++)
         {
            if (value == boardArray[row][col])
               count = count + 1;
         }         
      }      
   }
   // Square 9
   if ( cRow > 5 && cCol > 5)
   {
      for (int row = 6; row < 9; row++)
      {
         for (int col = 6; col < 9; col++)
         {
            if (value == boardArray[row][col])
               count = count + 1;
         }         
      }      
   }
   // Next check the row.
   for (int col = 0; col < 9; col++)
   {
      if (value == boardArray[cRow][col])
         count = count + 1;
   }
   // And finally check the column.
   for (int row = 0; row < 9; row++)
   {
      if (value == boardArray[row][cCol])
         count = count + 1;
   }
   // Now check the count.  It better be 3.   
   if (count == 3)
      notfound = true;
   else
      notfound = false;
   return notfound;
}
