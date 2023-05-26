#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <list>
#include <algorithm>
#include <iterator>

template<typename T>
inline void print(T& x, std::string&& name) {
	std::cout << name << ": " << x << '\n';
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::pair<T, T>& p) {
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

	enum mesh {M1, M2, M3, M4};

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

	std::list<std::pair<int, int>> find_hamiltonian_cycle(enum mesh t, int width, int height) {

		if(!has_hamiltonian_cycle(width, height)) return {};

		std::list<std::pair<int, int>> ret;

		switch(t) {
		case M1:
			if(!height % 2)
				// left face is concave, the orientation is -->
				hamiltonian_cycle_top(ret, width, height);
			else
				// top face is concave, the orientation is <--
				hamiltonian_cycle_right(ret, width, height);
			break;
		case M2:
			if(!height % 2)
				// right face is concave, the orientation is <--
				hamiltonian_cycle_bottom(ret, width, height);
			else
				// bottom face is concave, the orientation is <--
				hamiltonian_cycle_left(ret, width, height);
			break;
		case M3:
			if(!height % 2)
				// right face is concave, the orientation is <--
				hamiltonian_cycle_bottom(ret, width, height);
			else
				// top face is concave, the orientation is <--
				hamiltonian_cycle_right(ret, width, height);
			break;
		case M4:
			if(!height % 2)
				// right face is concave, the orientation is <--
				hamiltonian_cycle_bottom(ret, width, height);
			else
				// bottom face is concave, the orientation is <--
				hamiltonian_cycle_left(ret, width, height);
			break;

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

	std::list<std::pair<int, int>> connect_s_p(std::pair<int, int>& s1, std::pair<int, int>&p, int m, int n) {
		std::list<std::pair<int, int>> ret;

		if(n >= 4) {
			// horizontal cut
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

		} else if(m >= 4) {
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
				// then turn left/right
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
				// go to the left/right
				// then up
				// then to the right/left
				if(!s1.first) {
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
		} else {
			return {};
		}

		return ret;

	}

	std::list<std::pair<int, int>> connect_q_t(std::pair<int, int>&q, std::pair<int, int>& t1, int m, int n) {
		std::list<std::pair<int, int>> ret;

		if(n >= 4) {
			int d = t1.second - q.second;
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

		} else if(m >= 4) {
			int d = t1.first - q.first;
			ret.push_back({0, 0});
			auto it = ret.end();
			std::advance(it, -1);
			if(d > 0) {
				// t is down, q is up
				// go down
				for(int i = t1.first; i < n; i++)
					it = ret.insert(it, {i, t1.second});

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
				// t is up, q is down
				// go up
				for(int i = t1.first; i >= 0; i--)
					it = ret.insert(it, {i, t1.second});

				for(int i = 0; i <= t1.first; i++)
					it = ret.insert(it, {i, !(t1.second - m + 2) + m - 2});
				// go in a zig-zag fashion until you reach q
				int k = !(t1.second - m + 2);
				for(int i = t1.first + 1; i < n; i++) {
					it = ret.insert(it, {i, k + m - 2});
					it = ret.insert(it, {i, !k + m - 2});
					k = !k;
				}
			} else {
				// they are on the same side
				if(!q.first) {
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

		} else {
			return {};
		}
		return ret;
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
			switch(n) {
			case 1:
				switch(m) {
				case 1: return {s1};
				case 2: return {s1, {0, !s1.second}};
				case 3: return {s1, {0, 1}, {0, !s1.second * 2}};
				}
				break;
			case 2:
				switch(m) {
				case 1: return {s1};
				case 2:
				case 3:
					return {};
				}
				break;
			case 3:
				switch(m) {
				case 1:
				case 2:
				case 3:
					return {};
				}
				break;
			}
			return ret;

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
				ret = connect_s_p(s1, p, m, n);
				// connect p and q
				// just a zig-zag pattern
				int k = p.second;
				for(int i = p.first + 1; i < q.first; i++) {
					for(int j = 0; j < m; j++)
						ret.push_back({i, std::abs(j - k)});
					k = std::abs(m - 1 - k);
				}


				// connect q and t
				auto res = connect_q_t(q, t1, m, n);
				ret.insert(ret.end(), res.begin(), res.end());

				return ret;

			} else if(m >= 4) {
				// perform vertical cut
				if(s1.second > t1.second) std::swap(s1, t1);
				std::pair<int, int> p = this->find_junction_vertex_left(s1, n);
				std::pair<int, int> q = {!(m % 2)*(!(!(p.first)) * (n - 1)) + (m % 2)*(!(p.first) * (n - 1)), m - 2};
				std::cout << p << ", " << q << '\n';

				// connect s and p
				ret = connect_s_p(s1, p, m, n);
				// connect p and q
				// just a zig-zag pattern
				int k = p.first;
				for(int j = p.second + 1; j < q.second; j++) {
					for(int i = 0; i < n; i++)
						ret.push_back({std::abs(i - k), j});
					k = std::abs(n - 1 - k);
				}
				// connect q and t
				auto res = connect_q_t(q, t1, m, n);
				ret.insert(ret.end(), res.begin(), res.end());
				return ret;

			} else {
				return {};
			}
		} else {
			// 3x4 or 4x3 case
			return {};
		}

		return {};
	}

	std::list<std::pair<int, int>> find_hamiltonian_path(std::pair<int, int>& s, std::pair<int, int>& t, int width, int height) {

		if(!has_hamiltonian_path(s, t, width, height)) return {};

		// peel the matrix
		peeling r = peel(s, t, width, height);

		std::list<std::pair<int, int>> ret = find_path_m5(s, t, r);

		if(!ret.empty()
				|| (r.r2 - r.r1 == 1 && !(height == 5 && s.first == 3 && t.first == 3))
				|| (r.r4 - r.r3 == 1 && !(width == 5 && s.second == 3 && t.second == 3))
				|| (r.r2 - r.r1 == 2)
				|| (r.r4 - r.r3 == 2)) {

			if(ret.empty()) {
				if(r.r2 - r.r1 == 1) {
					if(r.r1 >= 2)
						r.r1--;
					else
						r.r2++;
				} else if(r.r4 - r.r3 == 1) {
					if(r.r3 >= 2)
						r.r3--;
					else
						r.r4++;
				} else if(r.r2 - r.r1 == 2) {
					// then it's a 2xm matrix
					// all we need to do is lend either the left or the right column
					if(std::min(s.second, t.second) > 0 && r.r3 > 0) {
						// lend the left column if M1 is not empty
						r.r3++;
					} else {
						// lend the right column
						r.r4--;
					}
				} else if(r.r4 - r.r3 == 2) {
					// then it's a nx2 matrix
					// all we need to do is lend either the top or the bottom row
					if(std::min(s.first, t.first) > 0 && r.r1 > 0) {
						// lend the top row if M3 is not empty
						r.r1++;
					} else {
						r.r2--;
					}
				}

				ret = find_path_m5(s, t, r);

			}

			for(auto& elem : ret) {
				elem.first += r.r1 + 1;
				elem.second += r.r3 + 1;
			}
			// right should be flat
			auto m1 = find_hamiltonian_cycle(M1, r.r3 + 1, height);
			// left should be flat
			auto m2 = find_hamiltonian_cycle(M2, width - 1 - r.r4, height);
			// bottom should be flat
			auto m3 = find_hamiltonian_cycle(M3, r.r4 - r.r3, r.r1 + 1);
			// top should be flat
			auto m4 = find_hamiltonian_cycle(M4, r.r4 - r.r3, height - 1 - r.r2);

			// connect cycles
			// M1 --> M3
			if(!m3.empty()) {
				// we connect M1's (0, m-1) to M3's (0, 0)
				// if (0, m-1) is followed by (1, m-1)
				// there are two cases:
				// 1) (0, 0) is followed by (0, 1) in M3
				// just append all elements of M3 to M1 after the element (0, m-1)
				// 2) (0, 0) is followed by (1, 0) in M3
				// so, M3 ends in (1, 0)
				// in that case, we just iterate over M3 backwards

				// if (0, m-1) is not followed by (1, m-1)
				// then connect (1, m-1) to (1, 0) in M3

				// find (0, 0) in M3
				// find the successor of (0, 0)
				// so, in case (0, 0) is at the end of the list
				// its successor is m3.begin()

				auto m1_it = std::find_if(m1.begin(), m1.end(), [r](const std::pair<int, int>& p){return !p.first && p.second == r.r3;});
				++m1_it;

				if(m1_it->first != 1) {
					--m1_it;
					// connect (1, m-1) to (1, 0) (in M3)
					auto it = std::find_if(m3.begin(), m3.end(), [](const std::pair<int, int>&p){return p.first == 1 && !p.second;});
					auto next = ++it;
					--it;
					std::cout << *it << ", " << *next << '\n';
					if(next == m3.end()) next = m3.begin();
					// if (0, 0) goes after (1, 0), we trace the M3 backwards
					if(!(!(*next).first) && !(!(*next).second)) {
						auto I = std::reverse_iterator(it);
						for( ; I != m3.rend(); I++) {
							(*I).second += r.r3 + 1;
							m1_it = m1.insert(m1_it, *I);
						}
						for(I = m3.rbegin(); I != std::reverse_iterator(it); I++) {
							(*I).second += r.r3 + 1;
							m1_it = m1.insert(m1_it, *I);
						}
					} else {
						for(auto I = it; I != m3.end(); I++) {
							(*I).second += r.r3 + 1;
							m1.insert(m1_it, *I);
						}
						for(auto I = m3.begin(); I != it; I++) {
							(*I).second += r.r3 + 1;
							m1.insert(m1_it, *I);
						}
					}

				} else {
					auto it = std::find_if(m3.begin(), m3.end(), [](const std::pair<int, int>& p){return !p.first && !p.second;});
					auto next = ++it;
					--it;
					if(next == m3.end()) {
						next = m3.begin();
					}
					if(!(!(*next).first) && !(*next).second) {
						// case 2 - we iterate backwards over M3 starting from (0, 0)
						auto I = std::reverse_iterator(it);
						for( ; I != m3.rend(); I++) {
							(*I).second += r.r3 + 1;
							m1_it = m1.insert(m1_it, *I);
						}
						for(I = m3.rbegin(); I != std::reverse_iterator(it); I++) {
							(*I).second += r.r3 + 1;
							m1_it = m1.insert(m1_it, *I);
						}
					} else {
						// case 1
						for(auto I = it; I != m3.end(); I++) {
							(*I).second += r.r3 + 1;
							m1.insert(m1_it, *I);
						}
						for(auto I = m3.begin(); I != it; I++) {
							(*I).second += r.r3 + 1;
							m1.insert(m1_it, *I);
						}
					}
				}

			}

			std::cout << "begin M1\n";
			for(const auto& elem : m1)
				std::cout << elem << '\n';
			std::cout << "end M1\n";

			// M1 and M3 --> M2
			if(!m2.empty()) {

			}

			// M1 and M2 and M3 --> M4
			if(!m4.empty()) {

			}

			// connect path and cycle

		} else {
			std::pair<int, int> s1 = {s.first - r.r1 - 1, s.second - r.r3 - 1};
			std::pair<int, int> t1 = {t.first - r.r1 - 1, t.second - r.r3 - 1};

			std::cout << "points:";
			std::cout << s1 << '\n' << t1 << '\n';
			int n = r.r2 - r.r1;
			int m = r.r4 - r.r3;
			std::cout << n << '\n' << m << '\n';

			// trisect M5
			if(n >= 4) {
				if(s1.first > t1.first) std::swap(s1, t1);
				std::pair<int, int> p = {0, (r.r3 <= 0) * 2};
				std::pair<int, int> q = {n - 1, (r.r3 <= 0) * 2};
				std::cout << p << ", " << q << '\n';

				ret = this->connect_s_p(s1, p, m, n);
				for(auto& elem : ret) {
					elem.first += r.r1 + 1;
					elem.second += r.r3 + 1;
				}
				// connect p and q
				if(r.r3 <= 0) {
					// use M2
					for(int i = 0; i < height; i++) {
						if(i % 2) {
							// go from right to left
							for(int j = width - 1; j >= r.r4 + 1; j--) {
								ret.push_back({i, j});
							}
						} else {
							for(int j = r.r4 + 1; j < width; j++) {
								ret.push_back({i, j});
							}
						}
					}
				} else {
					// use M1
					for(int i = 0; i < height; i++) {
						if(i % 2) {
							for(int j = r.r3; j >= 0; j--) {
								ret.push_back({i, j});
							}
						} else {
							for(int j = 0; j <= r.r3; j++) {
								ret.push_back({i, j});
							}
						}
					}
				}

				// connect q and t
				auto res = this->connect_q_t(q, t1, m, n);
				for(auto& elem : res) {
					elem.first += r.r1 + 1;
					elem.second += r.r3 + 1;
				}
				ret.insert(ret.end(), res.begin(), res.end());

			} else if(m >= 4) {
				// perform vertical cut
				if(s1.second > t1.second) std::swap(s1, t1);
				// if M3 is empty, we go to M4
				// thus, if r3 == 0, both p and q are 2
				// if M3 is not empty, they are 0
				std::pair<int, int> p = {(r.r1 <= 0) * 2, 0};
				std::pair<int, int> q = {(r.r1 <= 0) * 2, m - 1};
				std::cout << p << ", " << q << '\n';

				// connect s and p
				ret = connect_s_p(s1, p, m, n);
				// connect p and q via M3 or M4
				for(auto& elem : ret) {
					elem.first += r.r1 + 1;
					elem.second += r.r3 + 1;
				}

				if(r.r1 <= 0) {
					// use M4
					for(int j = r.r3 + 1; j <= r.r4; j++) {
						if((j - r.r3 - 1) % 2) {
							for(int i = height - 1; i >= r.r2 + 1; i--) {
								ret.push_back({i, j});
							}
						} else {
							for(int i = r.r2 + 1; i < height; i++)
								ret.push_back({i, j});
						}
					}
				} else {
					// use M3
					for(int j = r.r3 + 1; j <= r.r4; j++) {
						if((j - r.r3 - 1) % 2) {
							for(int i = r.r1; i >= 0; i--)
								ret.push_back({i, j});
						} else {
							for(int i = 0; i <= r.r1; i++)
								ret.push_back({i, j});
						}
					}
				}

				// connect q and t
				auto res = connect_q_t(q, t1, m, n);
				for(auto& elem : res) {
					elem.first += r.r1 + 1;
					elem.second += r.r3 + 1;
				}
				ret.insert(ret.end(), res.begin(), res.end());
				return ret;

			} else {
				return {};
			}

		}

		return ret;
	}

	std::pair<int, int> H_C_M1_M3_CCW(int m, int n, int x, int y) {
		if((n % 2) && (m % 2)) return {};

		if(!(n % 2)) {
			// za y parno, idemo levo
			// za y neparno, idemo desno
			// na koloni m-1 idemo gore

			if(y % 2) {
				// idemo desno za x < m - 2
				// za x == m - 2, spustamo se dole ako nije y == n - 1
				// ako je x == m - 2 i y == n - 1, onda idemo desno
				// za x == m - 1, idemo na gore
				return {y + (x == m - 2 && y < n - 1) - (x == m - 1), x + (x < m - 2) + (x == m - 2 && y == n - 1)};
			} else {
				// idemo levo za x > 0 i x < m - 1
				// x > 0 <-> !(!x) == 1 && x >= 0
				// za x == 0, spustamo se dole, osim kada je y == n - 1 (tada idemo desno)
				// za x == m - 1 idemo na gore kada nije y == 0
				// za x == m - 1 i y == 0 idemo levo
				return {y + (!x && y < n - 1) - (x == m - 1 && !(!y)), x - ((!(!x) && x < m - 1) || (!y && x == m - 1)) + (!x && y == n - 1)};

			}


		} else {
			// od donjeg desnog ugla na gore, pa cik-cak po kolonama na levo
			// za x neparno, idemo na gore
			// za parno x, idemo na dole
			// pazimo na y == 0, y == n - 2 i y == n - 1
			if(x % 2) {
				// za y == 0, skrecemo levo (x - 1, y)
				// za y == n - 1 idemo desno (x + 1, y), osim za x == m - 1 (tada idemo gore)
				// inace idemo samo gore (x, y - 1)
				// x koordinatu resava funkcija (x - 1)*!y + (x + 1)*(y == n - 1 && x != m - 1) + x*(y > 0 && y < n - 1) + x*(y == n - 1 && x == m - 1)
				// imamo x - !y + (y == n - 1 && x != m - 1)
				// ako ubacimo y == 0, imamo x - 1 + 0 = x - 1
				// y == n - 1, dobijamo x - 0 + 1 = x + 1
				// a inace x - 0 + 0 = x
				return {y - (!(!y) && y < n - 1) - (y == n - 1 && x == m - 1), x - !y + (y == n - 1 && x != m - 1)};

			} else {
				// za y < n - 2, idemo dole (x, y + 1)
				// za y == n - 2, idemo levo za x > 0 (x - 1, y), a inace na dole
				// za y == n - 1, idemo desno (x + 1, y)
				// x koordinata: x - (y == n - 2) + (y == n - 1)
				// y koordinata: y + (y > 0 && y < n - 2)
				return {y + (y < n - 2) + (!x && y == n - 2), x - (y == n - 2 && !(!x)) + (y == n - 1)};
			}


		}

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
		// top should be concave
		// so, we go bottom-up from the right corner
		for(int i = height - 1; i >= 0; i--)
			ret.push_back({i, width - 1});
		// we then go from right to left in a zig-zag fashion
		for(int j = width - 2; j >= 0; j--) {
			// width - 1, width - 3, ... --> up
			// width - 2, width - 4, ... --> down
			// width - 1 - j...when j = width - 2, it is 1
			// so, odd width - 1 - j means down
			// even j means up
			if((width - 1 - j) % 2) {
				for(int i = 0; i < height - 1; i++)
					ret.push_back({i, j});
			} else {
				for(int i = height - 2; i >= 0; i--)
					ret.push_back({i, j});
			}
		}
		// then, we turn right and cover the bottom face
		for(int j = 0; j < width - 1; j++)
			ret.push_back({height - 1, j});
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

	std::list<std::pair<int, int>> cycle = G.find_hamiltonian_cycle(Mesh<int>::mesh::M1, 4, 10);
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

	std::cout << "The path ends\n";

	path = G.find_path_m5(s, t, p);
	std::cout << "The path in M5 is:\n";
	for(const auto& l : path) {
		std::cout << '(' << l.first << ", " << l.second << ")\n";
	}


	std::cout << "HCM13\n";
	for(int y = 0; y < 4; y++) {
		for(int x = 0; x < 5; x++) {
			std::cout << G.H_C_M1_M3_CCW(5, 4, x, y) << " ";
		}
		std::cout << '\n';
	}

	std::pair<int, int> T = {0, 0};
	for(int y = 0; y < 4; y++) {
		for(int x = 0; x < 5; x++) {
			T = G.H_C_M1_M3_CCW(5, 4, T.second, T.first);
		}
	}

	std::cout << T << '\n';


	return 0;
}
