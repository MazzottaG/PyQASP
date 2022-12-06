input = """
% Input: specify parts of the rules ...

 rule(r1). head(peng,r1). 

 rule(r2). head(bird,r2).

 rule(r3). head(neg_flies,r3). pbl(peng,r3). nbl(flies,r3).

 rule(r4). head(flies,r4).  pbl(bird,r4). nbl(neg_flies,r4).

 opp(flies,neg_flies). 

 pr(r1,r2). pr(r2,r3). pr(r3,r4).


"""
output = """
{head(bird,r2), head(flies,r4), head(neg_flies,r3), head(peng,r1), nbl(flies,r3), nbl(neg_flies,r4), opp(flies,neg_flies), pbl(bird,r4), pbl(peng,r3), pr(r1,r2), pr(r2,r3), pr(r3,r4), rule(r1), rule(r2), rule(r3), rule(r4)}
"""
