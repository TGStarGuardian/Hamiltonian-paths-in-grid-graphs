The repository consists of the following files:
* lin_rec.py - deals with algorithms regarding linear recurrent sequences
* fibonacci.py - contains two algorithms to compute the N-th term of the Fibonacci sequence (with F0 = 0, F1 = 1).

---------------------------------------------
lin_rec.py

The code consists of LinearRecurrence class.
The class contains the following:
* nested classes:
  - GeneratingFunction:
    + self.denominator - the denominator of the generating function
    + self.numerator(initialValues) - returns the numerator of the generating function of the linear recurrent sequence satisfying equation with initialValues as initial values
* fields:
  - self.equation - an array representing the characteristic polynomial of the linear recurrent equation in the form x^N - aN-1 x^N-1 - ... - a0 x^0 for the equation F_N = a0 F0 + ... + a_N-1 + F_N-1. The N-th index is the coefficient of x^N.
  - self.order - an integer representing the order of the equation, i.e. the degree of the polynomial
  - self.generatingFunction - a GeneratingFunction object representing the generating function of the given recurrence
* methods:
  - self.oneTerm(initialValues, N) - computes the N-th term of the sequence satisfying self.equation with initialValues as initial values
  - self.oneCoeff(numerator, denominator, N) - a helper function for oneTerm
  - self.sliceCoeff(denominator, N) - computes a slice of coefficients from the term x^(N + d - 1) up to the term x^N in the power series of 1/denominator(x)
  - self.vectorNTerm(initialValueMatrix, N) - computes the N-th term of multiple sequences satisfying the equation given in self.equation, with their initial values stored as rows in a 2D array initialValueMatrix
  - self.newModExp(N) - computes x^N mod C(x), where C(x) is the characteristic polynomial of the equation.
  - self.initialSlice(initialValues) - computes an array of terms from F_self.order+1 up to F_2*self.order+1
  - self.newFiducciaNTerm(initialValues, N) - uses Fidducia style algorithm to compute a slice of terms from F_N up to F_N+self.order-1
  
-----------------------------------------------
fibonacci.py

The code consists of two functions:
* fibonacci(N) - computes the N-th term of the Fibonacci sequence, with N being any non-negative integer
* fibonacci_power_of_2(N) computes the N-th term of the Fibonacci sequence, when N is a power of 2

-----------------------------------------------
Examples

Fibonacci = LinearRecurrence([-1, -1, 1])
print(Fibonacci.oneTerm([0, 1], 5)
>> 5
print(Fibonacci.vectorNTerm([[0, 1], [0, 1], [1, 1], [2, 2]], 5))
>> [5 5 8 16]
print(Fibonacci.newModExp(5))
>> [3, 5] ///// x^5 mod (x^2 - x - 1) = 3 + 5x
print(Fibonacci.newFiducciaNTerm([0, 1], 5)
>> [5, 8] ///// F5 = 5, F6 = 8
print(Fibonacci.sliceCoeff(Fibonacci.generatingFunction.denominator, 5))
>> [5, 8] ///// i.e. 5 x^4 + 8 x^5 in the power series of 1/(1 - x - x^2)
print(Fibonacci.initialSlice([0, 1]))
>> [1, 2] ///// the sequence goes 0, 1, 1, 2

