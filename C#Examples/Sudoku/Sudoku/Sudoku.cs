using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Sudoku
{
    public class Sudoku
    {
        public bool IsSudokuValid(int[,] sudoku)
        {
            List<Task<bool>> tasks = new();
            tasks.Add(new Task<bool>(() => ColumnsValidator(sudoku)));
            tasks.Add(new Task<bool>(() => RowsValidator(sudoku)));
            for (int i = 0; i < 3; i++)
            {
                int index = i;
                tasks.Add(new Task<bool>(() => TrippleSquareValidator(sudoku, index)));
            }
            foreach (var t in tasks)
                t.Start();
            Task.WaitAll(tasks.ToArray());
            foreach(var t in tasks)
            {
                if (!t.Result)
                {
                    return false;
                }
            }
            return true;
        }

        private bool TrippleSquareValidator(int[,] sudoku, int sudokuRowNum)
        {
            for (int i = sudokuRowNum * 3; i < (sudokuRowNum + 1) * 3; i += 3)
            {
                for (int j = 0; j < sudoku.GetLength(1); j += 3)
                {
                    Dictionary<int, int> squareDisctionary = new();
                    for (int sqi = i; sqi < i + 3; sqi++)
                    {
                        for (int sqj = j; sqj < j + 3; sqj++)
                        {
                            int currentElement = sudoku[sqi, sqj];
                            if (squareDisctionary.ContainsKey(currentElement))
                            {
                                squareDisctionary[currentElement]++;
                            }
                            else
                            {
                                squareDisctionary[currentElement] = 1;
                            }
                        }
                    }
                    if (!IsDictionaryUniq(squareDisctionary))
                    {
                        return false;
                    }
                }
            }

            return true;
        }

        private bool ColumnsValidator(int[,] sudoku)
        {
            for (int i = 0; i < sudoku.GetLength(0); i++)
            {
                Dictionary<int, int> columnsDictionary = new();

                for (int j = 0; j < sudoku.GetLength(1); j++)
                {
                    int currentElement = sudoku[j, i];
                    if (columnsDictionary.ContainsKey(currentElement))
                    {
                        columnsDictionary[currentElement]++;
                    }
                    else
                    {
                        columnsDictionary[currentElement] = 1;
                    }
                }
                if (!IsDictionaryUniq(columnsDictionary))
                {
                    return false;
                }
            }
            return true;
        }

        private bool RowsValidator(int[,] sudoku)
        {
            for (int i = 0; i < sudoku.GetLength(0); i++)
            {
                Dictionary<int, int> rowDictionary = new();

                for (int j = 0; j < sudoku.GetLength(1); j++)
                {
                    int currentElement = sudoku[i, j];
                    if (rowDictionary.ContainsKey(currentElement))
                    {
                        rowDictionary[currentElement]++;
                    }
                    else
                    {
                        rowDictionary[currentElement] = 1;
                    }
                }
                if (!IsDictionaryUniq(rowDictionary))
                {
                    return false;
                }
            }
            return true;
        }

        private static bool IsDictionaryUniq(Dictionary<int, int> dictionady)
        {
            return !dictionady.Select(el => new
            {
                key = el.Key,
                value = el.Value
            }).Where(n => n.key != 0 && n.value > 1).Any();
        }
    }
}
