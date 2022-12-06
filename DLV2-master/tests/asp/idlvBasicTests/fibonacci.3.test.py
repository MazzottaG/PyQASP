input = """
fib0(1,1).
fib0(2,1).

fib(N,X) :- fib0(N,X).
fib(N,X) :- fib(N1,Y1), Y1=Y2+X, fib(N2,Y2), N2=2+N, N1=1+N, N<1, X<1. %+(Y1,Y2,X), fib(N2,Y2), +(N2,2,N), +(N1,1,N).
"""
output = """
{fib0(1,1), fib0(2,1), fib(1,1), fib(2,1), fib(0,0), fib(-1,-1), fib(-2,-1), fib(-3,0)}
"""
