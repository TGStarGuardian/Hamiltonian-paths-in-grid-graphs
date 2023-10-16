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

std::pair<int, int> number_to_point_rectangle(long m, long n, long x) {
	return {x / m, x % m};
}

std::pair<int, int> number_to_point_L(long m, long n, long x) {
	// the upper vertical rectangle has m(4n - 4) vertices
	// if x < m(4n - 4), it is in the upper vertical rectangle
	// otherwise, it is in the horizontal rectangle
	long V1 = m*(4*n - 4);
	if(x < V1) {
		return number_to_point_rectangle(m, 4*n - 4, x);
	} else {
		x -= V1;
		auto ret = number_to_point_rectangle(3*m - 2, n, x);
		return {ret.first + 4*n - 4, ret.second};
	
	}

}

std::pair<int, int> number_to_point_C(long m, long n, long x) {
	// there are three rectangles: the upper horizontal
	// the middle vertical and the bottom horizontal
	long H = (3*m - 2)*n;
	long V = m*(3*n - 4);
	
	if(x < H) {
		return number_to_point_rectangle(3*m - 2, n, x);
	} else if(x < H + V) {
		x -= H;
		auto ret = number_to_point_rectangle(m, 3*n - 4, x);
		return {ret.first + n, ret.second};
	} else {
		x -= H + V;
		auto ret = number_to_point_rectangle(3*m - 2, n, x);
		return {ret.first + 4*n - 4, ret.second};
	}
}

std::pair<int, int> number_to_point_F(long m, long n, long x) {
	// there are 5 rectangles
	long H1 = (3*m - 2)*n;
	long V = m*(n - 2);
	long H2 = (3*m - 4)*n;
	
	if(x < H1) {
		return number_to_point_rectangle(3*m - 2, n, x);
	} else if(x < H1 + V) {
		x -= H1;
		auto ret = number_to_point_rectangle(m, n - 2, x);
		return {ret.first + n, ret.second};
	} else if(x < H1 + V + H2) {
		x -= H1 + V;
		auto ret = number_to_point_rectangle(3*m - 4, n, x);
		return {ret.first + 2*n - 2, ret.second};
	} else if(x < H1 + V + H2 + V) {
		x -= H1 + V + H2;
		auto ret = number_to_point_rectangle(m, n - 2, x);
		return {ret.first + 3*n - 2, ret.second};
	} else {
		x -= H1 + V + H2 + V;
		auto ret = number_to_point_rectangle(m, n, x);
		return {ret.first + 4*n - 4, ret.second};
	}
}

std::pair<int, int> number_to_point_E(long m, long n, long x) {
	// there are 5 rectangles
	long H1 = (3*m - 2)*n;
	long V = m*(n - 2);
	long H2 = (3*m - 4)*n;
	
	if(x < H1) {
		return number_to_point_rectangle(3*m - 2, n, x);
	} else if(x < H1 + V) {
		x -= H1;
		auto ret = number_to_point_rectangle(m, n - 2, x);
		return {ret.first + n, ret.second};
	} else if(x < H1 + V + H2) {
		x -= H1 + V;
		auto ret = number_to_point_rectangle(3*m - 4, n, x);
		return {ret.first + 2*n - 2, ret.second};
	} else if(x < H1 + V + H2 + V) {
		x -= H1 + V + H2;
		auto ret = number_to_point_rectangle(m, n - 2, x);
		return {ret.first + 3*n - 2, ret.second};
	} else {
		x -= H1 + V + H2 + V;
		auto ret = number_to_point_rectangle(3*m - 2, n, x);
		return {ret.first + 4*n - 4, ret.second};
	}
}

void worker_thread(int thread_index, long begin, long V, int m, int n, const std::pair<int, int>* s, const std::pair<int, int>* t, std::vector<std::pair<int, int>>* path, int type) {

	auto find_path = find_hamiltonian_path;
	auto convert = number_to_point_rectangle;

	switch(type) {

	case 0:

	// rectangular graph
		break;

	case 1:

	// L graph
		find_path = find_hamiltonian_path_L;
		convert = number_to_point_L;
		break;

	case 2:

	// C graph
	
		find_path = find_hamiltonian_path_C;
		convert = number_to_point_C;
		break;

	case 3:

	// F graph
		find_path = find_hamiltonian_path_F;
		convert = number_to_point_F;
		break;

	case 4:

	// E graph
		find_path = find_hamiltonian_path_E;
		convert = number_to_point_E;
		break;

	default:
		std::cout << "Invalid type!\n";
		return;
	}
	
	for(long i = begin; i < begin + V; ++i) {
		auto point = convert(m, n, i);
		(*path)[i] = find_path(m, n, point.second, point.first, *s, *t);
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

// V is the number of vertices
long V;
auto find_path = find_hamiltonian_path;
auto convert = number_to_point_rectangle;

switch(type) {

case 0:

// rectangular graph

	V = m*n;
	break;

case 1:

// L graph

	V = (3*m - 2)*(5*n - 4) - (2*m - 2)*(4*n - 4);
	find_path = find_hamiltonian_path_L;
	convert = number_to_point_L;
	break;

case 2:

// C graph

	V = (3*m - 2)*(5*n - 4) - (2*m - 2)*(3*n - 4);
	find_path = find_hamiltonian_path_C;
	convert = number_to_point_C;
	break;

case 3:

// F graph

	V = (3*m - 2)*(5*n - 4) - (2*m - 2)*(4*n - 4) + (2*m - 4)*n;
	find_path = find_hamiltonian_path_F;
	convert = number_to_point_F;
	break;

case 4:

// E graph

	V = (3*m - 2)*(5*n - 4) - (2*m - 2)*(3*n - 4) + (2*m - 4)*n;
	find_path = find_hamiltonian_path_E;
	convert = number_to_point_E;
	break;

default:
	std::cout << "Invalid type!\n";
	return -1;
}

std::vector<std::pair<int, int>> hamiltonian_path;
hamiltonian_path.reserve(V);


if(parallel) {

	std::cout << "Choose the number of threads\n";
	std::cin >> num_threads;
	std::vector<std::thread> threads;
	
	// each thread gets approximately the same number of vertices
	// the last thread gets the remaining ones
	long step = V / num_threads;
	
	auto start = std::chrono::high_resolution_clock::now();
	
	for(int i = 0; i < num_threads; ++i) {
		threads.push_back(std::thread(worker_thread, i, step * i, (i == num_threads - 1)? V + step - step * num_threads : step, m, n, &s, &t, &hamiltonian_path, type));
	}
	
	for(auto& t : threads) {
		t.join();
	}
	
	
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "Time spent: " << duration.count() << " microseconds\n";

} else {

	auto start = std::chrono::high_resolution_clock::now();
	
	for(long i = 0; i < V; ++i) {
		auto point = convert(m, n, i);
		hamiltonian_path[i] = find_path(m, n, point.second, point.first, s, t);
	}
	
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "Time spent: " << duration.count() << " microseconds\n";
}

/*
for(long i = 0; i < V; ++i) {
	auto point = convert(m, n, i);
	std::cout << point << " --> " << hamiltonian_path[i] << '\n';
}
*/

return 0;
}
