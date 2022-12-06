input = """
% This is example shows the necessity of a recomputation of heuristic values
% if all PTs in the cache have become invalidated.
% The example works if the PT consideration order corresponds to the 
% appearance in the file, if the PT cache size is less or equal to the
% number of v*, and if the heuristic function favors PTs which directly
% derive MBTs.

% The v*s are rated good wrt the heuristic and take all the space in the
% PT cache. Then two PTs are considered which can derive the MBTs only
% together, so they are not rated well and do not go into the PT. Then
% another PT creates an inconsistency, and therefore its negation is
% propagated. This propagation determines all of the v*s, therefore the
% PT cache contains no valid PTs.
 
% x and y are must-be-true, their purpose is to assign good heuristic values
% for v*.
:- not y.
:- not x.

% v* are PTs which are later determined (and therefore no longer PT) by c.
v1 | nv1.
v2 | nv2.
v3 | nv3.
v4 | nv4.
v5 | nv5.

% They directly derive both mbts x and y and will get a good heuristic value.
% Same for the nv* (though that would not be necessary).
x :- v1. y :- v1.
x :- v2. y :- v2.
x :- v3. y :- v3.
x :- v4. y :- v4.
x :- v5. y :- v5.

% a and b's heuristic values will be rated lower than the ones of v*
% (since neither of them can derive x or y) and will not go into the cache
% if the number of v* is big enough.
% Same for na and nb.
a | na.
b | nb.

x :- a,b.
x :- na,nb.
y :- a,b.
y :- na,nb.

% This is the "determinator". PT c directly entails an inconsistency,
% therefore nc is derived.
c | nc.

bad1 :- c.
bad2 :- c.
:- bad1, bad2.

% nc is cloned to avoid the v*s affecting nc.
% However, nc affects the v*.
nc_clone :- nc.
:- v1, nc_clone, nc.
:- v2, nc_clone, nc.
:- v3, nc_clone, nc.
:- v4, nc_clone, nc.
:- v5, nc_clone, nc.

"""
output = """
{a, b, nc, nc_clone, nv1, nv2, nv3, nv4, nv5, x, y}
{na, nb, nc, nc_clone, nv1, nv2, nv3, nv4, nv5, x, y}
"""
