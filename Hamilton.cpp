#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <list>

template<typename T>
inline void print(T& x, std::string&& name) {
	std::cout << name << ": " << x << '\n';
}

template<typename T>
std::ostream& operator<<(std::ostream& os, std::pair<T, T>& p) {
	os << '(' << p.first << ", " << p.second << ')';
	return os;
}

inline bool out_of_bounds(int x, int l, int r) {
	return (x < l && x > r);
}

inline bool is_corner_vertex(int y, int width) {
	return !y || !(y - width);
}

inline bool connected_by_non_boundary_edge(std::pair<int, int>& s, std::pair<int, int>& t, int width, int height) {
	return (s.first == t.first && std::abs(s.second - t.second) == 1 && !is_corner_vertex(s.first, height))
			|| (s.second == t.second && std::abs(s.first - t.first) == 1 && !is_corner_vertex(s.second, width));
}

template<typename T>
class Mesh {
public:

	typedef struct peeling {
		int r1, r2, r3, r4;

		friend std::ostream& operator<<(std::ostream& os, Mesh::peeling p) {
			os
			<< "r1: " << p.r1
			<< ", r2: " << p.r2
			<< ", r3: " << p.r3
			<< ", r4: " << p.r4
			<< '\n';
			return os;
		}

	} peeling;

	Mesh(const std::vector<std::vector<T>>& G) {
		_height = G.size();
		_width = G[0].size();
		for(const std::vector<T>& row : G) {
			(_width == row.size())? 0 : throw "[Mesh] The input is not a matrix";
		}
		graph = std::vector<std::vector<T>>(G);
		size_parity = _width % 2 && _height % 2;
	}

	Mesh(int width, int height) {
		graph.resize(height);
		for(std::vector<T>& row : graph) {
			row.resize(width);
		}
		_height = height;
		_width = width;
		size_parity = _width % 2 && _height % 2;
	}

	int get_height() {
		return _height;
	}

	int get_width() {
		return _width;
	}

	friend std::ostream& operator<<(std::ostream& os, const Mesh& M) {
		os 		<< "[Mesh]\n"
				<< "m: " << M._width << '\n'
				<< "n: " << M._height << '\n';

		for(const std::vector<T>& row : M.graph) {
			for(const T& element : row) {
				os << element <<  ' ';
			}
			os << '\n';
		}
		return os;
	}

	bool color_compatible(std::pair<int, int>& v1, std::pair<int, int>& v2, int width, int height) {
		int parity= (width % 2) && (height % 2);
		return (!parity && ((v1.first + v1.second + v2.first + v2.second) % 2))
				|| (parity && !((v1.first + v1.second) % 2) && !((v2.first + v2.second) % 2));
	}

	bool are_antipodes(std::pair<int, int>& v1, std::pair<int, int>& v2, int width, int height) {
		return std::min(v1.first, v2.first) <= 1
				&& std::min(v1.second, v2.second) <= 1
				&& std::max(v1.first, v2.first) >= height - 2
				&& std::max(v1.second, v2.second) >= width - 2;
	}

	bool has_hamiltonian_cycle(int width, int height) {
		return !((width % 2) && (height % 2)) && (width ^ 1) && (height ^ 1);
	}

	bool has_hamiltonian_path(std::pair<int, int>& s, std::pair<int, int>& t, int width, int height) {
		if(out_of_bounds(s.first, 0, height)
				|| out_of_bounds(s.second, 0, width)
				|| out_of_bounds(t.first, 0, height)
				|| out_of_bounds(t.second, 0, width)) {
			throw "Error: either s or t are outside the matrix";
		}
		return color_compatible(s, t, width, height) &&
				!( (height == 1 && (!is_corner_vertex(s.second, width) || !is_corner_vertex(t.second, width)))
				 || (width == 1 && (!is_corner_vertex(s.first, height) || !is_corner_vertex(t.first, height)))
				 || ((width == 2 || height == 2) && connected_by_non_boundary_edge(s, t, width, height))
				 || (height == 3 && !(width % 2) && ((s.first + s.second) % 2) && !((t.first + t.second) % 2) && ( (s.first == 1 && t.second > s.second) || (s.first != 1 && s.second <= t.second + 1)))
				)
				;
	}

	std::list<std::pair<std::pair<int, int>, T>> find_hamiltonian_cycle(bool t) {

		if(!has_hamiltonian_cycle(_width, _height)) return {};

		std::list<std::pair<std::pair<int, int>, T>> ret;
		if(t) {
			if(!(_height % 2)) {
				// left face is concave
				hamiltonian_cycle_top(ret);
			} else {
				// bottom face is concave
				hamiltonian_cycle_left(ret);
			}
		}
			else {
				if(!(_height % 2)) {
					// right face is concave
					hamiltonian_cycle_bottom(ret);
				} else {
					// bottom face is concave
					hamiltonian_cycle_right(ret);
				}
			}

		return ret;
	}

	peeling peel(std::pair<int, int>& s, std::pair<int, int>& t) {
		peeling r;

		r.r1 = std::min(s.first, t.first) - 1;
		r.r2 = std::max(s.first, t.first);
		r.r3 = std::min(s.second, t.second) - 1;
		r.r4 = std::max(s.second, t.second);

		// if(!(r.r1 % 2)) r.r1--;
		r.r1 -= !(r.r1 % 2);
		// if(!(_height - r.r2) % 2) r.r2++;
		r.r2 += !((_height - r.r2) % 2);
		// if(!(r.r3 % 2)) r.r3--;
		r.r3 -= !(r.r3 % 2);
		// if(!(_width - r.r4) % 2) r.r4++;
		r.r4 += !((_width - r.r4) % 2);
		return r;


	}

private:
	std::vector<std::vector<T>> graph;
	bool size_parity; // true when size is odd
	int _height, _width;

	void hamiltonian_cycle_left(std::list<std::pair<std::pair<int, int>, T>>& ret) {
		for(int i = 0; i <= _height - 1; i++) {
			ret.push_back({{i, 0}, graph[i][0]});
		}
	// go in a serpentine fashion to the right
		for(int j = 1; j <= _width - 1; j++) {
			if (j % 2) {
				// for odd j, we go up
				for(int i = _height - 1; i >= 1; i--) {
					ret.push_back({{i, j}, graph[i][j]});
				}
			} else {
				// for even j, we go down
				for(int i = 1; i <= _height - 1; i++) {
					ret.push_back({{i, j}, graph[i][j]});
				}
			}
		}

		// and finally we add a cap to the top
		for(int j = _width - 1; j >= 1; j--) {
			ret.push_back({{0, j}, graph[0][j]});
		}
	}

	void hamiltonian_cycle_top(std::list<std::pair<std::pair<int, int>, T>>& ret) {
		for(int j = 0; j <= _width - 1; j++) {
			ret.push_back({{0, j}, graph[0][j]});
		}
	// go in a serpentine fashion to the right
		for(int i = 1; i <= _height - 1; i++) {
			if (i % 2) {
				// for odd j, we go up
				for(int j = _width - 1; j >= 1; j--) {
					ret.push_back({{i, j}, graph[i][j]});
				}
			} else {
				// for even j, we go down
				for(int j = 1; j <= _width - 1; j++) {
					ret.push_back({{i, j}, graph[i][j]});
				}
			}
		}

		// and finally we add a cap to the top
		for(int i = _height - 1; i >= 1; i--) {
			ret.push_back({{i, 0}, graph[i][0]});
		}
	}

	void hamiltonian_cycle_bottom(std::list<std::pair<std::pair<int, int>, T>>& ret) {
		// t == false --> only the top face is concave
		// go all the way to the right
		for(int j = 0; j <= _width - 1; j++) {
			ret.push_back({{_height - 1, j}, graph[_height - 1][j]});
		}
		// go in a serpentine fashion to the top
		for(int i = _height - 2; i >= 0; i--) {
			if (!((i + _height) % 2)) {
				// for even i + _height, we go to the left
				for(int j = _width - 1; j >= 1; j--) {
					ret.push_back({{i, j}, graph[i][j]});
				}
			} else {
				// for odd i + _height, we go to the right
				for(int j = 1; j <= _width - 1; j++) {
					ret.push_back({{i, j}, graph[i][j]});
				}
			}
		}

		// and finally we add a cap to the left
		for(int i = 0; i <= _height - 2; i++) {
			ret.push_back({{i, 0}, graph[i][0]});
		}
	}

	void hamiltonian_cycle_right(std::list<std::pair<std::pair<int, int>, T>>& ret) {
		// t == false --> only the top face is concave
		// go all the way to the right
		for(int i = 0; i <= _height - 1; i++) {
			ret.push_back({{i, _width - 1}, graph[i][_width - 1]});
		}
		// go in a serpentine fashion to the top
		for(int j = _width - 2; j >= 0; j--) {
			if (!((j + _width) % 2)) {
				// for even i + _height, we go to the left
				for(int i = _height - 1; i >= 1; i--) {
					ret.push_back({{i, j}, graph[i][j]});
				}
			} else {
				// for odd i + _height, we go to the right
				for(int i = 1; i <= _height - 1; i++) {
					ret.push_back({{i, j}, graph[i][j]});
				}
			}
		}

		// and finally we add a cap to the left
		for(int j = 0; j <= _width - 2; j++) {
			ret.push_back({{0, j}, graph[0][j]});
		}
	}

};




int main() {

	srand((unsigned) time(NULL));

	int n, m;
	std::pair<int, int> s, t;

	std::cout << "Unesite dimenzije matrice\n";

	std::cin >> n >> m;

	print<int>(n, "n");
	print<int>(m, "m");

	std::cout << "Unesite koordinate početne i krajnje tačke\n";


	int t1, t2;
	std::cin >> t1 >> t2;
	s = std::pair<int, int>(t1, t2);

	std::cin >> t1 >> t2;
	t = std::pair<int, int>(t1, t2);

	if(s.second > t.second) {
		std::swap(s, t);
	}

	print<std::pair<int, int>>(s, "s");
	print<std::pair<int, int>>(t, "t");

	std::vector<std::vector<int>> matrix;

	matrix.resize(n);

	for(auto& row : matrix) {
		row.resize(m);
		for(int& element : row) {
			element = rand();
		}
	}

	std::cout << matrix[0].size() << '\n';

	Mesh<int> G(matrix);
	std::cout << G;;
	std::cout << G.color_compatible(s, t, G.get_width(), G.get_height()) << '\n';
	std::cout << G.are_antipodes(s, t, G.get_width(), G.get_height()) << '\n';
	std::cout << G.has_hamiltonian_path(s, t, G.get_width(), G.get_height()) << '\n';
	std::cout << G.has_hamiltonian_cycle(G.get_width(), G.get_height()) << '\n';

	std::list<std::pair<std::pair<int, int>, int>> cycle = G.find_hamiltonian_cycle(true);
	for(const auto& l : cycle) {
		std::cout << '(' << l.first.first << ", " << l.first.second << ") " << l.second << '\n';
	}

	Mesh<int>::peeling p = G.peel(s, t);
	std::cout << p;

	return 0;
}
