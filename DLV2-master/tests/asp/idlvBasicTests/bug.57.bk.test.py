input = """
block(a).
block(b).
block(c).
location(B) :- block(B).
location(table).
"""
output = """
{block(a), block(b), block(c), location(a), location(b), location(c), location(table)}
"""
