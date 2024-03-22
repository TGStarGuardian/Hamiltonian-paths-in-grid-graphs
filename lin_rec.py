from numpy.polynomial import polynomial
import numpy as np

# p1 is of deg n
# p2 is of deg at most 2n-1
# return value: coefficients from x^n up to x^(2n-1) in p1*p2
def middle_product(p1, p2):
	n = len(p1) - 1
	R = np.array([0 for _ in range(n)])
	for i in range(n + 1):
		for j in range(n - i, min(n << 1, len(p2))):
			if i + j >= n << 1:
				break
			else:
				R[i + j - n] += p1[i]*p2[j]
	return R

class LinearRecurrence:
	def __init__(self, equation):
		# a polynomial describing the equation Fn = a0 F0 + ... + an-1 Fn-1
		# in the form [-a0, -a1, ..., -an-1, 1]
		# which is equivalent to -a0 F0 - ... - an-1 Fn-1 + Fn = 0
		self.equation = equation
		self.order = len(equation) - 1
		self.generatingFunction = self.GeneratingFunction(equation)
		
	class GeneratingFunction:
		def __init__(self, equation):
			self.denominator = np.array(list(reversed(equation)))
		def numerator(self, initialValues):
			return polynomial.polymul(self.denominator, initialValues)[:len(self.denominator) - 1:]
			
	def oneCoeff(self, numerator, denominator, N):
		while N:
			T = denominator.copy()
			for i in range(1, len(denominator), 2):
				T[i] *= -1
			U = polynomial.polymul(numerator, T)
			numerator = U[1::2] if N & 1 else U[0::2]
			A = polynomial.polymul(denominator, T)
			denominator = np.array([A[2*i] for i in range(self.order + 1)])
			N >>= 1
		return numerator[0]/denominator[0]
	def oneTerm(self, initialValues, N):
		return self.oneCoeff(self.generatingFunction.numerator(initialValues), self.generatingFunction.denominator, N)
	
	# code that computes the power series expansion of 1/denominator from x^(N - order + 1) up to x^(N)
	def sliceCoeff(self, denominator, N):
		if not N:
			R = np.array([0 for _ in range(self.order)])
			R[-1] = 1/denominator[0]
			return R
		T = denominator.copy()
		for i in range(1, len(denominator), 2):
			T[i] *= -1
		A = polynomial.polymul(denominator, T)
		V = A[0::2]
		W = self.sliceCoeff(V, N >> 1)
		S = np.array([0 for _ in range(((len(W) - 1) << 1) + 1)])
		for i in range(len(W)):
			S[i << 1] = W[i]
		if not (N & 1):
			S = polynomial.polymulx(S)
		
		return middle_product(T, S)
		B = polynomial.polymul(T, S)
		return np.array([B[self.order + i] for i in range(self.order)])
	
	# code that computes the N-th term of k sequences that satisfy the same equation
	# but may have different initial values
	def vectorNTerm(self, initialValueMatrix, N):
		denominator = self.generatingFunction.denominator
		numerator = [self.generatingFunction.numerator(initialValues) for initialValues in initialValueMatrix]
		U = np.array(list(reversed(self.sliceCoeff(self.generatingFunction.denominator, N))))
		R = np.array([0 for _ in range(len(numerator))])
		for i in range(len(initialValueMatrix)):
			R[i] = sum(map(lambda x, y : x * y, numerator[i], U))
		return R
	
	# computes x^N mod C(x), where C is the characteristic polynomial of the recurrence
	def newModExp(self, N):
		denominator = self.generatingFunction.denominator
		U = self.sliceCoeff(denominator, N)
		return np.array(list(reversed(polynomial.polymul(U, denominator)[:self.order:])))
	
	# computes terms from [F_d+1, ..., F_2d+1], where d is the order of the equation
	def initialSlice(self, initialValues):
		denominator = self.generatingFunction.denominator
		T = -polynomial.polymul(denominator, initialValues)[self.order::]
		H = self.sliceCoeff(denominator, self.order - 1)
		V = polynomial.polymul(H, T)[:self.order:]
		return V
	
	# computes a slice of terms [F_N, ... , F_N+d-1]
	def newFiducciaNTerm(self, initialValues, N):
		R = self.newModExp(N)
		U = np.concatenate((initialValues, self.initialSlice(initialValues)), axis = None)[:(self.order << 2) - 1:]
		# return middle_product(polynomial.polymulx(list(reversed(R))), U)
		V = polynomial.polymul(U, polynomial.polymulx(list(reversed(R))))
		return V[self.order:self.order << 1:]
		
Fibonacci = LinearRecurrence([-1, -1, 1])
print(Fibonacci.oneTerm([0, 1], int(input())))
print(Fibonacci.vectorNTerm([[0, 1], [0, 1], [1, 1], [2, 2]], 5))
print(Fibonacci.newModExp(int(input())))
print(Fibonacci.newFiducciaNTerm([0, 1], int(input())))
print(Fibonacci.sliceCoeff(Fibonacci.generatingFunction.denominator, 1))
print(Fibonacci.initialSlice([0, 1]))

Tribonacci = LinearRecurrence([-1, -1, -1, 1])
print(Tribonacci.newFiducciaNTerm([0, 0, 1], int(input())))





