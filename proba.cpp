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

std::pair<int, int> find_path_m5(int m, int n, int x, int y, std::pair<int, int>& s, std::pair<int, int>& t) {
	// za n >= 4, radimo po horizontali
	// za n < 4, radimo po vertikali
	if(n >= 4) return horizontal_trisection(m, n, x, y, s, t);
	else return vertical_trisection(m, n, x, y, s, t);
	
	// treba dodati i ostale slucajeve
	// to cemo posle

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
/*
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < m; j++) {
			std::cout << find_hamiltonian_path(m, n, j, i, s, t) << " ";
		}
		std::cout << '\n';
	}
	
	auto x = s;
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < m; j++) {
			std::cout <<  (x = find_hamiltonian_path(m, n, x.second, x.first, s, t)) << '\n';
		}
	}
	
	*/
	
	std::cout << is_corner_vertex(m - 1, m) << '\n';
	std::cout << connected_by_non_boundary_edge(s, t, m, n) << '\n';
	std::cout << has_hamiltonian_path(s, t, m, n) << '\n';
	
	/*
	auto start = std::chrono::high_resolution_clock::now();
	auto x = s;
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < m; j++) {
			x = find_hamiltonian_path(m, n, x.second, x.first, s, t);
		}
	}
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << duration.count() << '\n';
	*/
	return 0;
}
