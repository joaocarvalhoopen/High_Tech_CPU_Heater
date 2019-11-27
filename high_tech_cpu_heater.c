/******************************************************************************
 *                                                                            *
 *  High Tech CPU Heater - Generates executable and DLL (shared library)      *
 *                                                                            *
 ******************************************************************************
 * Author: Joao Nuno Carvalho                                                 *
 * Date: 2019.11.27                                                           *
 * License: MIT Open Source License                                           *
 * Description: This is a simple software version of a Rube Goldberg Machine! *
 *              It was made to learn and to have fun while doing it.          *
 *              This is a CPU heater, but a really high tech version that has *
 * many technologies to achieve the processing of 16 threads simultaneous     *
 * each with 32 sums of 8 bits each in each clock. They are AVX2 256 bit      *
 * single instruction multiple data instructions.                             *
 * Although the code is small and simple.                                     *
 *                                                                            *
 * The architecture of the program is:                                        *
 *     Python using CTypes calls parallel and Single Instructions Multiple    *
 *     Data DLL in C that has inside Assembly instructions to determine the   *
 *     CPU capabilities.                                                      *
 *                                                                            *
 * This program uses the following technologies:                              *
 *   -Assembly for detecting if the CPU has AVX2 instructions and for         *
 *    detecting the number of logical cores.                                  *
 *   -C Programming with multi-threading and with AVX2 instructions using     *
 *    intrinsics.                                                             *
 *   -Make and GCC in the hybrid compilation of an shared library DLL with    *
 *    position independent code and a executable for the tests in C.          *
 *   -Python with CTypes wrapers to call the DLL shared library written in C  *
 *    and to process the heating timer.                                       *
 *                                                                            *
 * To install do:                                                             *
 *     make                                                                   *
 *     python cpu_heater.py                                                   *
 *                                                                            *
 ******************************************************************************
 * References:                                                                *
 *   [Wikipedia page of CPUID assembly instruction]                           *
 *      (https://en.wikipedia.org/wiki/CPUID)                                 *
 *   [Using CPUID for SIMD Detection]                                         *
 *      (http://softpixel.com/~cwright/programming/simd/cpuid.php)            *
 *                                                                            *
 *   [Crunching Numbers with AVX and AVX2]                                    *
 *      (https://www.codeproject.com/Articles/874396/Crunching-Numbers-with-AVX-and-AVX)
 *                                                                            *
 *   [Python CTypes Tutorial part 1]                                          *
 *      (https://dbader.org/blog/python-ctypes-tutorial)                      *
 *   [Python CTypes Tutorial part 2]                                          *
 *      (https://dbader.org/blog/python-ctypes-tutorial-part-2)               *
 *   [ctypes — A foreign function library for Python]                         *
 *      (https://docs.python.org/3/library/ctypes.html)                       *
 *                                                                            *
 ******************************************************************************
*/


#include <stdio.h>
#include <stdbool.h>
#include <immintrin.h>
#include <threads.h>

// Declarations.
void printBits(size_t const size, void const * const ptr);

bool has_CPU_AVX2_instructions();

int getCPUNumOfLogicalCores();

int hot_processing(void *arg);

void span_hot_threads(const int num_threads);

// Code definitions.
int main(){

    if (has_CPU_AVX2_instructions()){
        printf("The CPU has AVX2 instructions!\n");
    }else{
        printf("The CPU doesn't have AVX2 instructions!\n");
    }

    int logical_cores = getCPUNumOfLogicalCores();
    printf("Logical cpus: %d \n", logical_cores);

    // hot_processing(NULL);

    int num_threads = logical_cores;
    span_hot_threads(num_threads);

    return 0;
}

void printBits(size_t const size, void const * const ptr){
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;

    for (int i=size-1;i>=0;i--)
    {
        for (int j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

/*
    [Wikipedia page of CPUID assembly instruction](https://en.wikipedia.org/wiki/CPUID)
    [Using CPUID for SIMD Detection](http://softpixel.com/~cwright/programming/simd/cpuid.php)
*/
bool has_CPU_AVX2_instructions(){
    int func, eax, ebx, ecx, edx;

    // Extended features input for CPUID instruction.
    func = 0x80000007;
    func = 0x7;
    ecx = 0x0;

    __asm__ __volatile__ ("cpuid":
                  "=a" (eax),   // EAX into a (output)
                  "=b" (ebx),   // EBX into b (output)
                  "=c" (ecx),   // ECX into c (output)
                  "=d" (edx) :  // EDX into d (output)
                  "a"  (func),  // a into EAX (input)
                  "c"  (ecx)    // c into ECX (input)              
                  ); 

    // Debug: 
    // printf("The code func: %i gives eax: %i, ebx: %i, ecx: %i, edx: %i \n", func, eax, ebx, ecx, edx);
    // printBits(sizeof(ebx), &ebx);
    
    // AVX2 instruction support is the 5 bit in the EBX register in the extended features with
    // the func input 7, use func = 0x80000007 .
    if (((ebx >> 5) & 0x1) == 0x1)
        return true;
    else
        return false;
}

int getCPUNumOfLogicalCores(){
    int func, eax, ebx, ecx, edx;

    // Extended features input for CPUID instruction.
    func = 0x00000001;

    __asm__ __volatile__ ("cpuid":
                  "=a" (eax),   // EAX into a (output)
                  "=b" (ebx),   // EBX into b (output)
                  "=c" (ecx),   // ECX into c (output)
                  "=d" (edx) :  // EDX into d (output)
                  "a"  (func)); // a into EAX (input)

  // Logical core count per CPU is in EBX register between bits 23 and 16. 
  int logical_cores = (ebx >> 16) & 0xff; // EBX[23:16]
  // printf("Logical cpus: %d \n", logical_cores);
  return logical_cores;
}

/*
    [Crunching Numbers with AVX and AVX2](https://www.codeproject.com/Articles/874396/Crunching-Numbers-with-AVX-and-AVX)
*/
int hot_processing(void *arg){

    // 32 numbers of 8 bits each.
    __m256i int_vector_a = _mm256_set_epi8(0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0);
    __m256i int_vector_b = _mm256_set_epi8(1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                           1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                           1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                           1, 1);
    
    __m256i result = int_vector_a;  
    

    /*
    unsigned char *ptr = (unsigned char*)&int_vector_b;
    for(int i=0; i<32; ++i){
        printf("%d ", ptr[i]);    
    }
    printf("\n");
    */

    // unsigned long int max_num = (256L * 4000000L * 4L * 5L) / 10L; // Ruffly 1 second at 4GHZ
    
    unsigned long int max_num = 82000000002;
    // printf("max_num: %ld \n", max_num);
    
    // Adds 32 values each with 8 bits, from 0 to 255 and then goes up to zero (overflow) in a cycle.    
    for(unsigned long int i=0; i<max_num; ++i ){
        result = _mm256_add_epi8(result, int_vector_b);
        result = _mm256_add_epi8(result, int_vector_b);
        result = _mm256_add_epi8(result, int_vector_b);
        result = _mm256_add_epi8(result, int_vector_b);
        result = _mm256_add_epi8(result, int_vector_b);
        result = _mm256_add_epi8(result, int_vector_b);
        result = _mm256_add_epi8(result, int_vector_b);
        result = _mm256_add_epi8(result, int_vector_b);
        result = _mm256_add_epi8(result, int_vector_b);
        result = _mm256_add_epi8(result, int_vector_b);
    }

    unsigned char *ptr = (unsigned char*)&result;
    /*
    for(int i=0; i<32; ++i){
        printf("%d ", ptr[i]);    
    }
    printf("\n");
    */

    // return 1;
    // I'am returning the value so that the compiler doesn't optimize the code away.
    return (int) ptr[0];
} 

void span_hot_threads(const int num_threads){
    thrd_t thread[num_threads];
    int result[num_threads];

    for(int i=0; i<num_threads; ++i){
        thrd_create(&thread[i], hot_processing, NULL);
    }

    for(int i=0; i<num_threads; ++i){
        thrd_join(thread[i], &result[i]);
    }

    /*
    for(int i=0; i<num_threads; ++i){
        printf("Thread return %d at the end\n", result[i]);
    }
    */
}
