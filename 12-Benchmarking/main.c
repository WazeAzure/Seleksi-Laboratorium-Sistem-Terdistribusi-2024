#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>

#define NUM_ITERATIONS 5
#define NUM_PROGRAMS 10

typedef struct {
    const char* command;
    double avg_time;
} BenchmarkData;

void* benchmark(void* arg) {
    BenchmarkData* data = (BenchmarkData*)arg;
    double total_time = 0;

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        clock_t start = clock();
        system(data->command);
        clock_t end = clock();
        total_time += (double)(end - start) / CLOCKS_PER_SEC;
    }

    data->avg_time = total_time / NUM_ITERATIONS;
    return NULL;
}

int main() {
    const char* programs[NUM_PROGRAMS] = {
        "./matrix_inverse_c input.txt output_c.txt",
        "python3 matrix_inverse.py input.txt output_py.txt",
        "java MatrixInverse input.txt output_java.txt",
        "./matrix_inverse_cpp input.txt output_cpp.txt",
        "ruby matrix_inverse.rb input.txt output_ruby.txt",
        "php matrix_inverse.php input.txt output_php.txt",
        "node matrix_inverse.js input.txt output_js.txt",
        "./matrix_inverse_go input.txt output_go.txt",
        "./matrix_inverse_pascal input.txt output_pascal.txt",
        "perl matrix_inverse.pl input.txt output_perl.txt"
    };

    pthread_t threads[NUM_PROGRAMS];
    BenchmarkData data[NUM_PROGRAMS];

    // Mulai setiap program di thread yang berbeda
    for (int i = 0; i < NUM_PROGRAMS; i++) {
        data[i].command = programs[i];
        pthread_create(&threads[i], NULL, benchmark, &data[i]);
    }

    // Tunggu setiap thread selesai
    for (int i = 0; i < NUM_PROGRAMS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Cetak hasil benchmarking
    for (int i = 0; i < NUM_PROGRAMS; i++) {
        printf("Program %d (%s) average time: %lf seconds\n", i + 1, data[i].command, data[i].avg_time);
    }

    return 0;
}
