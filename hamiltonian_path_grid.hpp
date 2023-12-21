#ifndef HAMILTONIAN_PATH_GRID_HPP
#define HAMILTONIAN_PATH_GRID_HPP

#include<utility>

bool has_hamiltonian_cycle(int width, int height);

bool has_hamiltonian_path(int m, int n, std::pair<int, int> s, std::pair<int, int> t);

bool has_hamiltonian_path_L(int m, int n, const std::pair<int, int>& s, const std::pair<int, int>& t);

bool has_hamiltonian_path_C(int m, int n, const std::pair<int, int>& s, const std::pair<int, int>& t);

bool has_hamiltonian_path_F(int m, int n, const std::pair<int, int>& s, const std::pair<int, int>& t);

bool has_hamiltonian_path_E(int m, int n, const std::pair<int, int>& s, const std::pair<int, int>& t);

std::pair<int, int> find_hamiltonian_path(int m, int n, int x, int y, const std::pair<int, int>& s, const std::pair<int, int>& t);

std::pair<int, int> find_hamiltonian_path_L(int m, int n, int x, int y, const std::pair<int, int>& s, const std::pair<int, int>& t);

std::pair<int, int> find_hamiltonian_path_C(int m, int n, int x, int y, const std::pair<int, int>& s, const std::pair<int, int>& t);

std::pair<int, int> find_hamiltonian_path_F(int m, int n, int x, int y, const std::pair<int,int>& s, const std::pair<int, int>& t);

std::pair<int, int> find_hamiltonian_path_E(int m, int n, int x, int y, const std::pair<int, int>& s, const std::pair<int, int>& t);


#endif
