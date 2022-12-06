input = """
p(a,1,2,1).
p(a,2,1,1).
p(b,1,2,2).
p(b,3,1,2).

shouldbe2(Z) :- B=1, #count{A:p(A,B,C,D)}=Z.
shouldbe2(Z) :- C=1, #count{A:p(A,B,C,D)}=Z.
shouldbe2(Z) :- C=2, #count{A:p(A,B,C,D)}=Z.
shouldbe2(Z) :- A=a, #count{B:p(A,B,C,D)}=Z.
shouldbe2(Z) :- A=b, #count{B:p(A,B,C,D)}=Z.
shouldbe2(Z) :- A=a, #count{C:p(A,B,C,D)}=Z.
shouldbe2(Z) :- A=b, #count{C:p(A,B,C,D)}=Z.

shouldbe1(Z) :- D=1, #count{A:p(A,B,C,D)}=Z.
shouldbe1(Z) :- D=2, #count{A:p(A,B,C,D)}=Z.
shouldbe1(Z) :- A=a, #count{D:p(A,B,C,D)}=Z.
shouldbe1(Z) :- A=b, #count{D:p(A,B,C,D)}=Z.
shouldbe1(Z) :- B=2, #count{A:p(A,B,C,D)}=Z.

shouldbe4(Z) :- B=1, #sum{C,D:p(A,B,C,D)}=Z.
shouldbe4(Z) :- A=b, #sum{B:p(A,B,C,D)}=Z.

shouldbe5(Z) :- C=1, #sum{B:p(A,B,C,D)}=Z.
"""
output = """
{p(a,1,2,1), p(a,2,1,1), p(b,1,2,2), p(b,3,1,2), shouldbe1(1), shouldbe2(2), shouldbe4(4), shouldbe5(5)}
"""
