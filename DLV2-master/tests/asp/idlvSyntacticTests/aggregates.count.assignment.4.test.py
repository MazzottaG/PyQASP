input = """
% The variables to be aggregated over must appear only
% in the aggregate conjunction, while here they appear
% as guard, too.

ouch :- #count{Y : a(Y)} = Y.
ok :- #count{Y : a(Y)} = X.
"""
output = """
"""
