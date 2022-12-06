input = """

 rule(r1). head(a,r1). nbl(b,r1). 

 rule(r2). head(c,r2).

 rule(r3). head(b,r3).

 pr(r1,r2). pr(r2,r3). 


"""
output = """
{head(a,r1), head(b,r3), head(c,r2), nbl(b,r1), pr(r1,r2), pr(r2,r3), rule(r1), rule(r2), rule(r3)}
"""
