import multiprocessing
import numpy as np

def read_matrix_from_file(filename):
    with open(filename, 'r') as f:
        content = f.read().splitlines()
    
    n = int(content[0].strip())

    # Handle double space separation
    matrix1 = [[float(num) for num in content[i + 1].strip().split(' ')] for i in range(n)]
    matrix2 = [[float(num) for num in content[i + n + 2].strip().split(' ')] for i in range(n)]
    
    return n, matrix1, matrix2

def calculate_element(matrix1, matrix2, i, j, n):
    return sum(matrix1[i][k] * matrix2[k][j] for k in range(n))

def worker(matrix1, matrix2, n, queue, start_row, end_row):
    result = [[0 for _ in range(n)] for _ in range(start_row, end_row)]
    for i in range(start_row, end_row):
        for j in range(n):
            result[i - start_row][j] = calculate_element(matrix1, matrix2, i, j, n)
    queue.put(result)

def matrix_multiplication_multiprocessing(matrix1, matrix2, n, num_workers):
    queue = multiprocessing.Queue()
    chunk_size = n // num_workers
    processes = []

    for i in range(num_workers):
        start_row = i * chunk_size
        end_row = n if i == num_workers - 1 else (i + 1) * chunk_size
        p = multiprocessing.Process(target=worker, args=(matrix1, matrix2, n, queue, start_row, end_row))
        processes.append(p)
        p.start()

    result = [[0 for _ in range(n)] for _ in range(n)]
    for i in range(num_workers):
        chunk = queue.get()
        result[i * chunk_size:(i + 1) * chunk_size] = chunk

    for p in processes:
        p.join()

    return result

if __name__ == "__main__":
    import sys
    from time import time

    filename = sys.argv[1]
    
    # Serial Execution
    n, matrix1, matrix2 = read_matrix_from_file(filename)
    start_time = time()
    result_serial = np.dot(matrix1, matrix2)
    serial_time = time() - start_time
    print("Serial execution time: {:.4f} seconds".format(serial_time))
    print(result_serial)
    
    # Multiprocessing Execution
    num_workers = multiprocessing.cpu_count()  # Use the number of CPU cores available
    start_time = time()
    result_parallel = matrix_multiplication_multiprocessing(matrix1, matrix2, n, num_workers)
    parallel_time = time() - start_time
    print("Parallel execution time (multiprocessing): {:.4f} seconds".format(parallel_time))
    print("result:", result_parallel)
    # Verify that the results are the same
    assert result_serial == result_parallel, "The results do not match!"
    print("Results match. Parallelization successful!")
