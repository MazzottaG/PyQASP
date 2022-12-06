input = """
persona("persona221","ciccio",35,"M").
persona("persona231","pippo",45,"F").
persona(X0,X1,X2,X3):-bambino(X0,X1,X2,X3).
persona(X0,X1,X2,X3):-genitore(X0,X1,X2,X3).
figlioDi("persona221","persona231").
:-persona(_,_,_,_).
:-figlioDi(UX,UY),figlioDi(UY,UX).
madreAux(UX,UY):-figlioDi(UY,UX),persona(UX,_,_,"F").
padreAux(UX,UY):-figlioDi(UY,UX),persona(UX,_,_,"M").
"""
output = """
"""
