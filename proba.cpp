#include <iostream>
#include <vector>
#include <array>
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

bool operator==(std::pair<int, int>& s, std::pair<int, int>& t) {
	return s.first == t.first && s.second == t.second;
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

std::pair<int, int> path_n_2(int n, int x, int y, const std::pair<int, int>& s, const std::pair<int, int>& t) {
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
	// takodje se podrazumeva da je m >= 2
	
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

typedef struct peeling {
	int r1, r2, r3, r4;

	friend std::ostream& operator<<(std::ostream& os, peeling& p) {
		os
		<< "r1: " << p.r1
		<< ", r2: " << p.r2
		<< ", r3: " << p.r3
		<< ", r4: " << p.r4
		<< '\n';
		return os;
	}

} peeling;

inline bool out_of_bounds(int x, int l, int r) {
	return (x < l && x > r);
}

inline bool is_corner_vertex(int x, int m) {
	// samo ako je x == 0 ili x == m - 1
	// ako je x == m - 1, onda je x - m - 1 == 0
	return !x || !(x - m + 1);
}

inline bool connected_by_non_boundary_edge(std::pair<int, int>& s, std::pair<int, int>& t, int m, int n) {
	// ovo implicira da je matrica 2xm ili nx2
	// za 2xm je ovo ispunjeno ako su ime iste x-koordinate, razlicite y-koordinate i x > 0 i x < m - 1
	// za nx2 slicno
	// s.first == t.first, s.first, t.first su iz {0, 1}
	// s.first - t.first == 0
	// s.first xor t.first
	return (n == 2 && s.second == t.second && !is_corner_vertex(s.second, m) && (s.first ^ t.first))
	|| (m == 2 && s.first == t.first && !is_corner_vertex(s.first, n) && (s.second ^ t.second));
}

bool color_compatible(std::pair<int, int>& v1, std::pair<int, int>& v2, int width, int height) {
	int parity = (width % 2) && (height % 2);
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

bool has_hamiltonian_path(std::pair<int, int> s, std::pair<int, int> t, int m, int n) {

	if(out_of_bounds(s.first, 0, n)
		|| out_of_bounds(s.second, 0, m)
		|| out_of_bounds(t.first, 0, n)
		|| out_of_bounds(t.second, 0, m)) {
	throw "Error: either s or t are outside the matrix";
}
	return color_compatible(s, t, m, n) &&
		!( (n == 1 && (!is_corner_vertex(s.second, m) || !is_corner_vertex(t.second, m)))
		 || (m == 1 && (!is_corner_vertex(s.first, n) || !is_corner_vertex(t.first, n)))
		 || connected_by_non_boundary_edge(s, t, m, n)
		 // n == 3, m parno...u radu stoji da treba biti izomorfno sa s crna, s.x < t.x (za sy == 1) ili t.x > s.x + 1 (za sy != 1)
		 // to znaci da mora biti ovo da vazi i kad rotiramo sliku i kad rotiramo uloge za s i t
		 // znaci, potrebno je obraditi slucaj kada se slika zarotira oko y-ose
		 // ali tada se menjaju boje
		 // tako da je potrebno samo razmotriti ovaj slucaj kada je s crna ili kada je t crna 
		 || (n == 3 && !(m % 2) && ((s.first + s.second) % 2) && ((s.first == 1 && s.second < t.second) || (s.first != 1 && t.second > s.second + 1)))
		 || (n == 3 && !(m % 2) && ((t.first + t.second) % 2) && ((t.first == 1 && t.second < s.second) || (t.first != 1 && s.second > t.second + 1)))
		 || (m == 3 && !(n % 2) && ((s.second + s.first) % 2) && ((s.second == 1 && s.first < t.first) || (s.second != 1 && t.first > s.first + 1)))
		 || (m == 3 && !(n % 2) && ((t.first + t.second) % 2) && ((t.second == 1 && t.first < s.first) || (t.second != 1 && s.first > t.first + 1)))
		);
}


std::pair<int, int> H_C_M1_M3_CCW(int m, int n, int x, int y) {
	// leva ili gornja strana ce biti konkavne
	// desna mora biti ravna zbog M1
	// donja mora biti ravna zbog M3
	if((n % 2) && (m % 2)) return {-1 , -1};

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

// CW za M1 dobijam tako sto okrenem matricu oko x-ose
// pa pozovem funkciju i vratim koordinate
std::pair<int, int> H_C_M1_CW(int m, int n, int x, int y) {
	auto ret = H_C_M1_M3_CCW(m, n, x, n - 1 - y);
	ret.first = n - 1 - ret.first;
	return ret;
}

// CW za M3 dobijam tako sto rotiam matricu oko y-ose
// pa pozovem funkciju i vratim koordinate

std::pair<int, int> H_C_M3_CW(int m, int n, int x, int y) {
	auto ret = H_C_M1_M3_CCW(m, n, m - 1 - x, y);
	ret.second = m - 1 - ret.second;
	return ret;
}

// za M2 je bitno da leva bude ravna
// za M4 da bude to gornja
// H_C_M1_M3_CCW obezbedjuje da su donja i desna ravne
// ako je rotiramo po obe ose, ostajemo na CCW
// ali sada za M2 i M4

std::pair<int, int> H_C_M2_M4_CCW(int m, int n, int x, int y) {
	auto ret = H_C_M1_M3_CCW(m, n, m - 1 - x, n - 1 - y);
	ret.second = m - 1 - ret.second;
	ret.first = n - 1 - ret.first;
	return ret;
}


// CW za M4 dobijam tako sto rotiam matricu oko y-ose
// pa pozovem funkciju i vratim koordinate

std::pair<int, int> H_C_M4_CW(int m, int n, int x, int y) {
	auto ret = H_C_M2_M4_CCW(m, n, m - 1 - x, y);
	ret.second = m - 1 - ret.second;
	return ret;
}

// CW za M2 dobijam tako sto okrenem matricu oko x-ose
// pa pozovem funkciju i vratim koordinate
std::pair<int, int> H_C_M2_CW(int m, int n, int x, int y) {
	auto ret = H_C_M2_M4_CCW(m, n, x, n - 1 - y);
	ret.first = n - 1 - ret.first;
	return ret;
}

peeling peel(std::pair<int, int>& s, std::pair<int, int>& t, int m, int n) {
	peeling r;

	r.r1 = std::min(s.first, t.first) - 1;
	r.r2 = std::max(s.first, t.first);
	r.r3 = std::min(s.second, t.second) - 1;
	r.r4 = std::max(s.second, t.second);

	// if(!(r.r1 % 2)) r.r1--;
	r.r1 -= !(r.r1 % 2);
	// if(!(_height - r.r2) % 2) r.r2++;
	r.r2 += !((n - r.r2) % 2);
	// if(!(r.r3 % 2)) r.r3--;
	r.r3 -= !(r.r3 % 2);
	// if(!(_width - r.r4) % 2) r.r4++;
	r.r4 += !((m - r.r4) % 2);
	return r;
}

constexpr inline int f(int y, int x) {
	return x + 3*y;
}

constexpr inline std::pair<int, int> f_inv(int c) {
	return {c / 3, c % 3};
}

inline std::pair<int, int> reflect_over_x(int x, int y, int n) {
	return {n - 1 - y, x};
}

inline std::pair<int, int> reflect_over_y(int x, int y, int m) {
	return {y, m - 1 - x};
}

inline std::pair<int, int> reflect_over_xy(int x, int y, int m, int n) {
	return {n - 1 - y, m  - 1 - x};
}


std::pair<int, int> find_path_m5(int m, int n, int x, int y, std::pair<int, int>& s, std::pair<int, int>& t) {
	if(n >= 4 && (m > 3 || (m == 3 && !((s.first + s.second) % 2) ))) return horizontal_trisection(m, n, x, y, s, t);
	
	if(n >= 4) {
		// za m == 1 i m == 2, imamo vec dole obradjene slucajeve
		// za m == 3, resavamo ovde
		if(m == 3) {
			// tada znamo da je s crna, jer nije prosao gornji slucaj
			// ovde je junction vertex na sredini
			// ako je t iznad s, onda okrenemo graf po x-osi
			
			if(t.second < s.second) {
				auto s1 = reflect_over_x(s.second, s.first, n);
				auto t1 = reflect_over_x(t.second, t.first, n);
				auto ret = find_path_m5(m, n, x, n - 1 - y, s1, t1);
				if(ret.first == -1 || ret.second == -1) return {-1, -1};
				return reflect_over_x(ret.second, ret.first, n);
			}
			
			// sada je s gore
			// spajamo s sa (1, 1), (1, 1) sa (2, 1), a (2, 1) sa t
			if(y <= 1) {
				if(x == 1 && y == 1) return {2, 1};
				std::pair<int, int> p = {1, 1};
				return path_2_m(3, x, y, s, p);
			} else {
				std::pair<int, int> q = {0, 1}, t1 = {t.first - 2, t.second};
				auto ret = path_2_m(3, x, y - 2, q, t1);
				if(ret.first == -1 || ret.second == -1) return {-1, -1};
				ret.first += 2;
				return ret;
			}
		}
	}
	
	if(m >= 4 && (n > 3 || (n == 3 && !((s.first + s.second) % 2) ))) return vertical_trisection(m, n, x, y, s, t);
	
	if(m >= 4) {
		if(n == 3) {
			if(t.first < s.first) {
				auto s1 = reflect_over_y(s.second, s.first, m);
				auto t1 = reflect_over_y(t.second, t.first, m);
				auto ret = find_path_m5(m, n, m - 1 - x, y, s1, t1);
				if(ret.first == -1 || ret.second == -1) return {-1, -1};
				return reflect_over_y(ret.second, ret.first, m);
			}
			
			// spajamo s sa (1, 1), (1, 1) sa (1, 2), a (1, 2) sa t
			if(x <= 1) {
				if(x == 1 && y == 1) return {1, 2};
				std::pair<int, int> p = {1, 1};
				return path_n_2(3, x, y, s, p);
			} else {
				std::pair<int, int> q = {1, 0}, t1 = {t.first, t.second - 2};
				auto ret = path_n_2(3, x - 2, y, q, t1);
				if(ret.first == -1 || ret.second == -1) return {-1, -1};
				ret.second += 2;
				return ret;
			}
		}
	
	}
	
	
	// ovde je m < 4 i n < 4
	// imamo slucajeve:
	
	// 1x2, 1x3, 2x1, 3x1
	if(m == 1) {
		// vracamo onaj dole/gore (ako on nije poslednji)
		if(!s.first && y <= n - 1) return {y + 1, x};
		if(s.first && y >= 0) return {y - 1, x};
		else return {-1, -1};
	}
	
	if(n == 1) {
		if(!s.second && x <= m - 1) return {y, x + 1};
		if(s.second && x >= 0) return {y, x - 1};
		else return {-1, -1};
	}
	
	// 2x2, 2x3, 3x2 --> pozovemo funkciju za 2xm i nx2
	if(n == 2) return path_2_m(m, x, y, s, t);
	if(m == 2) return path_n_2(n, x, y, s, t);
	
	// 3x3
	// s i t su beli, jer inace nema puta
	// imamo 4 slucaja:
	// 1) (0, 0) --> (2, 2) i varijacije
	// 2) (0, 0) --> (2, 0) i varijacije
	// 3) (0, 0) --> (1, 1) i varijacije
	// 4) (1, 1) --> (0, 0) i varijacije
	// konstruisemo 4 liste da obuhvate te slucajeve
	// ostale svodimo izomorfnim transformacijama na ta 4 slucaja
	// a[i] predstavlja sledeci element od {x, y} koji odgovara i
	// numerisemo {x, y} brojevima od 0 do 8
	// {0, 0} --> 0, {1, 0} --> 1, {2, 2} --> 8
	// f(x, y) = x + 3y
	std::array<int, 9> a1 = {f(1, 0), f(0, 2), f(1, 2), f(2, 0), f(0, 1), f(2, 2), f(2, 1), f(1, 1), f(0, -1)};
	std::array<int, 9> a2 = {f(1, 0), f(0, 2), f(1, 2), f(1, 1), f(0, 1), f(2, 2), f(0, -1), f(2, 0), f(2, 1)};
	std::array<int, 9> a3 = {f(1, 0), f(1, 1), f(0, 1), f(2, 0), f(0, -1), f(0, 2), f(2, 1), f(2, 2), f(1, 2)};
	std::array<int, 9> a4 = {f(0, -1), f(0, 0), f(0, 1), f(2, 0), f(1, 0), f(0, 2), f(2, 1), f(2, 2), f(1, 2)};
	
	
	if(!s.first && !s.second) {
		// s je (0, 0)
		// granamo po t
		// t moze biti (0, 2), (2, 0), (1, 1) i (2, 2)
		if(!t.second) {
			// t je (2, 0)
			// vracamo sledeci iz liste a2
			return f_inv(a2[f(y, x)]);
		}
		
		if(!t.first) {
			// rotiramo listu a2
			// (0, 0) ide u (1, 0), a trebalo bi u (0, 1)
			// (1, 1) ide u (0, 1), a trebalo bi u (1, 0)
			// (2, 2) ide u (2, 1), a trebalo bi u (1, 2)
			// znaci, samo rotiramo, pa vadimo iz a2, pa rotiramo
			auto ret = f_inv(a2[f(x, y)]);
			if(ret.first == -1 || ret.second == -1) return {-1, -1};
			return {ret.second, ret.first};
		}
		
		if(t.first == 1 && t.second == 1) {
			// vratimo sledeci iz a3
			return f_inv(a3[f(y, x)]);
		}
		
		if(t.first == 2 && t.second == 2) {
			// vratimo sledeci iz a1
			return f_inv(a1[f(y, x)]);
		}
		
	} else if(!s.first) {
		// s je (0, 2)
		// t moze biti (0, 0), (1, 1), (2, 0) i (2, 2)
		if(!t.first && !t.second) {
			// t je (0, 0)
			// ovo je kompozicija refleksije po y-osi i rotacije slucaja a2
			// refleksijom se dobija s' = (0, 0), t' = (0, 2)
			// a rotacijom s" = (0, 0), t" = (0, 0)
			auto step1 = reflect_over_y(x, y, 3);
			auto ret = f_inv(a2[f(step1.second, step1.first)]);
			// opet rotiramo, pa reflektujemo
			if(ret.first == -1 || ret.second == -1) return {-1, -1};
			return reflect_over_y(ret.first, ret.second, 3);
		}
		
		if(t.first == 2 && !t.second) {
			// t je (2, 0)
			// ovo je kompozicija rotacije i refleksije po x-osi slucaja a1
			// rotacija daje s' = (2, 0), t' = (0, 2)
			// refleksija daje s" = (0, 0), t"= (2, 2)
			auto step1 = reflect_over_x(y, x, 3);
			auto step2 = f_inv(a1[f(step1.first, step1.second)]);
			if(step2.first == -1 || step2.second == -1) return {-1, -1};
			auto ret = reflect_over_x(step2.second, step2.first, 3);
			return {ret.second, ret.first};
		}
		
		if(t.first == 1 && t.second == 1) {
			// ovo je refleksija po y-osi slucaja a3
			// s' = (0, 0), t' = (1, 1)
			auto step1 = reflect_over_y(x, y, 3);
			auto step2 = f_inv(a3[f(step1.first, step1.second)]);
			if(step2.first == -1 || step2.second == -1) return {-1, -1};
			return reflect_over_y(step2.second, step2.first, 3);
		}
		
		if(t.first == 2 && t.second == 2) {
			// ovo je refleksija po y-osi slucaja a2
			// s' = (0, 0), t' = (2, 0)
			auto step1 = reflect_over_y(x, y, 3);
			auto step2 = f_inv(a2[f(step1.first, step1.second)]);
			if(step2.first == -1 || step2.second == -1) return {-1, -1};
			return reflect_over_y(step2.second, step2.first, 3);
		}
		
	} else if(!s.second) {
		// s je (2, 0)
		// t moze biti (0, 0), (1, 1), (0, 2), (2, 2)
		
		if(!t.first && !t.second) {
			// refleksija po x-osi slucaja a2
			auto step1 = reflect_over_x(x, y, 3);
			auto step2 = f_inv(a2[f(step1.first, step1.second)]);
			if(step2.first == -1 || step2.second == -1) return {-1, -1};
			return reflect_over_x(step2.second, step2.first, 3);
		}
		
		if(!t.first && t.second == 2) {
			// refleksija po x-osi slucaja a1
			auto step1 = reflect_over_x(x, y, 3);
			auto step2 = f_inv(a1[f(step1.first, step1.second)]);
			if(step2.first == -1 || step2.second == -1) return {-1, -1};
			return reflect_over_x(step2.second, step2.first, 3);
		}
		
		if(t.first == 1 && t.second == 1) {
			// refleksija po x-osi slucaja a3
			auto step1 = reflect_over_x(x, y, 3);
			auto step2 = f_inv(a3[f(step1.first, step1.second)]);
			if(step2.first == -1 || step2.second == -1) return {-1, -1};
			return reflect_over_x(step2.second, step2.first, 3);
		}
		
		if(t.first == 2 && t.second == 2) {
			// refleksija po x-osi, pa onda rotacija slucaja a2
			auto step1 = reflect_over_x(x, y, 3);
			auto ret = f_inv(a2[f(step1.second, step1.first)]);
			if(ret.first == -1 || ret.second == -1) return {-1, -1};
			// opet rotiramo, pa reflektujemo
			return reflect_over_x(ret.first, ret.second, 3);
		}
		
	} else if(s.first == 1) {
		// s je (1, 1)
		// t moze biti (0, 0), (2, 0), (0, 2) i (2, 2)
		
		if(!t.first && !t.second) {
			// slucaj a4
			return f_inv(a4[f(y, x)]);
		}
		
		if(!t.first) {
			// refleksija po x-osi slucaja a4
			auto step1 = reflect_over_y(x, y, 3);
			auto step2 = f_inv(a4[f(step1.first, step1.second)]);
			if(step2.first == -1 || step2.second == -1) return {-1, -1};
			return reflect_over_y(step2.second, step2.first, 3);
		}
		
		if(!t.second) {
			// refleksija po y-osi slucaja a4
			auto step1 = reflect_over_x(x, y, 3);
			auto step2 = f_inv(a4[f(step1.first, step1.second)]);
			if(step2.first == -1 || step2.second == -1) return {-1, -1};
			return reflect_over_x(step2.second, step2.first, 3);
		}
		
		if(t.first && t.second) {
			// refleksija po x i refleksija po y-osi slucaja a4
			// to je zapravo centralna simetrija
			auto step1 = reflect_over_xy(x, y, 3, 3);
			auto step2 = f_inv(a4[f(step1.first, step1.second)]);
			if(step2.first == -1 || step2.second == -1) return {-1, -1};
			return reflect_over_xy(step2.second, step2.first, 3, 3);
		}
		
	} else {
		// s je (2, 2)
		// t moze biti (0, 0), (0, 2), (2, 0), (1, 1)
		if(!t.first && !t.second) {
			// centralna simetrija od a1
			auto step1 = reflect_over_xy(x, y, 3, 3);
			auto step2 = f_inv(a1[f(step1.first, step1.second)]);
			if(step2.first == -1 || step2.second == -1) return {-1, -1};
			return reflect_over_xy(step2.second, step2.first, 3, 3);
		}
		
		if(!t.first) {
			// centralna simetrija od a2
			auto step1 = reflect_over_xy(x, y, 3, 3);
			auto step2 = f_inv(a2[f(step1.first, step1.second)]);
			if(step2.first == -1 || step2.second == -1) return {-1, -1};
			return reflect_over_xy(step2.second, step2.first, 3, 3);
		}
		
		if(!t.second) {
			// centralna simetrija, pa rotacija slucaja a2
			auto step1 = reflect_over_xy(x, y, 3, 3);
			auto ret = f_inv(a2[f(step1.second, step1.first)]);
			if(ret.first == -1 || ret.second == -1) return {-1, -1};
			// opet rotiramo, pa reflektujemo
			return reflect_over_xy(ret.first, ret.second, 3, 3);
		}
		
		if(t.first && t.second) {
			// centralna simetrija slucaja a3
			auto step1 = reflect_over_xy(x, y, 3, 3);
			auto step2 = f_inv(a3[f(step1.first, step1.second)]);
			if(step2.first == -1 || step2.second == -1) return {-1, -1};
			return reflect_over_xy(step2.second, step2.first, 3, 3);
		}
	}
	
	return {-2, -2};
}

std::pair<int, int> hamiltonian_path_util(int m, int n, int x, int y, std::pair<int, int>& s, std::pair<int, int>& t, peeling& r) {
	
	// ako postoji u M5 i tacka je u M5, idi u M5
	// inace specijalan slucaj...
	std::pair<int, int> s1 = {s.first - r.r1 - 1, s.second - r.r3 - 1};
	std::pair<int, int> t1 = {t.first - r.r1 - 1, t.second - r.r3 - 1};
	if(has_hamiltonian_path(s1, t1, m, n)) {
		// ako su x i y u M5, racunaj u M5
		if(x >= r.r3 + 1 && x <= r.r4 && y >= r.r1 + 1 && y <= r.r2) {
			int x1 = x - r.r3 - 1;
			int y1 = y - r.r1 - 1;
			auto ret = find_path_m5(r.r4 - r.r3, r.r2 - r.r1, x1, y1, s1, t1);
			if(ret.first == -1 || ret.second == -1) return {-1, -1};
		}	
			
	} else {
	
	
	
	
	
	
	}
	
	return {-1, -1};
}

std::pair<int, int> find_hamiltonian_path(int m, int n, int x, int y, std::pair<int, int>& s, std::pair<int, int>& t) {
	// ako su s i t ista tacka, gledamo ima li ciklusa
	// ako ima, nalazimo ga
	if(s == t) {
		if(!has_hamiltonian_cycle(m, n)) return {-1, -1};
		return H_C_M1_M3_CCW(m, n, x, y);
	}
	
	// ako nema puta, batali
	if(!has_hamiltonian_path(s, t, m, n)) return {-1, -1};
	
	peeling r = peel(s, t, m, n);
	
	
	// util funkcija ce da obradi posebne slucajeve
	// recimo, korisno je da se kod ponovi ako se u
	// odredjenom slucaju menjaju granice za peeling
	// zato util funkcija prima peeling kao argument
	return hamiltonian_path_util(m, n, x, y, s, t, r);
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

	auto x = s;
	for(int i = 0; i < n*m; i++) {
		x = find_path_m5(m, n, x.second, x.first, s, t);
		std::cout << x << '\n';
	}


	return 0;
}
