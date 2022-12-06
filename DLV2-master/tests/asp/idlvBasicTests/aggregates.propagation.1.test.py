input = """
% This test describes a bug found in the Model Generator.
% After the first MG run the MG counters were correctly restored,
% but the aggregate's lower, and upper bounds were not.
% This happened because after the first MG run, MODEL_GENERATOR::Stack
% was cleared without popping the stored items first.

a(1) | a(2).

:- not #count{X : a(X)} = 1.

:- a(X).
"""
output = """
INCOHERENT
"""
