# RREF Converter

A C++ program that converts a matrix into Reduced Row Echelon Form (RREF) using the [Gauss–Jordan elimination method](https://en.wikipedia.org/wiki/Gaussian_elimination).

## Run

```bash
g++ -std=c++17 rref.cpp
./rref
```
When the program runs you will be prompted for the number of columns and rows of your input matrix, after which the program will do the row calculations and print the RREF.

## Method

The program follows these steps:

1. Find a nonzero entry in the current column at or below the pivot row. If none exists, move to the next column.
2. Swap the selected row into the pivot position.
3. Divide the pivot row by its pivot so the pivot becomes 1.
4. Subtract multiples of the pivot row from every other row to make the remaining entries in that column zero.
5. Move to the next pivot row and repeat.

The program uses the double datatype to represent matrix entries and a tolerance of 1e-9 (small) to handle the floating-point rounding errors.

## License

This project is licensed under the MIT License. 

See [LICENSE](LICENSE) for details.
