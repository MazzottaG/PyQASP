input="""
list1([1,2,[5,6,7]]).
list1([[d,f,g]|[1,2,4]]).
list2([6,[7,[8]]]).
list2([10]).
member(1..10).
memberList(M):-member(M),list1(L),&member(M,L;).
memberList(M):-member(M),list2(L),&member(M,L;).
"""
output="""
{list1([1,2,[5,6,7]]), list1([[d,f,g],1,2,4]), list2([6,[7,[8]]]), list2([10]), member(1), member(2), member(3), member(4), member(5), member(6), member(7), member(8), member(9), member(10), memberList(1), memberList(2), memberList(4), memberList(6), memberList(10)}
"""
