input = """

 rule(r1). head(c,r1). nbl(b,r1). 

 rule(r2). head(b,r2). nbl(a,r2).

 pr(r1,r2).


"""
output = """
{head(b,r2), head(c,r1), nbl(a,r2), nbl(b,r1), pr(r1,r2), rule(r1), rule(r2)}
"""
