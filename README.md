# High Tech CPU Heater
Having fun with Python using CTypes with a DLL shared library in C and AVX2 instrinsics in parallel and using multi-threaded code with spots of assembly.

## Description
This is a simple software version of a Rube Goldberg Machine! <br>
It was made to learn and to have fun while doing it. <br>
<br>
This is a CPU heater, but a really high tech version that has many technologies to achieve the processing of 16 threads simultaneous each with 32 sums of 8 bits each in each clock. They are AVX2 256 bit single instruction multiple data instructions. <br>
Although the code is small and simple. <br>

## The architecture of the program is <br>
 * Python using CTypes calls parallel and Single Instructions Multiple Data DLL in C that has inside Assembly instructions to determine the CPU capabilities.
 * Running on Linux Ubuntu 19.10.

## This program uses the following technologies:<br>
* Assembly for detecting if the CPU has AVX2 instructions and for detecting the number of logical cores.
* C Programming with multi-threading and with AVX2 instructions using intrinsics.
* Make and GCC in the hybrid compilation of an shared library DLL with position independent code and a executable for the tests in C. 
* Python with CTypes wrapers to call the DLL shared library written in C and to process the heating timer. 

## To install do
```
make
python cpu_heater.py
```      
 
## References
[Wikipedia page of CPUID assembly instruction](https://en.wikipedia.org/wiki/CPUID) <br>
[Using CPUID for SIMD Detection](http://softpixel.com/~cwright/programming/simd/cpuid.php) <br>
[Crunching Numbers with AVX and AVX2](https://www.codeproject.com/Articles/874396/Crunching-Numbers-with-AVX-and-AVX) <br>
[Python CTypes Tutorial part 1](https://dbader.org/blog/python-ctypes-tutorial) <br>
[Python CTypes Tutorial part 2](https://dbader.org/blog/python-ctypes-tutorial-part-2) <br>
[CTypes — A foreign function library for Python](https://docs.python.org/3/library/ctypes.html) <br>

## License
MIT Open Source License <br>
<br>
Have fun! <br>
<br>
Best regards, <br>
Joao Nuno Carvalho <br>
