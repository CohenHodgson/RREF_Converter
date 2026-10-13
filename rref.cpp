/*
gauss jordan method

1. find a row at or below lead with a nonzero entry in this column, if none exist, move onto next column
2. swap that row up to lead
3. divide row by its pivot so pivot becomes 1
4. subtract multiples of it from every other row so the rest of the column becomes 0
5. lead + 1 (move onto next potential pivot point)

compile: g++ -std=c++17 rref.cpp
run: ./rref (inside dir)
*/

#include <cmath> // fabs, abs val of decimal
#include <cstdlib> // exit for 
#include <iomanip> // formatting (setw and setprecision)
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

typedef vector<vector<double>> Matrix; // alias = Matrix, vector of rows, so m[row][col] = one entry in matrix

/*
   rounding errors (like 0.0000000001 instead of 0),
   so anything this close to zero just gets treated as zero

   note: rounding errors still accumulate in matrix, hence why we use double
   instead of float (even if float would be more efficient)
*/

const double small = 1e-9;

bool isZero(double x) { // returns 0 if smaller than small
    return fabs(x) < small;
}

/*
   reads one whole line and grabs the numbers out of it.
   if there aren't exactly numCols numbers, it asks again
*/

vector<double> readRow(int rowNumber, int numCols) {
    while (true) {
        cout << "row " << rowNumber << ": ";

        string line;

        if (!getline(cin, line)) { // if input failed (letters)
            cout << "\n Please input numbers and try again. \n";
            exit(1);
        }

        stringstream lineStream(line); // let's us pull numbers out of the line one at a time with >>
        vector<double> row;
        double number;
        while (lineStream >> number) { // >> grabs next num, stop at end of row or at letter/non-num
            row.push_back(number);
        }

        if ((int)row.size() == numCols) { // right amount of numbers, good to go
            return row;
        }
        else {
            cout << "need exactly " << numCols << " numbers, try that row again\n";
        }
    }
}

Matrix readMatrix() { // where user inputs matrix nums
    int numRows, numCols;

    cout << "how many rows? ";
    cin >> numRows;
    cout << "how many columns? ";
    cin >> numCols;
    cin.ignore(1000, '\n'); // throw away the left over new line
    /*
    type: 3 enter
    buffer: 3 \n
    cin >> numCols takes 3 and leaves \n

    this is the step that cin.ignore fixes:
    getline sees \n and returns empty line

    so that an empty row isn't entered every time you fill in one row.
    */

    Matrix m;

    cout << "type each row with spaces between the numbers\n";
    for (int row = 0; row < numRows; row++) {
        m.push_back(readRow(row + 1, numCols));
    }
    return m;
}

void printMatrix(const Matrix& m) {
    for (int row = 0; row < (int)m.size(); row++) { // so long as row is less than the size of matrix, row++
        for (int col = 0; col < (int)m[row].size(); col++) { // see above
            cout << setw(9) << setprecision(5) << m[row][col]; // iomanip options, let's columns be aligned for user input (setw) and sets amount of sig digs (setprecision)
        }
        cout << "\n";
    }
}


/* 
the three row ops in real num field. Later on: make field Zn an option also add sols., Ax = b.
*/
void swapRows(Matrix& m, int rowA, int rowB) { // swap two rows
    swap(m[rowA], m[rowB]);
}

void scaleRow(Matrix& m, int row, double factor) { // multiply every number in a row by some factor
    for (int col = 0; col < (int)m[row].size(); col++) {
        m[row][col] = m[row][col] * factor;
    }
}

void addMultipleOfRow(Matrix& m, int target, int source, double factor) { // target = target + (factor * source)
    for (int col = 0; col < (int)m[target].size(); col++) {
        m[target][col] = m[target][col] + factor * m[source][col];
    }
}

/*
   looks down a column (starting at startRow) for a number that isn't 0.
   gives back that row's number, or -1 if it's all zeros 
*/

int findPivotRow(const Matrix& m, int startRow, int col) { // take reference to matrix, start row and column
    for (int row = startRow; row < (int)m.size(); row++) { // then look through the row
        if (!isZero(m[row][col])) { // if elem is not 0
            return row;
        }
    }
    return -1;
}

void cleanUpZeros(Matrix& m) { // anything close to 0 = 0, also -0 -> 0
    for (int row = 0; row < (int)m.size(); row++) {
        for (int col = 0; col < (int)m[row].size(); col++) {
            if (isZero(m[row][col])) {
                m[row][col] = 0.0;
            }
        }
    }
}

void toRref(Matrix& m) { // makes a given matrix into RREF form
    int numRows = m.size();
    int numCols = m[0].size();

    int pivotRow = 0;  // where the next pivot (a 1) goes

    for (int col = 0; col < numCols; col++) {
        if (pivotRow >= numRows) {
            break;  // every row has a pivot already. We must be done
        }

        // step 1: anything usable in this column?
        int foundRow = findPivotRow(m, pivotRow, col);
        if (foundRow == -1) {
            continue;  // all zeros here, skip to the next column
        }

        // step 2: move that row into place
        swapRows(m, foundRow, pivotRow);

        // step 3: make the pivot 1
        double pivot = m[pivotRow][col];
        scaleRow(m, pivotRow, 1.0 / pivot);

        // step 4: zero out the rest of the column
        for (int row = 0; row < numRows; row++) {
            if (row == pivotRow) {
                continue;  // don't wipe out the pivot row itself
            }
            double factor = -m[row][col];
            addMultipleOfRow(m, row, pivotRow, factor);
        }

        pivotRow++;  // next pivot goes one row down
    }

    cleanUpZeros(m);
}


int main() {
    Matrix m = readMatrix();

    cout << "\nyour matrix:\n";
    printMatrix(m);

    toRref(m);

    cout << "\nrref:\n";
    printMatrix(m);

    return 0;
}
