using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;

namespace Sudoku
{
    class Program
    {
        static void Main(string[] args)
        {

            int[,] validSudoku = new int[9, 9] {
                        {1, 4, 7, 0, 0, 0, 0, 0, 3},
                        {2, 5, 0, 0, 0, 1, 0, 0, 0},
                        {3, 0, 9, 0, 0, 0, 0, 0, 0},
                        {0, 8, 0, 0, 2, 0, 0, 0, 4},
                        {0, 0, 0, 4, 1, 0, 0, 2, 0},
                        {9, 0, 0, 0, 0, 0, 6, 0, 0},
                        {0, 0, 3, 0, 0, 0, 0, 0, 9},
                        {4, 0, 0, 0, 0, 2, 0, 0, 0},
                        {0, 0, 1, 0, 0, 8, 0, 0, 7},
                };
            var watch = System.Diagnostics.Stopwatch.StartNew();
            var result = new Sudoku().IsSudokuValid(validSudoku);
            watch.Stop();
            Console.WriteLine("IsValid: " + result);
            Console.WriteLine((watch.ElapsedMilliseconds * 10 ^ 3) + " microseconds");
        }
    }
}
