#include <stdio.h>

int main() {
    int neff = 5;
    int arr[] = {10, 2, 3, 4, 5};
    int sum = 0;

    __asm__ (
        "xor %%eax, %%eax\n\t"             // index set 0
        "xor %%edx, %%edx\n\t"             // sum set 0
        "L1:\n\t"                           // Loop label
        "add (%%ebx, %%eax, 4), %%edx\n\t" // Add arr[eax] to edx
        "inc %%eax\n\t"                    // eax++
        "cmp %%eax, %%ecx\n\t" 
        "jg L1\n\t"                        // if eax < neff
        "mov %%edx, %0\n\t"                // Move sum to the output variable
        : "=r" (sum)                       // output 
        : "b" (arr), "c" (neff)            // input 
        : "eax", "edx"                     
    );

    printf("%d\n", sum);
    return 0;
}
