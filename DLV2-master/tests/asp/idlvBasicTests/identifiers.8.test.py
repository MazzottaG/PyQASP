input = """
% Strings starting with "aux" may occur in regular input, even if (in some)
% versions of DLV/proposed patches they can occur as a result of internal
% predicate names as well.

aux | aux1(1) | auxiliary(1,2).
"""
output = """
{aux}
{auxiliary(1,2)}
{aux1(1)}
"""
