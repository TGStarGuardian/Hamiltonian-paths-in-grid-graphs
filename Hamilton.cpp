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

		if(s.second > t.second) std::swap(s, t);

		auto x = s;
		auto y = t;

		if(x.first > y.first) std::swap(x, y);

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
				 || (width == 3 && !(height % 2) && ((x.first + x.second) % 2) && !((y.first + y.second) % 2) && ((x.second == 1 && y.first > x.first) || (x.second != 1 && y.first > x.first + 1)))
				 || (height == 3 && !(width % 2) && ((s.first + s.second) % 2) && !((t.first + t.second) % 2) && ( (s.first == 1 && t.second > s.second) || (s.first != 1 && t.second > s.second + 1)))
				)
				;
	}

	std::list<std::pair<int, int>> find_hamiltonian_cycle(bool t, int width, int height) {

		if(!has_hamiltonian_cycle(width, height)) return {};

		std::list<std::pair<int, int>> ret;
		if(t) {
			if(!(height % 2)) {
				// left face is concave
				hamiltonian_cycle_top(ret, width, height);
			} else {
				// bottom face is concave
				hamiltonian_cycle_left(ret, width, height);
			}
		}
			else {
				if(!(height % 2)) {
					// right face is concave
					hamiltonian_cycle_bottom(ret, width, height);
				} else {
					// bottom face is concave
					hamiltonian_cycle_right(ret, width, height);
				}
			}

		return ret;
	}

	peeling peel(std::pair<int, int>& s, std::pair<int, int>& t, int width, int height) {
		peeling r;

		r.r1 = std::min(s.first, t.first) - 1;
		r.r2 = std::max(s.first, t.first);
		r.r3 = std::min(s.second, t.second) - 1;
		r.r4 = std::max(s.second, t.second);

		// if(!(r.r1 % 2)) r.r1--;
		r.r1 -= !(r.r1 % 2);
		// if(!(_height - r.r2) % 2) r.r2++;
		r.r2 += !((height - r.r2) % 2);
		// if(!(r.r3 % 2)) r.r3--;
		r.r3 -= !(r.r3 % 2);
		// if(!(_width - r.r4) % 2) r.r4++;
		r.r4 += !((width - r.r4) % 2);
		return r;
	}

	std::pair<int, int> find_junction_vertex_top(std::pair<int, int>& s, int m) {
		// it is either (1, 0) or (1, m-1) depending on the colour of s
		// in case (1, 0) have different colours, then we return (1, 0)
		return ((s.first + s.second + 1) % 2)? std::pair<int, int>(1, 0) : std::pair<int, int>(1, m-1);
	}

	std::pair<int, int> find_junction_vertex_bottom(std::pair<int, int>& t, int m, int n) {
		// it is either (n-2, 0) or (n-2, m-1) depending on the colour of s
		// in case (0, 0) and t have different colours, then we return (0, 0)
		return ((t.first + t.second + n) % 2)? std::pair<int, int>(n-2, 0) : std::pair<int, int>(n-2, m-1);
	}

	std::pair<int, int> find_junction_vertex_left(std::pair<int, int>& s, int n) {
		// it is either (0, 1) or (n-1, 1) depending on the colour of s
		// in case (0, 1) have different colours, then we return (1, 0)
		return ((s.first + s.second + 1) % 2)? std::pair<int, int>(0, 1) : std::pair<int, int>(n-1, 1);
	}

	std::pair<int, int> find_junction_vertex_right(std::pair<int, int>& t, int m, int n) {
		// it is either (0, m-2) or (n-1, m-2) depending on the colour of s
		// in case (0, 0) and t have different colours, then we return (0, 0)
		return ((t.first + t.second + m) % 2)? std::pair<int, int>(0, m-2) : std::pair<int, int>(n-1, m-2);
	}

	std::list<std::pair<int, int>> find_path_m5(std::pair<int, int>& s, std::pair<int, int>& t, peeling &r) {

		std::list<std::pair<int, int>> ret;
		// M5: [r1 + 1, r2] x [r3 + 1, r4]
		std::pair<int, int> s1 = {s.first - r.r1 - 1, s.second - r.r3 - 1};
		std::pair<int, int> t1 = {t.first - r.r1 - 1, t.second - r.r3 - 1};

		std::cout << s1 << '\n' << t1 << '\n';
		int n = r.r2 - r.r1;
		int m = r.r4 - r.r3;
		std::cout << n << '\n' << m << '\n';

		if(!has_hamiltonian_path(s1, t1, m, n)) return {};

		if(n < 4 && m < 4) {
			return {};

		} else if((!(m % 2) && !(n % 2) && (m > 4 || n > 4))
				|| ((m % 2) && (n % 2) && (m > 5 || n > 5))
				|| (!(m % 2) && (n % 2) && n > 3)
				|| (!(n % 2) && (m % 2) && m > 3)
				|| (n == 3 && m >= 4 && !(s.second <= t.second? (s.first + s.second) % 2 : (t.first + t.second) % 2))
				|| (m == 3 && n >= 4 && !(s.first <= t.first? (s.first + s.second) % 2 : (t.first + t.second) % 2))) {
			if(n >= 4) {
				// perform horizontal cut
				if(s1.first > t1.first) std::swap(s1, t1);
				std::pair<int, int> p = this->find_junction_vertex_top(s1, m);
				// in case p is on the left, then even --> m - 1, odd --> 0
				// in case p is on the right, then even --> 0, odd --> m - 1
				std::pair<int, int> q = {n - 2, !(n % 2)*(!(!(p.second)) * (m - 1)) + (n % 2)*(!(p.second) * (m - 1))};
				std::cout << p << ", " << q << '\n';

				// connect s and p
				int d = s1.second - p.second;
				if(d > 0) {
					// s is on the right, p is on the left side
					// go to the right
					for(int j = s1.second; j < m; j++)
						ret.push_back({s1.first, j});
					// go up/down and then turn left until s
					for(int j = m - 1; j >= s1.second; j--)
						ret.push_back({!s1.first, j});
					// go in a zig-zag fashion until you reach p
					int k = !s1.first;
					for(int j = s1.second - 1; j >= 0; j--) {
						ret.push_back({k, j});
						ret.push_back({!k, j});
						k = !k;
					}
				} else if(d < 0) {
					// s is on the left, p is on the right side
					// go to the left
					for(int j = s1.second; j >= 0; j--)
						ret.push_back({s1.first, j});
					// go up/down and then turn right until s
					for(int j = 0; j <= s1.second; j++)
						ret.push_back({!s1.first, j});
					// go in a zig-zag fashion until you reach p
					int k = !s1.first;
					for(int j = s1.second + 1; j < m; j++) {
						ret.push_back({k, j});
						ret.push_back({!k, j});
						k = !k;
					}
				} else {
					// they are on the same side
					if(!s1.second) {
						for(int j = 0; j < m; j++)
							ret.push_back({0, j});
						for(int j = m - 1; j >= 0; j--)
							ret.push_back({1, j});
					} else {
						for(int j = m - 1; j >= 0; j--)
							ret.push_back({0, j});
						for(int j = 0; j < m; j++)
							ret.push_back({1, j});
					}
				}
				// connect p and q
				// just a zig-zag pattern
				int k = p.second;
				for(int i = p.first + 1; i < q.first; i++) {
					for(int j = 0; j < m; j++)
						ret.push_back({i, std::abs(j - k)});
					k = std::abs(m - 1 - k);
				}


				// connect q and t

				d = t1.second - q.second;
				ret.push_back({0, 0});
				auto it = ret.end();
				std::advance(it, -1);
				if(d > 0) {
					// t is on the right, q is on the left side
					// go to the right
					for(int j = t1.second; j < m; j++)
						it = ret.insert(it, {t1.first, j});
					// go up/down and then turn left until t
					for(int j = m - 1; j >= t1.second; j--)
						it = ret.insert(it, {!(t1.first - n + 2) + n - 2, j});
					// go in a zig-zag fashion until you reach q
					int k = !(t1.first - n + 2);
					for(int j = t1.second - 1; j >= 0; j--) {
						it = ret.insert(it, {k + n - 2, j});
						it = ret.insert(it, {!k + n - 2, j});
						k = !k;
					}
				} else if(d < 0) {
					// s is on the left, p is on the right side
					// go to the left
					for(int j = t1.second; j >= 0; j--)
						it = ret.insert(it, {t1.first, j});
					// go up/down and then turn right until s
					for(int j = 0; j <= t1.second; j++)
						it = ret.insert(it, {!(t1.first - n + 2) + n - 2, j});
					// go in a zig-zag fashion until you reach q
					int k = !(t1.first - n + 2);
					for(int j = t1.second + 1; j < m; j++) {
						it = ret.insert(it, {k + n - 2, j});
						it = ret.insert(it, {!k + n - 2, j});
						k = !k;
					}
				} else {
					// they are on the same side
					if(!t1.second) {
						for(int j = 0; j < m; j++)
							it = ret.insert(it, {n - 2, j});
						for(int j = m - 1; j >= 0; j--)
							it = ret.insert(it, {n - 1, j});
					} else {
						for(int j = m - 1; j >= 0; j--)
							it = ret.insert(it, {n - 2, j});
						for(int j = 0; j < m; j++)
							it = ret.insert(it, {n - 1, j});
					}
				}
				ret.pop_back();
				return ret;

			} else if(m >= 4) {
				// perform vertical cut
				if(s1.second > t1.second) std::swap(s1, t1);
				std::pair<int, int> p = this->find_junction_vertex_left(s1, n);
				std::pair<int, int> q = this->find_junction_vertex_right(t1, m, n);
				std::cout << p << ", " << q << '\n';

				// connect s and p
				int d = s1.first - p.first;

				if(d > 0) {
					// s is below p
					// go down
					for(int i = s1.first; i < n; i++) {
						ret.push_back({i, s1.second});
					}
					// then turn left/right
					// then go upto the s
					for(int i = m - 1; i >= s1.first; i--) {
						ret.push_back({i, !s1.second});
					}
					// then go in a zig-zag fashion until you reach p
					int k = !s1.second;
					for(int i = s1.first - 1; i >= 0; i--) {
						ret.push_back({i, k});
						ret.push_back({i, !k});
						k = !k;
					}
				} else if (d < 0) {
					// s is above p
					// go up
					for(int i = s1.first; i >= 0; i--) {
						ret.push_back({i, s1.second});
					}
					// then turn left/rigth
					// go downto s
					for(int i = 0; i <= s1.first; i++) {
						ret.push_back({i, !s1.second});
					}
					// then go in a zig-zag fashion until you reach p
					int k = !s1.second;
					for(int i = s1.first + 1; i < n; i++) {
						ret.push_back({i, k});
						ret.push_back({i, !k});
						k = !k;
					}

				} else {
					// go to the top/bottom
					// then right
					// then to the bottom/top
					if(!s.first) {
						// go down
						for(int i = 0; i < n; i++) {
							// note that the second coordinate is 0
							// that's because otherwise s == p
							// and that's impossible because s and p
							// have different colours
							ret.push_back({i, 0});
						}
						// go up
						for(int i = n - 1; i >= 0; i--) {
							ret.push_back({i, 1});
						}
					} else {
						// go up
						for(int i = n - 1; i >= 0; i--) {
							ret.push_back({i, 0});
						}
						// go down
						for(int i = 0; i < n; i++) {
							ret.push_back({i, 1});
						}
					}
				}

				// connect p and q
				// just a zig-zag pattern
				int k = p.first;
				for(int j = p.second + 1; j < q.second; j++) {
					for(int i = 0; i < n; i++)
						ret.push_back({std::abs(i - k), j});
					k = std::abs(n - 1 - k);
				}
				// connect q and t

				d = t1.first - q.first;
				ret.push_back({0, 0});
				auto it = ret.end();
				std::advance(it, -1);
				if(d > 0) {
					// t is on the right, q is on the left side
					// go to the right
					for(int i = t1.first; i < n; i++)
						it = ret.insert(it, {i, t1.second});
					// go up/down and then turn left until t
					for(int i = n - 1; i >= t1.first; i--)
						it = ret.insert(it, {i, !(t1.second - m + 2) + m - 2});
					// go in a zig-zag fashion until you reach q
					int k = !(t1.second - m + 2);
					for(int i = t1.first - 1; i >= 0; i--) {
						it = ret.insert(it, {i, k + m - 2});
						it = ret.insert(it, {i, !k + m - 2});
						k = !k;
					}
				} else if(d < 0) {
					// s is on the left, p is on the right side
					// go to the left
					for(int i = t1.first; i >= 0; i--)
						it = ret.insert(it, {i, t1.second});
					// go up/down and then turn right until s
					for(int i = 0; i <= t1.first; i++)
						it = ret.insert(it, {i, !(t.second - m + 2) + m - 2});
					// go in a zig-zag fashion until you reach q
					int k = !(t1.second - m + 2);
					for(int i = t1.first + 1; i < n; i++) {
						it = ret.insert(it, {i, k + n - 2});
						it = ret.insert(it, {i, !k + n - 2});
						k = !k;
					}
				} else {
					// they are on the same side
					if(!t1.first) {
						for(int i = 0; i < n; i++)
							it = ret.insert(it, {i, m - 2});
						for(int i = n - 1; i >= 0; i--)
							it = ret.insert(it, {m - 1, i});
					} else {
						for(int i = n - 1; i >= 0; i--)
							it = ret.insert(it, {i, m - 2});
						for(int i = 0; i < n; i++)
							it = ret.insert(it, {i, m - 1});
					}
				}
				ret.pop_back();
				return ret;

			} else {
				return {};
			}
		} else {
			return {};
		}

		return {};
	}

	std::list<std::pair<int, int>> find_hamiltonian_path(std::pair<int, int>& s, std::pair<int, int>& t, int width, int height) {

		if(!has_hamiltonian_path(s, t, width, height)) return {};

		// peel the matrix
		peeling r = peel(s, t, width, height);

		std::list<std::pair<int, int>> ret = find_path_m5(s, t, r);

		if(ret.empty()) {

			if(r.r2 - r.r1 == 1) {
				if(height == 5 && s.first == 3 && t.first == 3) {

				} else {
					if(r.r1 >= 2)
						r.r1--;
					else
						r.r2++;

					ret = find_path_m5(s, t, r);

					// etc etc etc
				}
			} else if(r.r4 - r.r3 == 1) {
				if(width == 5 && s.second == 3 && t.second == 3) {

				} else {
					if(r.r3 >= 2)
						r.r3--;
					else
						r.r4++;
					ret = find_path_m5(s, t, r);
				}

			} else if(r.r2 - r.r1 == 2) {


			} else if(r.r4 - r.r3 == 2) {


			} else {


			}

		} else {
			auto m1 = find_hamiltonian_cycle(true, r.r3 + 1, height);
			auto m2 = find_hamiltonian_cycle(false, width - 1 - r.r4, height);
			auto m3 = find_hamiltonian_cycle(true, r.r4 - r.r3, r.r1 + 1);
			auto m4 = find_hamiltonian_cycle(false, r.r4 - r.r3, height - 1 - r.r2);

			// connect cycles


			// connect path and cycle

		}

		return ret;
	}
private:
	std::vector<std::vector<T>> graph;
	bool size_parity; // true when size is odd
	int _height, _width;

	void hamiltonian_cycle_left(std::list<std::pair<int, int>>& ret, int width, int height) {
		for(int i = 0; i <= height - 1; i++) {
			ret.push_back({i, 0});
		}
	// go in a serpentine fashion to the right
		for(int j = 1; j <= width - 1; j++) {
			if (j % 2) {
				// for odd j, we go up
				for(int i = height - 1; i >= 1; i--) {
					ret.push_back({i, j});
				}
			} else {
				// for even j, we go down
				for(int i = 1; i <= height - 1; i++) {
					ret.push_back({i, j});
				}
			}
		}

		// and finally we add a cap to the top
		for(int j = width - 1; j >= 1; j--) {
			ret.push_back({0, j});
		}
	}

	void hamiltonian_cycle_top(std::list<std::pair<int, int>>& ret, int width, int height) {
		for(int j = 0; j <= width - 1; j++) {
			ret.push_back({0, j});
		}
	// go in a serpentine fashion to the right
		for(int i = 1; i <= height - 1; i++) {
			if (i % 2) {
				// for odd j, we go up
				for(int j = width - 1; j >= 1; j--) {
					ret.push_back({i, j});
				}
			} else {
				// for even j, we go down
				for(int j = 1; j <= width - 1; j++) {
					ret.push_back({i, j});
				}
			}
		}

		// and finally we add a cap to the top
		for(int i = height - 1; i >= 1; i--) {
			ret.push_back({i, 0});
		}
	}

	void hamiltonian_cycle_bottom(std::list<std::pair<int, int>>& ret, int width, int height) {
		// t == false --> only the top face is concave
		// go all the way to the right
		for(int j = 0; j <= width - 1; j++) {
			ret.push_back({height - 1, j});
		}
		// go in a serpentine fashion to the top
		for(int i = height - 2; i >= 0; i--) {
			if (!((i + height) % 2)) {
				// for even i + _height, we go to the left
				for(int j = width - 1; j >= 1; j--) {
					ret.push_back({i, j});
				}
			} else {
				// for odd i + _height, we go to the right
				for(int j = 1; j <= width - 1; j++) {
					ret.push_back({i, j});
				}
			}
		}

		// and finally we add a cap to the left
		for(int i = 0; i <= _height - 2; i++) {
			ret.push_back({i, 0});
		}
	}

	void hamiltonian_cycle_right(std::list<std::pair<int, int>>& ret, int width, int height) {
		// t == false --> only the top face is concave
		// go all the way to the right
		for(int i = 0; i <= height - 1; i++) {
			ret.push_back({i, width - 1});
		}
		// go in a serpentine fashion to the top
		for(int j = width - 2; j >= 0; j--) {
			if (!((j + width) % 2)) {
				// for even i + _height, we go to the left
				for(int i = height - 1; i >= 1; i--) {
					ret.push_back({i, j});
				}
			} else {
				// for odd i + _height, we go to the right
				for(int i = 1; i <= height - 1; i++) {
					ret.push_back({i, j});
				}
			}
		}

		// and finally we add a cap to the left
		for(int j = 0; j <= width - 2; j++) {
			ret.push_back({0, j});
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

	std::list<std::pair<int, int>> cycle = G.find_hamiltonian_cycle(true, G.get_width(), G.get_height());
	for(const auto& l : cycle) {
		std::cout << '(' << l.first << ", " << l.second << ")\n";
	}

	Mesh<int>::peeling p = G.peel(s, t, G.get_width(), G.get_height());
	std::cout << p;

	auto path = G.find_hamiltonian_path(s, t, G.get_width(), G.get_height());

	std::cout << "The path is:\n";
	for(const auto& l : path) {
			std::cout << '(' << l.first << ", " << l.second << ")\n";
	}

	path = G.find_path_m5(s, t, p);
	std::cout << "The path in M5 is:\n";
		for(const auto& l : path) {
				std::cout << '(' << l.first << ", " << l.second << ")\n";
		}



	return 0;
}
