input = """
produced(X) :- producedBy(X,Y), controlled(Y).
controlled(X) :- bought(X).
controlled(X) :- share(X,Y,N), controlled(Y), N>50.
controlled(X) :- share(X,Y,N), share(X,Z,M), controlled(Y), 
                 controlled(Z), T=M+N, T>50, Y!=Z.

producedBy(wine,barilla).
producedBy(pasta,barilla).
produceddBy(pasta,saiwa).
producedBy(beer,budweiser).
producedBy(beer,heineken).
producedBy(bread,saiwa).
producedBy(bread,panino).
producedBy(milk,parmalat).
producedBy(milk,candia).
producedBy(tomatoes,frutto).

share(budweiser,barilla,40).
share(budweiser,saiwa,20).
share(saiwa,heineken,60).
share(candia,saiwa,40).
share(parmalat,barilla,60).
share(frutto,barilla,15).
share(frutto,saiwa,20).
"""
output = """
{producedBy(beer,budweiser), producedBy(beer,heineken), producedBy(bread,panino), producedBy(bread,saiwa), producedBy(milk,candia), producedBy(milk,parmalat), producedBy(pasta,barilla), producedBy(tomatoes,frutto), producedBy(wine,barilla), produceddBy(pasta,saiwa), share(budweiser,barilla,40), share(budweiser,saiwa,20), share(candia,saiwa,40), share(frutto,barilla,15), share(frutto,saiwa,20), share(parmalat,barilla,60), share(saiwa,heineken,60)}
"""
