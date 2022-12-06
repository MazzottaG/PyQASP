input = """

 rule(r1). head(p,r1). nbl(q,r1). 

 rule(r2). head(q,r2). nbl(neg_q,r2).

 rule(r3). head(neg_p,r3). nbl(p,r3).

 rule(r4). head(p,r4). nbl(neg_p,r4).

 opp(p,neg_p). 
 opp(q,neg_q). 

 pr(r1,r2). pr(r2,r3). pr(r3,r4).


"""
output = """
{head(neg_p,r3), head(p,r1), head(p,r4), head(q,r2), nbl(neg_p,r4), nbl(neg_q,r2), nbl(p,r3), nbl(q,r1), opp(p,neg_p), opp(q,neg_q), pr(r1,r2), pr(r2,r3), pr(r3,r4), rule(r1), rule(r2), rule(r3), rule(r4)}
"""
