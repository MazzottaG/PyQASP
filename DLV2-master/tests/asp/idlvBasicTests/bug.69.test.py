input = """
% Simplified version of a program by Michael Fink, which segfaults
% with the Jun 11 2001 Release.

object(o1,4).
object(o2,5).
object(o3,17).

bid(1,14).
bid(2,34).
bid(3,14).

contains(1,o1,2).
contains(1,o2,4).
contains(1,o3,2).
contains(2,o3,10).
contains(3,o2,5).



select(B) | nselect(B) :- bid(B,V).
:- nselect(B).

assigned(B,I) :- bid(B,V), I!=0, not nassigned(B,I). %#int(I), !=(I,0), not nassigned(B,I).
nassigned(B,I) :- assigned(B,J), I!=J. %#int(I), !=(I,J).
nassigned(B,I) :- assigned(B1,I), select(B), B!=B1. %!=(B,B1).
assigned_bidId(I) :- assigned(B,I).
:- bid(B,V), not assigned_bidId(1).
:- assigned(B1,I), not assigned_bidId(J), J!=0. %#succ(J,I), !=(J,0), !=(J,#maxint).

%:- nselect(B),bid(B,V). [V:4]
%:- assigned(B,I), contains(B,O,N), #int(S), S=N+I. [S:1]

"""
output = """
"""
