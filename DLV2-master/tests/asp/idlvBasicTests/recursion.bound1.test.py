input = """
equal(N) :- target(P,N), true(P), equal(N+1).

target(1,1..5)|d.
true(1..3).
equal(5).
"""
output = """
{equal(1), equal(2), equal(3), equal(4), equal(5), target(1,1), target(1,2), target(1,3), target(1,4), target(1,5), true(1), true(2), true(3)}
{d, equal(5), true(1), true(2), true(3)}
"""
