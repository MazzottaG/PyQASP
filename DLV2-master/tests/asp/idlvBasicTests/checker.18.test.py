input = """
% Synthetic example constructed to trigger a bug in the quick model check
% (with -OH- set, to control choices).

:- not ta2.
:- not ta3.
ta1. 

cHOICE1 | ta3. 
cHOICE2 | fa1 :- ta3, cHOICE1. 
cHOICE1 | cHOICE3 :- ta2. 

ta2 | ta1 :- ta3. 
ta3 | fa3 :- ta3, ta1,  not cHOICE3. 
ta2 | fa2 . 
ta1 | ta3. 

"""
output = """
{cHOICE3, ta1, ta2, ta3}
"""
