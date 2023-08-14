#include "hamiltonian_path_grid.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

std::istream& operator>>(std::istream& is, std::pair<int, int>& p) {
	is >> p.first >> p.second;
	return is;
}

std::ostream& operator<<(std::ostream& os, const std::pair<int, int>& p) {
	os << '(' << p.first << ", " << p.second << ')';
	return os;
}

void worker_thread(const std::pair<int, int> start, int number_of_points, int m, int n, const std::pair<int, int>* s, const std::pair<int, int>* t, int type1, std::vector<std::vector<std::pair<int, int>>>* v) {
	
	auto current = start;
	for(int i = 0; i < number_of_points; ++i) {
		switch(type1) {
				case 0:
					(*v)[current.first][current.second] = find_hamiltonian_path(m, n, current.second, current.first, *s, *t);
					++current.second;
					if(current.second >= m) {
						++current.first;
						current.second -= m;
					}
					break;
				case 1:
					(*v)[current.first][current.second] = find_hamiltonian_path_L(m, n, current.second, current.first, *s, *t);
					break;
				case 2:
					(*v)[current.first][current.second] = find_hamiltonian_path_C(m, n, current.second, current.first, *s, *t);
					break;
				case 3:
					(*v)[current.first][current.second] = find_hamiltonian_path_F(m, n, current.second, current.first, *s, *t);
					break;
				case 4:
					(*v)[current.first][current.second] = find_hamiltonian_path_E(m, n, current.second, current.first, *s, *t);
					break;
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

std::vector<std::vector<std::pair<int, int>>> v;
v.reserve(n);

for(int i = 0; i < n; ++i) {
	v[i].reserve(m);
}

if(parallel) {
	std::cout << "Choose the number of threads\n";
	unsigned int num_threads;
	std::cin >> num_threads;
	std::vector<std::thread> threads;
	int number_of_points = m * n / num_threads;
	// auto start = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < num_threads; ++i) {
		threads.push_back(std::thread(worker_thread, std::pair{(number_of_points * i) / m, (number_of_points * i) % m}, (i == num_threads - 1)? (m*n - number_of_points * (num_threads - 1)) : number_of_points, m, n, &s, &t, type, &v));
				
	}
	
	for(auto& t : threads) {
		t.join();
	}
	// auto stop = std::chrono::high_resolution_clock::now();
	// auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	// std::cout << "Time spent: " << duration.count() << " microseconds\n";
	
} else {
	for(int i = 0; i < n; ++i) {
		for(int j = 0; j < m; ++j) {
			switch(type) {
				case 0:
					v[i][j] = find_hamiltonian_path(m, n, j, i, s, t);
					break;
				case 1:
					v[i][j] = find_hamiltonian_path_L(m, n, j, i, s, t);
					break;
				case 2:
					v[i][j] = find_hamiltonian_path_C(m, n, j, i, s, t);
					break;
				case 3:
					v[i][j] = find_hamiltonian_path_F(m, n, j, i, s, t);
					break;
				case 4:
					v[i][j] = find_hamiltonian_path_E(m, n, j, i, s, t);
					break;
			}
		}
	}
}

for(int i = 0; i < n; ++i) {
	for(int j = 0; j < m; ++j) {
		std::cout << v[i][j] << " ";
	}
	std::cout << '\n';
}

return 0;
}
