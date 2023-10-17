#include "hamiltonian_path_grid.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <time.h>

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

void worker_thread(int thread_index, long begin, int m, int n, long V, const std::pair<int, int>* s, const std::pair<int, int>* t, std::vector<std::pair<int, int>>* path) {

	auto find_path = find_hamiltonian_path;
	auto convert = number_to_point_rectangle;
	
	for(long i = begin; i < begin + V; ++i) {
		auto point = convert(m, n, i);
		(*path)[i] = find_path(m, n, point.second, point.first, *s, *t);
	}
	
}

long long evaluate_time(int number_of_threads, long m, long n, long V, const std::pair<int, int>& s, const std::pair<int, int>& t) {
	
	auto find_path = find_hamiltonian_path;
	auto convert = number_to_point_rectangle;

	std::vector<std::pair<int, int>> hamiltonian_path;
	hamiltonian_path.reserve(V);


	if(number_of_threads > 1) {
	
		std::vector<std::thread> threads;
	
		// each thread gets approximately the same number of vertices
		// the last thread gets the remaining ones
		long step = V / number_of_threads;
	
		auto start = std::chrono::high_resolution_clock::now();
		for(int i = 0; i < number_of_threads; ++i) {
			threads.push_back(std::thread(worker_thread, i, step * i, (i == number_of_threads - 1)? V + step - step * number_of_threads : step, m, n, &s, &t, &hamiltonian_path));
		}
	
		for(auto& t : threads) {
			t.join();
		}
	
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		return duration.count();

	} else {

		auto start = std::chrono::high_resolution_clock::now();
	
		for(long i = 0; i < V; ++i) {
			auto point = convert(m, n, i);
			hamiltonian_path[i] = find_path(m, n, point.second, point.first, s, t);
		}
	
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		return duration.count();
	}
	


}


int main() {

	srand(time(0));

	int number_of_threads;
	std::cout << "Enter the number of threads: ";
	std::cin >> number_of_threads;
	
	
	int T;
	std::cout << "Enter the number of tests: ";
	std::cin >> T;
	
	std::string filename;
	std::cout << "Choose file name: ";
	std::cin >> filename;
	
	std::ofstream data_csv;
	data_csv.open(filename, std::ios::out);
	
	if(!data_csv) {
		std::cout << "No such file!\n";
	} else {
		std::cout << "File created successfully!\n";
		data_csv << "m,n,V,sx,sy,tx,ty,time\n";
		for(int i = 0; i < T; ++i) {
			// generisemo m i n nasumicno iz intervala [1000, 10000]
			long m = 1000 + std::rand() % 9000;
			long n = 1000 + std::rand() % 9000;
			// s i t generisemo nasumicno
			std::pair<int, int> s = {std::rand() % n, std::rand() % m};
			std::pair<int, int> t = {std::rand() % n, std::rand() % m};
			
			// pakujemo tako da ima puta
			if(m % 2 and n % 2) {
				// ako s nije bela, menjamo u belu
				if((s.first + s.second) % 2) {
					if(s.first > 0) --s.first;
					else ++s.first;
				}
				
				// ako t nije bela, menjamo u belu
				if((t.first + t.second) % 2) {
					if(t.first > 0) --t.first;
					else ++t.first;
				}
				
			} else {
				// ako su istih boja, okrecemo jednu
				if((s.first + s.second) % 2 == (t.first + t.second) % 2) {
					if((s.first + s.second) % 2) {
						if(s.first > 0) --s.first;
						else ++s.first;
					}
				}			
			}
			
			long V = m*n;
			
			long long time = evaluate_time(number_of_threads, m, n, V, s, t);
			
			data_csv << m << ',' << n << ',' << V << ',' << s.second << ',' << s.first << ',' << t.second << ',' << t.first << ',' << time << '\n';
			
		}
		
		data_csv.close();
	
	}




	return 0;
}










