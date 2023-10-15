# Hamiltonian paths in grid graphs

This repository contains code in C++ that generates Hamiltonian paths and Hamiltonian cycles in grid graphs of the following types:
  * rectangle
  * L
  * C
  * F
  * E

Hamiltonian path is a path where each vertex is traversed exactly once, starting at some vertex s and ending at some vertex t.
Hamiltonian cycle is a special case of Hamiltonian path where s = t.

In the mentioned types (see above), there exist sequential algorithms that calculate Hamiltonian paths in O(m*n) time, and parallel algorithms in O(1) time when the number of processors is close to the number of vertices.
Each parallel step in a parallel algorithm can be computed independently and thus no communication between processors is needed, which makes this solution scalable. The algorithms can be found in the following papers: https://doi.org/10.1016/S0167-8191(02)00135-7 and https://doi.org/10.1155/2012/475087

Files hamiltonian_path_grid.hpp and hamiltonian_path_grid.cpp contain the implementations of functions that calculate the next vertex in a path/cycle (a.k.a parallel steps), while main.cpp contains code that runs those steps in parallel and stores the path in a matrix. Note that find_hamiltonian_path functions can calculate both paths and cycles.

File path.pyx is a Cython file that binds the code in hamiltonian_path_grid.hpp and hamiltonian_path_grid.cpp with Python. In file main.py is code that generates GUI to demonstrate the algorithms.

## Compiling in C++
To compile this code, simply run:

  $ g++ -o main main.cpp hamiltonian_path_grid.cpp
  
  $ ./main

## Running GUI
Before running main.py, you'd need to install Cython and PyQt5. Follow the official instructions for these steps: https://cython.org/ and https://pypi.org/project/PyQt5/.

Having done this, run:

  $ python setup.py build_ext --inplace
  
  $ python main.py

