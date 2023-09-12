#include "hamiltonian_path_grid.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

unsigned int num_threads;

std::istream& operator>>(std::istream& is, std::pair<int, int>& p) {
	is >> p.first >> p.second;
	return is;
}

std::ostream& operator<<(std::ostream& os, const std::pair<int, int>& p) {
	os << '(' << p.first << ", " << p.second << ')';
	return os;
}

inline bool out_of_bounds_L(int i, int j, int m, int n) {
	return i < 4*n - 4 and j >= m;
}

inline bool out_of_bounds_C(int i, int j, int m, int n) {
	return i >= n and i < 4*n - 4 and j >= m;
}

inline bool out_of_bounds_F(int i, int j, int m, int n) {
	return
	(i >= n and i < 2*n - 2 and j >= m)
	or (i >= 2*n - 2 and i < 3*n - 2 and j >= 3*m - 4)
	or (i >= 3*n - 2 and j >= m)
	;
}


inline bool out_of_bounds_E(int i, int j, int m, int n) {
	return
	(i >= n and i < 2*n - 2 and j >= m)
	or (i >= 2*n - 2 and i < 3*n - 2 and j >= 3*m - 4)
	or (i >= 3*n - 2 and i < 4*n - 4 and j >= m)
	;
}


void worker_thread(int thread_index, int number_of_points, int m, int n, const std::pair<int, int>* s, const std::pair<int, int>* t, int type1, std::vector<std::vector<std::pair<int, int>>>* v) {
	
	// za pravougaonik - ako nije krajnja nit, pocetna tacka se dobija kao:
	// ((thread_index * number_of_points) / n, (thread_index * number_of_points) % m)
	int iR = (thread_index * number_of_points) / m;
	int jR = (thread_index * number_of_points) % m;
	if(thread_index == num_threads - 1) {
		int tmp = m*n - number_of_points;
		iR = tmp / m;
		jR = tmp % m;
	}
	
	int m1 = (m + 2) / 3, n1 = (n + 4) / 5;
	
	// std::cout << iR << " " << jR << " " << thread_index << " " << number_of_points << '\n';
	
	for(int i = 0; i < number_of_points; ++i) {
		switch(type1) {
				case 0:
					(*v)[iR][jR] = find_hamiltonian_path(m, n, jR, iR, *s, *t);
					
					break;
				
				case 1:
					if(out_of_bounds_L(iR, jR, m1, n1)) {
						(*v)[iR][jR] = std::pair{-1, -1};
					} else {
						(*v)[iR][jR] = find_hamiltonian_path_L(m1, n1, jR, iR, *s, *t);
					}
					
					break;
				
				case 2:
					if(out_of_bounds_C(iR, jR, m1, n1)) {
						(*v)[iR][jR] = std::pair{-1, -1};
					} else {
						(*v)[iR][jR] = find_hamiltonian_path_C(m1, n1, jR, iR, *s, *t);
					}
					break;
				case 3:
					if(out_of_bounds_F(iR, jR, m1, n1)) {
						(*v)[iR][jR] = std::pair{-1, -1};
					} else {
						(*v)[iR][jR] = find_hamiltonian_path_F(m1, n1, jR, iR, *s, *t);
					}
					break;
				case 4:
					if(out_of_bounds_E(iR, jR, m1, n1)) {
						(*v)[iR][jR] = std::pair{-1, -1};
					} else {
						(*v)[iR][jR] = find_hamiltonian_path_E(m1, n1, jR, iR, *s, *t);
					}
					break;
				
			}
		++jR;
		if(jR >= m) {
			++iR;
			jR -= m;
		}
	}
}

int main() {

std::cout
	<< "Choose a grid graph:\n"
	<< "R : 0\nL : 1\nC : 2\nF : 3\nE : 4\n";
int type;
std::cin >> type;

if(type < 0 or type > 4) {
	std::cout << "Wrong type!\n";
	return -1;
}

int m, n;
std::pair<int, int> s, t;

std::cout << "m: ";
std::cin >> m;
std::cout << "n: ";
std::cin >> n;
std::cout << "s: ";
std::cin >> s;
std::cout << "t: ";
std::cin >> t;


std::cout << "To run the parallel algorithm, press 1\n";
bool parallel;

std::cin >> parallel;

int m1 = m, n1 = n;
if(type != 0) {
	m1 = 3*m - 2;
	n1 = 5*n - 4;
}

std::vector<std::vector<std::pair<int, int>>> v;
v.reserve(n1);

for(int i = 0; i < n1; ++i) {
	v[i].reserve(m1);
}

if(parallel) {
	std::cout << "Choose the number of threads\n";
	std::cin >> num_threads;
	std::vector<std::thread> threads;
	int number_of_points = m1 * n1 / num_threads;
	auto start = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < num_threads; ++i) {
		threads.push_back(std::thread(worker_thread, i, (i == num_threads - 1)? (m1 * n1 - number_of_points * (num_threads - 1)) : number_of_points, m1, n1, &s, &t, type, &v));
				
	}
	
	for(auto& t : threads) {
		t.join();
	}
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "Time spent: " << duration.count() << " microseconds\n";
	
} else {
	for(int i = 0; i < n1; ++i) {
		for(int j = 0; j < m1; ++j) {
			switch(type) {
				case 0:
					v[i][j] = find_hamiltonian_path(m, n, j, i, s, t);
					break;
				case 1:
					if(out_of_bounds_L(i, j, m, n)) {
						v[i][j] = std::pair{-1, -1};
					} else {
						v[i][j] = find_hamiltonian_path_L(m, n, j, i, s, t);
					}
					break;
				case 2:
					if(out_of_bounds_C(i, j, m, n)) {
						v[i][j] = std::pair{-1, -1};
					} else {
						v[i][j] = find_hamiltonian_path_C(m, n, j, i, s, t);
					}
					break;
				case 3:
					if(out_of_bounds_F(i, j, m, n)) {
						v[i][j] = std::pair{-1, -1};
					} else {
						v[i][j] = find_hamiltonian_path_F(m, n, j, i, s, t);
					}
					break;
				case 4:
					if(out_of_bounds_E(i, j, m, n)) {
						v[i][j] = std::pair{-1, -1};
					} else {
						v[i][j] = find_hamiltonian_path_E(m, n, j, i, s, t);
					}
					break;
			}
		}
	}
}

/*
for(int i = 0; i < n1; ++i) {
	for(int j = 0; j < m1; ++j) {
		std::cout << v[i][j] << " ";
	}
	std::cout << '\n';
}
*/
return 0;
}
