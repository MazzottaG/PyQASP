input = """
% Guard parsing check is triggered correctly but it fails an assertion.

ouch(N) :- a = #count{X : p(X)}.
"""
output = """
"""
