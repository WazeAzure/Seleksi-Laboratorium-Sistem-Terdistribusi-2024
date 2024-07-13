#include <stdio.h>

int main() {
    int neff = 5;
    int arr[] = {1, 2, 3, 4, 5};
    int sum = 0;

    __asm__ (
        "xor %%eax, %%eax\n\t"           // Clear eax (index)
        "xor %%edx, %%edx\n\t"           // Clear edx (sum)

        "L1:\n\t"                         // Loop label
        "add (%1, %%eax, 4), %%edx\n\t"  // Add arr[eax] to edx
        "inc %%eax\n\t"                  // Increment eax
        "cmp %%eax, %3\n\t"              // Compare eax with neff
        "jl 1\n\t"                      // Jump to loop label if eax < neff
        "mov %%edx, %0\n\t"              // Move the sum to the output variable
        : "=r" (sum)                     // Output operands
        : "r" (arr), "r" (neff)          // Input operands
        : "eax", "edx"                   // Clobbered registers
    );

    printf("%d\n", sum); // Output the sum
    return 0;
}
