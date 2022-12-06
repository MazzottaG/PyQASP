input="""
list1([1,2,[5,6,7]]).
list1([[d,f,g]|[1,2,4]]).
list2([6,[7,[8]]]).
list2([10]).
append(X,Y,R):-list1(X),list2(Y),&append(X,Y;R).


"""
output="""
{list1([1,2,[5,6,7]]), list1([[d,f,g],1,2,4]), list2([6,[7,[8]]]), list2([10]), append([1,2,[5,6,7]],[6,[7,[8]]],[1,2,[5,6,7],6,[7,[8]]]), append([1,2,[5,6,7]],[10],[1,2,[5,6,7],10]), append([[d,f,g],1,2,4],[6,[7,[8]]],[[d,f,g],1,2,4,6,[7,[8]]]), append([[d,f,g],1,2,4],[10],[[d,f,g],1,2,4,10])}
"""
