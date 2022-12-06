input = """
i(0). i(1).
a(X) | not_a(X) :- i(X).
ok :- 0 < #count{X:a(X)}< 2.
:- not ok.
"""
output = """
{a(1), i(0), i(1), not_a(0), ok}
{a(0), i(0), i(1), not_a(1), ok}
"""
