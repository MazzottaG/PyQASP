input = """
win( X ) :- move( X, Y ), not win( Y ).

move( a, d ).
move( b, c ).

"""
output = """
{move(a,d), move(b,c), win(a), win(b)}
"""
