import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.*;

public class Example1 {
    public static void main(String[] args) {
        int rows = 100;
        int columns = 1000000;
        int[][] matrix = createAndFillMatrix(rows, columns);
        long startTime = System.nanoTime();
        int result = matrixMaxValueParallel(matrix);
        long stopTime = System.nanoTime();
        System.out.println(result);
        System.out.println((stopTime - startTime)/1e6 + "ms");
    }

    public static int matrixMaxValueParallel(int[][] matrix) {
        try {
            ExecutorService executor = Executors.newFixedThreadPool(8);
            List<Callable<Integer>> tasks = new ArrayList<>();

            for (int[] row : matrix) {
                Callable<Integer> task = () -> getRowMaxValue(row);
                tasks.add(task);
            }
            List<Future<Integer>> futures = executor.invokeAll(tasks);
            List<Integer> integers = new ArrayList<>();
            for (Future<Integer> future : futures) {
                integers.add(future.get());
            }
            executor.shutdown();
            return Collections.max(integers);
        } catch (InterruptedException | ExecutionException e) {
            e.printStackTrace();
        }
        return Integer.MIN_VALUE;
    }

    private static int getRowMaxValue(int[] row) {
        int max = Integer.MIN_VALUE;
        for (int number : row) {
            if (number > max) {
                max = number;
            }
        }
        return max;
    }

    public static int[][] createAndFillMatrix(int rows, int columns) {
        int[][] matrix = new int[rows][columns];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                matrix[i][j] = (int) (Math.random() * (1000000 + 1));
            }
        }
        return matrix;
    }
}
