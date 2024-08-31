import numpy as np
from multiprocessing import Pool

def multiply_row_by_matrix(row, matrix):
    return np.dot(row, matrix)

def parallel_matrix_multiplication(A, B):
    with Pool() as pool:
        result = pool.starmap(multiply_row_by_matrix, [(row, B) for row in A])
    return np.array(result)

# Example matrices
A = np.random.rand(1000, 1000)
B = np.random.rand(1000, 1000)

# Perform parallel matrix multiplication
result = parallel_matrix_multiplication(A, B)
print(result)