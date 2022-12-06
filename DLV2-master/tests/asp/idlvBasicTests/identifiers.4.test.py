input = """
p(a). longPredicate(a).
q. quasiPredicate.

tRUE :- p(a), longPredicate(a), q, quasiPredicate.
"""
output = """
{longPredicate(a), p(a), q, quasiPredicate, tRUE}
"""
