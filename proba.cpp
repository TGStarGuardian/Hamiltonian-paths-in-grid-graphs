#include <iostream>
#include <vector>
#include <chrono>


std::istream& operator>>(std::istream& is, std::pair<int, int>& p) {
	is >> p.first >> p.second;
	return is;
}

std::ostream& operator<<(std::ostream& os, const std::pair<int, int>& p) {
	os << '(' << p.first << ", " << p.second << ')';
	return os;
}

std::pair<int, int> operator+(const std::pair<int, int>&& x, const std::pair<int, int>&& y) {
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

std::pair<int, int> path_2_m(int m, int x, int y, const std::pair<int, int>& s, const std::pair<int, int>& t) {
	if(s.second < t.second) {
		// od s idemo skroz levo
		// onda menjamo traku, pa se vracamo desno do s
		// onda cik-cak nadesno do ispod/iznad t
		// pa skroz desno
		// pa menjamo traku i onda levo do t
		
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
			if(((x - s.second) % 2) ^ (y == s.first))
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
			if(t.second == m - 1) return t;
			else return {y, x + 1};
		} else {
			if(y == t.first) return {y, x - 1};
			else return {1 - y, x};
		}
		
	} else if(s.second > t.second) {		
		// ako posmatramo zdesna nalevo, s je levo, a t je desno
		// treba nove koordinate su m-1-s.second i m-1-t.second
		// samo treba oduzeti ih i dodati posle
		std::pair<int, int> s1 = {s.first, m - 1 - s.second}, t1 = {t.first, m - 1 - t.second},
		ret = path_2_m(m, m - 1 - x, y, s1, t1);
		if(ret.first == -1 || ret.second == -1) return {-1, -1};
		ret.second = m - 1 - ret.second;
		return ret;
		
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

std::pair<int, int> path_n_2(int m, int n, int x, int y, const std::pair<int, int>& s, const std::pair<int, int>& t) {
	// ako zamenimo mesta x i y, m i n i okrenemo koordinate s i t, dobijamo slucaj 2xm
	std::pair<int, int> s1 = {s.second, s.first};
	std::pair<int, int> t1 = {t.second, t.first};
	auto p = path_2_m(n, y, x, s1, t1);
	return {p.second, p.first};
}


// resavamo trisekciju u M5
std::pair<int, int> horizontal_trisection(int m, int n, int x, int y, const std::pair<int, int>& s, const std::pair<int, int>& t) {
	// postoje 2 slucaja - s je gore ili je t gore
	// podrazumevamo da su s i t antipodi, jer inace ovo ne radi
	// podrazumevamo i n >= 4, jer inace nema horizontalne trisekcije
	
	if(s.first < t.first) {
		// trazimo tacku p
		// p.first == 1
		// dakle, p.second = (m - 1)*((s.first + s.second) % 2)
		std::pair<int, int> p = {1, (m - 1)*((s.first + s.second) % 2)};
		// spajamo s sa p
		// ako je tacka u traci [0 - 1]x[0 - m-1] i nije jednaka p
		if(x == p.second && y == 1) return {2, p.second};
		if(y < 2) return path_2_m(m, x, y, s, p);
		// spajamo p sa q
		// idemo cik-cak
		// ako je p levo, onda na parnim y idemo desno
		// ako je p desno, na parnim y idemo levo
		if(2 <= y && y < n - 2) {
			// levo idemo ako je p levo i y neparno
			// ili ako je p desno, a y parno
			// p je levo ako je p.first == 0, a inace je desno
			// imamo (!p && neparno) || (p && parno)
			// (!A i B) ili (A i !B)
			// to je A xor B
			// ili !A xor !B
			// A = p.first, B = neparno
			// jedino treba da pazimo na x
			// ako idemo levo, x mora biti > 0 (y, x - 1)
			// ako idemo desno, x mora biti < m - 1 (y, x + 1)
			// za x == 0 i idemo levo, spustamo se dole (y + 1, x)
			// za x == m - 1 i idemo desno, spustamo se dole (y + 1, x)
			// y, y + 1 su opcije
			// y: (!(!x) && ((y % 2) ^ !!p.first)) || (x < m - 1 || ((y % 2)  == !!p.first))
			// Fy + !F(y + 1) == y + !F
			// x - 1, x i x + 1
			// x - 1: (!(!x) && ((y % 2) ^ !p.first))
			// x + 1: (x < m - 1 && ((y % 2) == !p.first))
			// A(x - 1) + B(x + 1) + (!A && !B)x
			// F == A || B
			bool A = (!!x && ((y % 2) ^ !!p.second));
			bool B = (x < m - 1 && !((y % 2) ^ !!p.second));
			return {y + !(A || B), A*(x - 1) + B*(x + 1) + !(A || B)*x};
		} else {
			// spajamo q sa t
			// ako je n - 2 parno i p je levo, isli smo desno
			// ako je n - 2 parno i p je desno, isli smo levo
			// ako je n - 2 neparno i p je levo, isli smo levo
			// ako je n - 2 neparno i p desno, isli smo desno
			// to znaci da je x-koordinata od q ili 0 ili m - 1 s obzirom na ove uslove
			// ona je m - 1 ako je n - 2 parno i p je levo ili ako je n - 2 neparno i p je desno
			// (parno && !p) || (neparno && p)
			// opet A xor B, uzmimo A = neparno, B = !p
			std::pair<int, int> q = {0, !(n % 2)*(!!(p.second) * (m - 1)) + (n % 2)*(!p.second * (m - 1))};
			// sada povezujemo q sa t
			// prvo smanjimo koordinate, pa ih povecamo
			// visina 0 je isto sto i n - 2
			std::pair<int, int> t1 = {t.first - n + 2, t.second}, ret = path_2_m(m, x, y - n + 2, q, t1);
			if(ret.first == -1 || ret.second == -1) return {-1, -1};
			ret.first += n - 2;
			return ret;
			
			}
	} else {
		// mozemo krenuti od n-1
		std::pair<int, int> s1 = {n - 1 - s.first, s.second}, t1 = {n - 1 - t.first, t.second};
		std::pair<int, int> ret = horizontal_trisection(m, n, x, n - 1 - y, s1, t1);
		if(ret.first == -1 || ret.second == -1) return {-1, -1};
		ret.first = n - 1 - ret.first;
		return ret;
	}
}

std::pair<int, int> vertical_trisection(int m, int n, int x, int y, std::pair<int, int>& s, std::pair<int, int>&t) {
	// mozemo rotirati i uraditi horizontalnu
	std::pair<int, int> s1 = {s.second, s.first}, t1 = {t.second, t.first};
	std::pair<int, int> ret = horizontal_trisection(n, m, y, x, s1, t1);
	return {ret.second, ret.first};
}


int main() {

	std::pair<int, int> s, t;
	int n, m;
	
	std::cout << "Unesi s: ";
	std::cin >> s;
	std::cout << "Unesi t: ";
	std::cin >> t;
	std::cout << "Unesi m: ";
	std::cin >> m;
	std::cout << "Unesi n: ";
	std::cin >> n;
/*	
	std::pair<int, int> x = s;
	auto start = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 1000000; j++) {
			x = path_n_2(2, 1000000, x.second, x.first, s, t);
			// std::cout << x << '\n';
		}
	}
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << duration.count() << '\n';
	std::cout << x << '\n';
*/
	
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < m; j++) {
			std::cout << vertical_trisection(m, n, j, i, s, t) << " ";
		}
		std::cout << '\n';
	}
	/*
	std::pair<int, int> p = {1, 0};
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 7; j++) {
			std::cout << path_2_m(7, j, i, s, p) << " ";
		}
		std::cout << '\n';
	}
	*/
	/*
	std::pair<int, int> x1 = {1, 6}, x2 = {1, 7};
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 8; j++) {
			std::cout << path_2_m(8, j, i, x1, x2) << " ";
		}
		std::cout << '\n';
	}
	*/
	return 0;
}
