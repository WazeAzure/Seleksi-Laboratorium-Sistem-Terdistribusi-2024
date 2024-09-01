import java.io.*;
import java.util.concurrent.*;

public class Main {
    // Reading the matrix from a file
    public static double[][] readMatrixFromFile(String filename) throws IOException {
        BufferedReader reader = new BufferedReader(new FileReader(filename));
        int n = Integer.parseInt(reader.readLine().trim());
        double[][] matrix1 = new double[n][n];
        double[][] matrix2 = new double[n][n];

        // Reading the first matrix
        for (int i = 0; i < n; i++) {
            String[] elements = reader.readLine().trim().split("  ");
            for (int j = 0; j < n; j++) {
                matrix1[i][j] = Double.parseDouble(elements[j]);
            }
        }

        // Reading the second matrix
        for (int i = 0; i < n; i++) {
            String[] elements = reader.readLine().trim().split("  ");
            for (int j = 0; j < n; j++) {
                matrix2[i][j] = Double.parseDouble(elements[j]);
            }
        }

        reader.close();
        return new double[][]{matrix1, matrix2};
    }

    // Serial matrix multiplication
    public static double[][] matrixMultiplicationSerial(double[][] matrix1, double[][] matrix2, int n) {
        double[][] result = new double[n][n];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < n; k++) {
                    result[i][j] += matrix1[i][k] * matrix2[k][j];
                }
            }
        }
        return result;
    }

    // Parallel matrix multiplication task
    static class MatrixMultiplicationTask extends RecursiveTask<double[][]> {
        private final double[][] matrix1;
        private final double[][] matrix2;
        private final double[][] result;
        private final int startRow, endRow, n;

        public MatrixMultiplicationTask(double[][] matrix1, double[][] matrix2, double[][] result, int startRow, int endRow, int n) {
            this.matrix1 = matrix1;
            this.matrix2 = matrix2;
            this.result = result;
            this.startRow = startRow;
            this.endRow = endRow;
            this.n = n;
        }

        @Override
        protected double[][] compute() {
            if (endRow - startRow <= n / Runtime.getRuntime().availableProcessors()) {
                for (int i = startRow; i < endRow; i++) {
                    for (int j = 0; j < n; j++) {
                        for (int k = 0; k < n; k++) {
                            result[i][j] += matrix1[i][k] * matrix2[k][j];
                        }
                    }
                }
            } else {
                int mid = (startRow + endRow) / 2;
                MatrixMultiplicationTask task1 = new MatrixMultiplicationTask(matrix1, matrix2, result, startRow, mid, n);
                MatrixMultiplicationTask task2 = new MatrixMultiplicationTask(matrix1, matrix2, result, mid, endRow, n);
                invokeAll(task1, task2);
            }
            return result;
        }
    }

    // Parallel matrix multiplication
    public static double[][] matrixMultiplicationParallel(double[][] matrix1, double[][] matrix2, int n) {
        double[][] result = new double[n][n];
        ForkJoinPool pool = new ForkJoinPool();
        MatrixMultiplicationTask task = new MatrixMultiplicationTask(matrix1, matrix2, result, 0, n, n);
        pool.invoke(task);
        pool.shutdown();
        return result;
    }

    public static void main(String[] args) throws IOException {
        if (args.length != 1) {
            System.out.println("Usage: java MatrixMultiplication <input file>");
            return;
        }

        String filename = args[0];

        // Read matrices from file
        double[][][] matrices = readMatrixFromFile(filename);
        double[][] matrix1 = matrices[0];
        double[][] matrix2 = matrices[1];
        int n = matrix1.length;

        // Serial Execution
        long startTime = System.nanoTime();
        double[][] resultSerial = matrixMultiplicationSerial(matrix1, matrix2, n);
        long serialTime = System.nanoTime() - startTime;
        System.out.println("Serial execution time: " + (serialTime / 1e9) + " seconds");

        // Parallel Execution
        startTime = System.nanoTime();
        double[][] resultParallel = matrixMultiplicationParallel(matrix1, matrix2, n);
        long parallelTime = System.nanoTime() - startTime;
        System.out.println("Parallel execution time: " + (parallelTime / 1e9) + " seconds");

        // Verify that the results are the same
        boolean match = true;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (resultSerial[i][j] != resultParallel[i][j]) {
                    match = false;
                    break;
                }
            }
        }
        System.out.println("Results match: " + match);
    }
}
