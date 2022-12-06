input = """
fib0(1,1).
fib0(2,1).

fib(N,X) :- fib0(N,X).
fib(N,X) :- fib(N1,Y1), fib(N2,Y2), N2=2+N, N1=1+N, Y1=Y2+X, N<1, X<1. %+(N2,2,N), +(N1,1,N), +(Y1,Y2,X).
"""
output = """
{fib0(1,1), fib0(2,1), fib(1,1), fib(2,1), fib(0,0), fib(-1,-1), fib(-2,-1), fib(-3,0)}
"""
