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

## print("Hello world")

print(has_hamiltonian_cycle(5, 4))
print(has_hamiltonian_cycle(5, 7))

for i in range(5):
	s = ""
	for j in range(5):
		y, x = find_hamiltonian_path(5, 5, j, i, (0, 0), (1, 1))
		s += "(" +  str(y) + ", " + str(x) + ")" + " "
	print(s)


s = (4, 4)
t = (1, 5)
m = 3
n = 3

print("E")

for i in range(n):
	output = ""
	for j in range(3*m - 2):
		y, x = find_hamiltonian_path_E(m, n, j, i, s, t)
		output += "(" +  str(y) + ", " + str(x) + ")" + " "
	print(output)

for i in range(n, 2*n - 2):
	output = ""
	for j in range(m):
		y, x = find_hamiltonian_path_E(m, n, j, i, s, t)
		output += "(" +  str(y) + ", " + str(x) + ")" + " "
	print(output)

for i in range(2*n - 2, 3*n - 2):
	output = ""
	for j in range(3*m - 4):
		y, x = find_hamiltonian_path_E(m, n, j, i, s, t)
		output += "(" +  str(y) + ", " + str(x) + ")" + " "
	print(output)

for i in range(3*n - 2, 4*n - 4):
	output = ""
	for j in range(m):
		y, x = find_hamiltonian_path_E(m, n, j, i, s, t)
		output += "(" +  str(y) + ", " + str(x) + ")" + " "
	print(output)
	
for i in range(4*n - 4, 5*n - 4):
	output = ""
	for j in range(3*m - 2):
		y, x = find_hamiltonian_path_E(m, n, j, i, s, t)
		output += "(" +  str(y) + ", " + str(x) + ")" + " "
	print(output)
		
		
		
		
		
		
		
		
		
		
		
		
		






