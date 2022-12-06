input = """
up(L,0) | -up(L,0) :- latch(L).

latch(a).
latch(b).
latch(c).
"""
output = """
{-up(b,0), -up(c,0), latch(a), latch(b), latch(c), up(a,0)}
{-up(c,0), latch(a), latch(b), latch(c), up(a,0), up(b,0)}
{-up(b,0), latch(a), latch(b), latch(c), up(a,0), up(c,0)}
{latch(a), latch(b), latch(c), up(a,0), up(b,0), up(c,0)}
{-up(a,0), -up(b,0), -up(c,0), latch(a), latch(b), latch(c)}
{-up(a,0), -up(b,0), latch(a), latch(b), latch(c), up(c,0)}
{-up(a,0), -up(c,0), latch(a), latch(b), latch(c), up(b,0)}
{-up(a,0), latch(a), latch(b), latch(c), up(b,0), up(c,0)}
"""
