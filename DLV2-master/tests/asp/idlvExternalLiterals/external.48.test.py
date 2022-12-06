input="""
list1([1,2,[5,6,7]]).
list1([[d,f,g]|[1,2,4]]).
list2([6,[7,[8]]]).
list2([10]).
memberList(L):-list2(L),&member([7,[8]],L;).
member:-&member(10,[10,7,9];).
memberList(L):-list2(L),&member(L,[[10]];).
"""
output="""
{list1([1,2,[5,6,7]]), list1([[d,f,g],1,2,4]), list2([6,[7,[8]]]), list2([10]), member, memberList([10]), memberList([6,[7,[8]]])}
"""
