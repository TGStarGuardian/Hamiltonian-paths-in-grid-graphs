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

	bool color_compatible(std::pair<int, int>& v1, std::pair<int, int>& v2) {
		return (!size_parity && ((v1.first + v1.second + v2.first + v2.second) % 2))
				|| (size_parity && !((v1.first + v1.second) % 2) && !((v2.first + v2.second) % 2));
	}

	bool are_antipodes(std::pair<int, int>& v1, std::pair<int, int>& v2) {
		return std::min(v1.first, v2.first) <= 2
				&& std::min(v1.second, v2.second) <= 2
				&& std::max(v1.first, v2.first) >= _height - 1
				&& std::max(v1.second, v2.second) >= _width - 1;
	}

	bool has_hamiltonian_cycle(void) {
		return !size_parity && (_width ^ 1) && (_height ^ 1);
	}

	bool has_hamiltonian_path(std::pair<int, int>& s, std::pair<int, int>& t) {
		if(out_of_bounds(s.first, 0, _height)
				|| out_of_bounds(s.second, 0, _width)
				|| out_of_bounds(t.first, 0, _height)
				|| out_of_bounds(t.second, 0, _width)) {
			throw "Error: either s or t are outside the matrix";
		}
		return color_compatible(s, t) &&
				!( (_height == 1 && (!is_corner_vertex(s.second, _width) || !is_corner_vertex(t.second, _width)))
				 || (_width == 1 && (!is_corner_vertex(s.first, _height) || !is_corner_vertex(t.first, _height)))
				 || ((_width == 2 || _height == 2) && connected_by_non_boundary_edge(s, t, _width, _height))
				 || (_height == 3 && !(_width % 2) && ((s.first + s.second) % 2) && !((t.first + t.second) % 2) && ( (s.first == 1 && t.second > s.second) || (s.first != 1 && s.second <= t.second + 1)))
				)
				;
	}

	std::list<std::pair<std::pair<int, int>, T>> find_hamiltonian_cycle(bool t) {

		if(!has_hamiltonian_cycle()) return {};

		std::list<std::pair<std::pair<int, int>, T>> ret;
		// t == true --> only the bottom face is concave
		if(t) {
			// go all the way down
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
				std::cout << "Row: " << j << '\n';
			}

			// and finally we add a cap to the top
			for(int j = _width - 1; j >= 1; j--) {
				ret.push_back({{0, j}, graph[0][j]});
			}
		}
			else {
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

		return ret;
	}


private:
	std::vector<std::vector<T>> graph;
	bool size_parity; // true when size is odd
	int _height, _width;
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
	std::cout << G.color_compatible(s, t) << '\n';
	std::cout << G.are_antipodes(s, t) << '\n';
	std::cout << G.has_hamiltonian_path(s, t) << '\n';
	std::cout << G.has_hamiltonian_cycle() << '\n';

	std::list<std::pair<std::pair<int, int>, int>> cycle = G.find_hamiltonian_cycle(false);
	for(const auto& l : cycle) {
		std::cout << '(' << l.first.first << ", " << l.first.second << ") " << l.second << '\n';
	}

	return 0;
}
