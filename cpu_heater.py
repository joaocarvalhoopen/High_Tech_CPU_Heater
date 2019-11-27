#******************************************************************************
#*                                                                            *
#*  High Tech CPU Heater - Python w/ DLL shared library and AVX2 in parallel. *
#*                                                                            *
#******************************************************************************
#* Author: Joao Nuno Carvalho                                                 *
#* Date: 2019.11.27                                                           *
#* License: MIT Open Source License                                           *
#* Description: This is a simple software version of a Rube Goldberg Machine! *
#*              It was made to learn and to have fun while doing it.          *
#*              This is a CPU heater, but a really high tech version that has *
#* many technologies to achieve the processing of 16 threads simultaneous     *
#* each with 32 sums of 8 bits each in each clock. They are AVX2 256 bit      *
#* single instruction multiple data instructions.                             *
#* Although the code is small and simple.                                     *
#*                                                                            *
#* The architecture of the program is:                                        *
#*     Python using CTypes calls parallel and Single Instructions Multiple    *
#*     Data DLL in C that has inside Assembly instructions to determine the   *
#*     CPU capabilities.                                                      *
#*                                                                            *
#* This program uses the following technologies:                              *
#*   -Assembly for detecting if the CPU has AVX2 instructions and for         *
#*    detecting the number of logical cores.                                  *
#*   -C Programming with multi-threading and with AVX2 instructions using     *
#*    intrinsics.                                                             *
#*   -Make and GCC in the hybrid compilation of an shared library DLL with    *
#*    position independent code and a executable for the tests in C.          *
#*   -Python with CTypes wrapers to call the DLL shared library written in C  *
#*    and to process the heating timer.                                       *
#*                                                                            *
#* To install do:                                                             *
#*     make                                                                   *
#*     python cpu_heater.py                                                   *
#*                                                                            *
#******************************************************************************
#* References:                                                                *
#*   [Wikipedia page of CPUID assembly instruction]                           *
#*      (https://en.wikipedia.org/wiki/CPUID)                                 *
#*   [Using CPUID for SIMD Detection]                                         *
#*      (http://softpixel.com/~cwright/programming/simd/cpuid.php)            *
#*                                                                            *
#*   [Crunching Numbers with AVX and AVX2]                                    *
#*      (https://www.codeproject.com/Articles/874396/Crunching-Numbers-with-AVX-and-AVX)
#*                                                                            *
#*   [Python CTypes Tutorial part 1]                                          *
#*      (https://dbader.org/blog/python-ctypes-tutorial)                      *
#*   [Python CTypes Tutorial part 2]                                          *
#*      (https://dbader.org/blog/python-ctypes-tutorial-part-2)               *
#*   [ctypes — A foreign function library for Python]                         *
#*      (https://docs.python.org/3/library/ctypes.html)                       *
#*                                                                            *
#******************************************************************************


import ctypes
import datetime

# Load the shared library into c types.
lib_heater = ctypes.CDLL("./high_tech_cpu_heater.so")


# bool has_CPU_AVX2_instructions();
has_CPU_AVX2_instructions_func = lib_heater.has_CPU_AVX2_instructions
has_CPU_AVX2_instructions_func.restype = ctypes.c_bool

# Call the C function from the library
res = has_CPU_AVX2_instructions_func()
if (res):
    print("Pyhton: The CPU has AVX2 instructions!")
else:
    print("Pyhton: The CPU doesn't have AVX2 instructions!")
    exit()

# int getCPUNumOfLogicalCores();
getCPUNumOfLogicalCores_func = lib_heater.getCPUNumOfLogicalCores
getCPUNumOfLogicalCores_func.restype = ctypes.c_int

# Call the C function from the library
logical_cores = getCPUNumOfLogicalCores_func()
print("Pyhton: The CPU has ", logical_cores, " logical cores")


# void span_hot_threads(const int num_threads);
span_hot_threads_func = lib_heater.span_hot_threads
span_hot_threads_func.argtypes = [ctypes.c_int]

# Call the C function from the library
# void span_hot_threads_func(logical_cores);

while(True):
    # Ask for the number of minutes for the heater.
    minutes = input("Please enter the number of minutes for the heater? ") 
    if (minutes == "q" or minutes == "Q"):
        exit(0) 
    minutes = int(minutes)
    time_1 = datetime.datetime.now()
    time_2 = datetime.datetime.now()
    elapsedTime = time_2 - time_1
    while( elapsedTime.total_seconds() < 60 * minutes):
        span_hot_threads_func(logical_cores)
        time_2 = datetime.datetime.now()
        elapsedTime = time_2 - time_1
        
