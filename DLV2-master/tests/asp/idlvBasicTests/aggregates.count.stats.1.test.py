input = """
student(1,joe,1978). 
student(2,sue,1978).
student(3,tim,1978).
student(4,pat,1979).
student(5,may,1979).

yearOfBirth(Y) :-  student(S,_,Y).
bornIn(Y,N) :- yearOfBirth(Y), N = #count{S:student(S,_,Y)}. 
"""
output = """
{bornIn(1978,3), bornIn(1979,2), student(1,joe,1978), student(2,sue,1978), student(3,tim,1978), student(4,pat,1979), student(5,may,1979), yearOfBirth(1978), yearOfBirth(1979)}
"""
