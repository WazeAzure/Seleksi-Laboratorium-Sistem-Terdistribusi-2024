#include <stdio.h>

int main() {
    int neff = 5;
    int arr[] = {10, 2, 3, 4, 5};
    int min = 0;
    int max = 0;

    __asm__ (
        "mov (%%ebx), %%eax\n\t"            // load first var
        "mov %%eax, %%edx\n\t"              // edx - min - initialize
        "mov %%eax, %%ecx\n\t"              // ecx - max - initialize
        "xor %%eax, %%eax\n\t"              // eax - index - 0
        
        "L1:\n\t"
        "cmp (%%ebx, %%eax, 4), %%edx\n\t"  // compare current el with min
        "jge L2\n\t"
        "mov (%%ebx, %%eax, 4), %%edx\n\t"
        
        "L2:\n\t"
        "cmp (%%ebx, %%eax, 4), %%ecx\n\t"  // compare current el with max
        "jle L3\n\t"
        "mov (%%ebx, %%eax, 4), %%ecx\n\t"  // compare current el with max
        
        "L3:\n\t"
        "inc %%eax\n\t"         // idx++
        "cmp %%eax, %%esi\n\t"
        "jg L1\n\t"
        
        "mov %%edx, %0\n\t"     // mov edx to min
        "mov %%ecx, %1\n\t"     // mov edx to max
        : "=r" (min), "=r" (max)           // output 
        : "b" (arr), "S" (neff)            // input 
        : "eax", "edx", "ecx"                     
    );

    printf("%d %d\n", min, max);
    return 0;
}
