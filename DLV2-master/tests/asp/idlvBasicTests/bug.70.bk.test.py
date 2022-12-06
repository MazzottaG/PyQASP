input = """
% Locations:
loc(dropOff).
loc(distCenter).
loc(truck).
%loc(dest).

% Packages:
pkg(pidVar).
"""
output = """
{loc(distCenter), loc(dropOff), loc(truck), pkg(pidVar)}
"""
