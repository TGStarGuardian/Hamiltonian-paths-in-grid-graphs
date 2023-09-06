from libcpp cimport bool
from libcpp.pair cimport pair

cdef extern from "hamiltonian_path_grid.hpp":
	bool has_hamiltonian_cycle(int, int);

	pair[int, int] find_hamiltonian_path(int m, int n, int x, int y, const pair[int, int]& s, const pair[int, int]& t);
	
cdef extern from "hamiltonian_path_grid.cpp":
	pass

## print("Hello world")

print(has_hamiltonian_cycle(5, 4))
print(has_hamiltonian_cycle(5, 7))

for i in range(5):
	s = ""
	for j in range(5):
		y, x = find_hamiltonian_path(5, 5, j, i, (0, 0), (1, 1))
		s += "(" +  str(y) + ", " + str(x) + ")" + " "
	print(s)










