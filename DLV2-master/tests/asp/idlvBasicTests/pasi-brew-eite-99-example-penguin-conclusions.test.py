input = """
% Input: specify parts of the rules ...


 rule(r1). head(bird,r1).

 rule(r2). head(swims,r1). 

 rule(r3). head(neg_flies,r3). pbl(peng,r3). nbl(flies,r3).

 rule(r4). head(flies,r4).  pbl(bird,r4). nbl(neg_flies,r4).

 rule(r5). head(peng,r5). pbl(bird,r5). pbl(swims,r5). nbl(neg_peng,r5).

 opp(flies,neg_flies). 
 opp(peng,neg_peng). 

 pr(r3,r4).


"""
output = """
{head(bird,r1), head(flies,r4), head(neg_flies,r3), head(peng,r5), head(swims,r1), nbl(flies,r3), nbl(neg_flies,r4), nbl(neg_peng,r5), opp(flies,neg_flies), opp(peng,neg_peng), pbl(bird,r4), pbl(bird,r5), pbl(peng,r3), pbl(swims,r5), pr(r3,r4), rule(r1), rule(r2), rule(r3), rule(r4), rule(r5)}
"""
