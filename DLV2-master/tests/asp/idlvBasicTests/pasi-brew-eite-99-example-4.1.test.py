input = """

 rule(r1). head(a,r1). nbl(b,r1). 

 rule(r2). head(b,r2). nbl(a,r2).

 pr(r1,r2).


"""
output = """
{head(a,r1), head(b,r2), nbl(a,r2), nbl(b,r1), pr(r1,r2), rule(r1), rule(r2)}
"""
