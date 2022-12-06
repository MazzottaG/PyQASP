input = """

 rule(r1). head(b,r1). pbl(a,r1). nbl(neg_b,r1). 

 rule(r2). head(c,r2). nbl(b,r2).

 rule(r3). head(a,r3). nbl(c,r3).

 opp(b,neg_b). 

 pr(r1,r2). pr(r2,r3).


"""
output = """
{head(a,r3), head(b,r1), head(c,r2), nbl(b,r2), nbl(c,r3), nbl(neg_b,r1), opp(b,neg_b), pbl(a,r1), pr(r1,r2), pr(r2,r3), rule(r1), rule(r2), rule(r3)}
"""
