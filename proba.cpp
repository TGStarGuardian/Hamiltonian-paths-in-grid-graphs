#include <iostream>
#include <vector>


std::istream& operator>>(std::istream& is, std::pair<int, int>& p) {
	is >> p.first >> p.second;
	return is;
}

std::ostream& operator<<(std::ostream& os, const std::pair<int, int>& p) {
	os << '(' << p.first << ", " << p.second << ')';
	return os;
}

std::pair<int, int> operator+(const std::pair<int, int>& x, const std::pair<int, int>& y) {
	return {x.first + y.first, x.second + y.second};
}

std::pair<int, int> operator+=(std::pair<int, int>& x, std::pair<int, int>&& y) {
	x.first += y.first;
	x.second += y.second;
	return x;
}

std::pair<int, int> operator*(int a, const std::pair<int, int>& x) {
	return {a * x.first, a * x.second};
}

std::pair<int, int> path_2_m(int m, int n, int x, int y, std::pair<int, int>& s, std::pair<int, int>& t) {
	if(s.second < t.second) {
		// od s idemo skroz levo
		// onda menjamo traku, pa se vracamo desno do s
		// onda cik-cak nadesno do ispod/iznad t
		// pa skroz desno
		// pa menjamo traku i onda levo do t
		/*
		if(x > 0 && x <= s.second) {
			if(y == s.first) return {y, x - 1};
			else return {y, x + 1};
		} else if(x == 0) {
			if(y == s.first) return {1 - y, 0};
			else return {y, x + 1};
		} else if(x > s.second && x < t.second - 1) {
			// ako je x parno u odnosu na s.second
			// i y = s.first, onda idemo desno
			// ako je x parno, a y != s.first, menjamo traku
			// ako je x neparno, onda obrnuto
			// (A && !B) || (!A && B)
			// !(A => B) || !(B => A)
			// to je A xor B
			// A = (x - s.second) % 2
			// B = y != s.first;
			if(((x - s.second) % 2) ^ (y != s.first))
				return {1 - y, x};
			else return {y, x + 1};
		} else if(x == t.second - 1) {
			// ako je y == t.first, onda na gore
			// a inace desno
			if(y == t.first) return {1 - y, x};
			else return {y, x + 1};
		} else if(x > t.second && x < m - 1) {
			if(y == t.first) return {y, x - 1};
			else return {y, x + 1};
		} else if(x == t.second) {
			if(y == t.first) return {-1, -1};
			else return {y, x + 1};
		} else {
			if(y == t.first) return {y, x - 1};
			else return {1 - y, x};
		}
		*/
		// y, 1 - y ili -1
		// 1 - y: (!x && y == s.first) || (x > s.second && x < t.second - 1 && (((x - s.second) % 2) ^ (y != s.first)))
		// || (x == t.second - 1 && y == t.first) || (x == m - 1 && y != t.first)
		// -1: (x == t.second && y == t.first)
		// F(1 - y) - A + (!F && !A)y
		// F - Fy - A + (!F && !A)y
		// F - A + y(!F && !A - F) = F - A + y(1 - 2F)
		
		// x, x + 1, x - 1, -1
		// x: (x == 0 && y == s.first) || ((((x - s.second) % 2) ^ (y != s.first)) && x > s.second && x < t.second - 1)
		// || (x == t.second - 1 && y == t.first) || (x == m - 1 && y != t.first)
		// x - 1: (x > 0 && x <= s.second && y == s.first) || (x > t.second && x < m - 1 && y == t.first) || (x == m - 1 && y == t.first)
		// Fx - A + B(x - 1) + (!F && !A && !B)(x + 1)
		bool Fy =  (!x && y == s.first) || (x > s.second && x < t.second - 1 && (((x - s.second) % 2) ^ (y != s.first)))
		|| (x == t.second - 1 && y == t.first) || (x == m - 1 && y != t.first);
		bool A = (x == t.second && y == t.first);
		bool Fx = (x == 0 && y == s.first) || ((((x - s.second) % 2) ^ (y != s.first)) && x > s.second && x < t.second - 1)
		|| (x == t.second - 1 && y == t.first) || (x == m - 1 && y != t.first);
		bool Bx = (x > 0 && x <= s.second && y == s.first) || (x > t.second && x < m - 1 && y == t.first) || (x == m - 1 && y == t.first);
		return {Fy*(1 - y) - A + (!Fy && !A)*y , Fx*x - A + Bx*(x - 1) + (!Fx && !A && !Bx)*(x + 1)};

	} else if(s.second > t.second) {
		// od s idemo desno
		// pa menjamo traku i vracamo se levo do s
		// onda idemo cik-cak do t, pa skroz levo
		// i na kraju menjamo traku i idemo desno
		
		// za s.x <= x < m - 1 i y == s.y, idemo desno (y, x + 1)
		// za s.x <= x < m - 1 i y != s.y, idemo levo (y, x - 1)
		// za x == m - 1, idemo dole (1 - y, x) ako je y == s.y, a inace levo (y, x - 1)
		// za t.x + 1 < x < s.x, idemo cik-cak
		// 	za x parno u odnosu na s.x, idemo na ... (1 - y, x) ako je y == s.first, a inace levo (y, x - 1)
		//	za x neparno u odnosu na s.x, idemo ... (1 - y, x) ako je y != s.first, a inace levo (y, x - 1)
		// za x < t.x i y == t.y, idemo desno (y, x - 1)
		// za x < t.x i y != t.y, idemo levo (y, x + 1)
		// za x == t.x + 1 i y == t.y, menjamo traku (1 - y, x)
		// za x == t.x + 1 i y != t.y, idemo levo
		// za x == t.x i y == t.y, {-1, -1}
		// za x == t.x i y != t.y, idemo levo (y, x - 1)
		// za 0 < x < t.x i y == t.y, idemo desno (y, x + 1)
		// za 0 < x < t.x i y != t.y, idemo levo (y, x - 1)
		// za x == 0, menjamo traku (1 - y, x) ako je y != t.first, a inace idemo desno (y, x + 1)
		
		// uslov za t.x < x < s.x da se menja traka je (!mod%2 && y == 0) || (mod%2 && y == 1)
		// A = (s.x - x) % 2
		// B = y != s.first
		// 1 - y: (x == m - 1 && y == s.first) || (t.second + 1 < x && x < s.second && (((s.second - x) % 2) ^ y != s.first)) || (!x && y != t.first)
		// || (x == t.second + 1 && y == t.first)
		// -1: x == t.second && y == t.first
		
		// x: (x == m - 1 && y == s.first) || (t.second + 1 < x && x < s.second && (((s.second - x) % 2) ^ !y)) || (!x && y != t.first)
		// x + 1: (s.second <= x && x < m - 1 && y == s.first) || (x < t.second && y == t.first) || (!(!x) && x < t.second && y == t.first)
		// || (!x && y == t.first)
		
		bool Fy = (x == m - 1 && y == s.first) || (t.second + 1 < x && x < s.second && (((s.second - x) % 2) ^ y == s.first)) || (!x && y != t.first)
		|| (x == t.second + 1 && y == t.first);
		bool A = x == t.second && y == t.first;
		bool Fx = (x == m - 1 && y == s.first) || (t.second + 1 < x && x < s.second && (((s.second - x) % 2) ^ y == s.first))
		|| (!x && y != t.first) || (x == t.second + 1 && y == t.first);
		bool Bx = (s.second <= x && x < m - 1 && y == s.first) || (x < t.second && y == t.first)
		|| (!(!x) && x < t.second && y == t.first) || (!x && y == t.first);
		
		return {Fy*(1 - y) - A + (!Fy && !A)*y, Fx*x - A + Bx*(x + 1) + (!Fx && !A && !Bx)*(x - 1)};
	} else {
		// ako su oba na 0, onda desno, pa dole/gore, pa levo
		// ako su oba na m-1, onda levo, pa dole/gore, pa desno
		if(!s.second) {
			// za y == s.first, idemo desno (y, x + 1) ako je x < m - 1
			// za x == m - 1 i y == s.first, menjamo traku (1 - y, x)
			// za y != s.first, idemo levo (y, x - 1) ako je x > 0
			// za y != s.first i x == 0, {-1, -1}
			// 1 - y: x == m - 1 && y == s.first
			bool Fy = x == m - 1 && y == s.first;
			bool A = y != s.first && !x;
			// x: x == m - 1 && y == s.first
			// x - 1: !(!x) && y != s.first
			bool Fx = x == m - 1 && y == s.first;
			bool Bx = !(!x) && y != s.first;
			return {Fy*(1 - y) - A + (!Fy && !A)*y, Fx*x + Bx*(x - 1) - A + (!Fx && !Bx && !A)*(x + 1)};
		} else if(s.second == m - 1) {
			// za y == s.first, idemo levo (y, x - 1) ako je x > 0
			// za y == s.first i x == 0, menjamo traku (1 - y, x)
			// za y != s.first i x < m - 1, idemo desno
			// za y != s.first i x == m - 1, {-1, -1}
			// 1 - y: x == 0 && y == s.first
			// x: y == s.first && !x
			// x - 1: !(!x) && y == s.first
			bool Fy = !x && y == s.first;
			bool A = y == t.first && x == t.second;
			bool Fx = !x && y == s.first;
			bool Bx = !(!x) && y == s.first; 
			return {Fy*(1 - y) - A + (!Fy && !A)*y, Fx*x + Bx*(x - 1) - A + (!Fx && !Bx && !A)*(x + 1)};;	
		} else {
			return {-1, -1};
		}
	
	}
}



int main() {

	std::pair<int, int> s, t;
	
	std::cout << "Unesi s: ";
	std::cin >> s;
	std::cout << "Unesi t: ";
	std::cin >> t;
	
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 8; j++) {
			std::cout << path_2_m(8, 2, j, i, s, t) << " ";
		}
		std::cout << '\n';
	}


	return 0;
}
