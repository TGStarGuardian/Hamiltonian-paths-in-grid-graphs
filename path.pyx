from libcpp cimport bool
from libcpp.pair cimport pair

cdef extern from "hamiltonian_path_grid.hpp":
	bool has_hamiltonian_cycle(int, int);
	
	bool has_hamiltonian_path(int m, int n, pair[int, int] s, pair[int, int] t);

	pair[int, int] find_hamiltonian_path(int m, int n, int x, int y, const pair[int, int]& s, const pair[int, int]& t);
	
	pair[int, int] find_hamiltonian_path_L(int m, int n, int x, int y, const pair[int, int]& s, const pair[int, int]& t);

	pair[int, int] find_hamiltonian_path_C(int m, int n, int x, int y, const pair[int, int]& s, const pair[int, int]& t);

	pair[int, int] find_hamiltonian_path_F(int m, int n, int x, int y, const pair[int,int]& s, const pair[int, int]& t);

	pair[int, int] find_hamiltonian_path_E(int m, int n, int x, int y, const pair[int, int]& s, const pair[int, int]& t);
	
cdef extern from "hamiltonian_path_grid.cpp":
	pass
	
def findHamiltonianPath(m, n, x, y, s, t):
	return find_hamiltonian_path(m, n, x, y, s, t)
	
def findHamiltonianCycle(m, n, x, y, s, t):
	return find_hamiltonian_path(m, n, x, y, s, s)
	
def findHamiltonianPathL(m, n, x, y, s, t):
	return find_hamiltonian_path_L(m, n, x, y, s, t)

def findHamiltonianCycleL(m, n, x, y, s, t):
	return find_hamiltonian_path_L(m, n, x, y, s, s)
	
def findHamiltonianPathC(m, n, x, y, s, t):
	return find_hamiltonian_path_C(m, n, x, y, s, t)

def findHamiltonianCycleC(m, n, x, y, s, t):
	return find_hamiltonian_path_C(m, n, x, y, s, s)
	
def findHamiltonianPathF(m, n, x, y, s, t):
	return find_hamiltonian_path_F(m, n, x, y, s, t)

def findHamiltonianCycleF(m, n, x, y, s, t):
	return find_hamiltonian_path_F(m, n, x, y, s, s)
	
def findHamiltonianPathE(m, n, x, y, s, t):
	return find_hamiltonian_path_E(m, n, x, y, s, t)
	
def findHamiltonianCycleE(m, n, x, y, s, t):
	return find_hamiltonian_path_E(m, n, x, y, s, s)	
	
	
	
