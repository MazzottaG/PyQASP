input="""
list1([1,2,[5,6,7]]).
list1([[d,f,g]|[1,2,4]]).
list2([6,[7,[8]]]).
list2([10]).
append(R):-list2(Y),&append([1,2],Y;R).
append(R):-list1(X),&append(X,[1,2];R).
append2(X,Y):-list1(X),list2(Y),&append(X,Y;[1,2,[5,6,7],10]).
"""
output="""
{append([1,2,10]), append([1,2,6,[7,[8]]]), append([1,2,[5,6,7],1,2]), append([[d,f,g],1,2,4,1,2]), append2([1,2,[5,6,7]],[10]), list1([1,2,[5,6,7]]), list1([[d,f,g],1,2,4]), list2([10]), list2([6,[7,[8]]])}
"""
