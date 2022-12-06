input = """
win( X ) :- move( X, Y ), not win( Y ).

move( a, d ).
move( b, c ).
move( c, b ).

"""
output = """
{move(a,d), move(b,c), move(c,b), win(a), win(b)}
{move(a,d), move(b,c), move(c,b), win(a), win(c)}
"""
