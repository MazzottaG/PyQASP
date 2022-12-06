input = """
ouch11 :- "2" < "1".
ouch21 :- "2" <= "1".
ouch31 :- "1" > "2".
ouch41 :- "1" >= "2".

okay11 :- "1" < "2".
okay21 :- "1" <= "2".
okay31 :- "2" > "1".
okay41 :- "2" >= "1".

ouch12 :- beta < alpha.
ouch22 :- beta <= alpha.
ouch32 :- alpha > beta.
ouch42 :- alpha >= beta.

okay12 :- alpha < beta.
okay22 :- alpha <= beta.
okay32 :- beta > alpha.
okay42 :- beta >= alpha.
"""
output = """
{okay11, okay12, okay21, okay22, okay31, okay32, okay41, okay42}
"""
