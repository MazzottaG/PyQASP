input = """
%@global_ordering(@value=0).
% instance

manAssignsScore(m_1,w_1,4). manAssignsScore(m_1,w_2,2). manAssignsScore(m_1,w_3,2). manAssignsScore(m_1,w_4,1).
manAssignsScore(m_2,w_1,2). manAssignsScore(m_2,w_2,1). manAssignsScore(m_2,w_3,4). manAssignsScore(m_2,w_4,3).
manAssignsScore(m_3,w_1,1). manAssignsScore(m_3,w_2,3). manAssignsScore(m_3,w_3,2). manAssignsScore(m_3,w_4,2).
manAssignsScore(m_4,w_1,2). manAssignsScore(m_4,w_2,3). manAssignsScore(m_4,w_3,4). manAssignsScore(m_4,w_4,1).

womanAssignsScore(w_1,m_1,3). womanAssignsScore(w_1,m_2,4). womanAssignsScore(w_1,m_3,2). womanAssignsScore(w_1,m_4,1).
womanAssignsScore(w_2,m_1,1). womanAssignsScore(w_2,m_2,4). womanAssignsScore(w_2,m_3,3). womanAssignsScore(w_2,m_4,2).
womanAssignsScore(w_3,m_1,4). womanAssignsScore(w_3,m_2,2). womanAssignsScore(w_3,m_3,3). womanAssignsScore(w_3,m_4,1).
womanAssignsScore(w_4,m_1,3). womanAssignsScore(w_4,m_2,2). womanAssignsScore(w_4,m_3,4). womanAssignsScore(w_4,m_4,1).

% encoding
% guess matching
%@rule_ordering(@value=1).
match(M,W) :- manAssignsScore(M,_,_), womanAssignsScore(W,_,_), not nonMatch(M,W).
nonMatch(M,W) :- manAssignsScore(M,_,_), womanAssignsScore(W,_,_), not match(M,W).

% no polygamy

:- match(M1,W), match(M,W), M <> M1.
:- match(M,W), match(M,W1), W <> W1.

% no singles
jailed(M) :- match(M,_).
:- manAssignsScore(M,_,_), not jailed(M).

% strong stability condition
%@rule_partial_order(@after={manAssignsScore(M,W,Smw)},@before={womanAssignsScore(W,M1,Swm1)}).
:- match(M,W1), manAssignsScore(M,W,Smw), W1 <> W, manAssignsScore(M,W1,Smw1),   Smw >  Smw1,
   match(M1,W), womanAssignsScore(W,M,Swm),        womanAssignsScore(W,M1,Swm1), Swm >= Swm1.

"""
output = """
{jailed(m_1), jailed(m_2), jailed(m_3), jailed(m_4), manAssignsScore(m_1,w_1,4), manAssignsScore(m_1,w_2,2), manAssignsScore(m_1,w_3,2), manAssignsScore(m_1,w_4,1), manAssignsScore(m_2,w_1,2), manAssignsScore(m_2,w_2,1), manAssignsScore(m_2,w_3,4), manAssignsScore(m_2,w_4,3), manAssignsScore(m_3,w_1,1), manAssignsScore(m_3,w_2,3), manAssignsScore(m_3,w_3,2), manAssignsScore(m_3,w_4,2), manAssignsScore(m_4,w_1,2), manAssignsScore(m_4,w_2,3), manAssignsScore(m_4,w_3,4), manAssignsScore(m_4,w_4,1), match(m_1,w_1), match(m_2,w_3), match(m_3,w_2), match(m_4,w_4), nonMatch(m_1,w_2), nonMatch(m_1,w_3), nonMatch(m_1,w_4), nonMatch(m_2,w_1), nonMatch(m_2,w_2), nonMatch(m_2,w_4), nonMatch(m_3,w_1), nonMatch(m_3,w_3), nonMatch(m_3,w_4), nonMatch(m_4,w_1), nonMatch(m_4,w_2), nonMatch(m_4,w_3), womanAssignsScore(w_1,m_1,3), womanAssignsScore(w_1,m_2,4), womanAssignsScore(w_1,m_3,2), womanAssignsScore(w_1,m_4,1), womanAssignsScore(w_2,m_1,1), womanAssignsScore(w_2,m_2,4), womanAssignsScore(w_2,m_3,3), womanAssignsScore(w_2,m_4,2), womanAssignsScore(w_3,m_1,4), womanAssignsScore(w_3,m_2,2), womanAssignsScore(w_3,m_3,3), womanAssignsScore(w_3,m_4,1), womanAssignsScore(w_4,m_1,3), womanAssignsScore(w_4,m_2,2), womanAssignsScore(w_4,m_3,4), womanAssignsScore(w_4,m_4,1)}
"""
