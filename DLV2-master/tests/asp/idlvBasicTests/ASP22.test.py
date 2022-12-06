input = """
% instance

node(1). node(2). node(3). 
link(1,2). link(2,1). link(2,3). 
link(3,2). link(3,1). link(1,3). 
colour(red). colour(green). colour(blue).

% encoding

% Guess colours.
chosenColour(N,C) | notChosenColour(N,C) :- node(N), colour(C).

% At least one color per node.
:- node(X), not colored(X).
colored(X) :- chosenColour(X,Fv1).

% Only one color per node.
:- chosenColour(N,C1), chosenColour(N,C2), C1!=C2. 

% No two adjacent nodes have the same colour. 
:- link(X,Y),  X<Y, chosenColour(X,C), chosenColour(Y,C).


"""
output = """
{chosenColour(1,blue), chosenColour(2,green), chosenColour(3,red), colored(1), colored(2), colored(3), colour(blue), colour(green), colour(red), link(1,2), link(1,3), link(2,1), link(2,3), link(3,1), link(3,2), node(1), node(2), node(3), notChosenColour(1,green), notChosenColour(1,red), notChosenColour(2,blue), notChosenColour(2,red), notChosenColour(3,blue), notChosenColour(3,green)}
{chosenColour(1,green), chosenColour(2,blue), chosenColour(3,red), colored(1), colored(2), colored(3), colour(blue), colour(green), colour(red), link(1,2), link(1,3), link(2,1), link(2,3), link(3,1), link(3,2), node(1), node(2), node(3), notChosenColour(1,blue), notChosenColour(1,red), notChosenColour(2,green), notChosenColour(2,red), notChosenColour(3,blue), notChosenColour(3,green)}
{chosenColour(1,blue), chosenColour(2,red), chosenColour(3,green), colored(1), colored(2), colored(3), colour(blue), colour(green), colour(red), link(1,2), link(1,3), link(2,1), link(2,3), link(3,1), link(3,2), node(1), node(2), node(3), notChosenColour(1,green), notChosenColour(1,red), notChosenColour(2,blue), notChosenColour(2,green), notChosenColour(3,blue), notChosenColour(3,red)}
{chosenColour(1,green), chosenColour(2,red), chosenColour(3,blue), colored(1), colored(2), colored(3), colour(blue), colour(green), colour(red), link(1,2), link(1,3), link(2,1), link(2,3), link(3,1), link(3,2), node(1), node(2), node(3), notChosenColour(1,blue), notChosenColour(1,red), notChosenColour(2,blue), notChosenColour(2,green), notChosenColour(3,green), notChosenColour(3,red)}
{chosenColour(1,red), chosenColour(2,green), chosenColour(3,blue), colored(1), colored(2), colored(3), colour(blue), colour(green), colour(red), link(1,2), link(1,3), link(2,1), link(2,3), link(3,1), link(3,2), node(1), node(2), node(3), notChosenColour(1,blue), notChosenColour(1,green), notChosenColour(2,blue), notChosenColour(2,red), notChosenColour(3,green), notChosenColour(3,red)}
{chosenColour(1,red), chosenColour(2,blue), chosenColour(3,green), colored(1), colored(2), colored(3), colour(blue), colour(green), colour(red), link(1,2), link(1,3), link(2,1), link(2,3), link(3,1), link(3,2), node(1), node(2), node(3), notChosenColour(1,blue), notChosenColour(1,green), notChosenColour(2,green), notChosenColour(2,red), notChosenColour(3,blue), notChosenColour(3,red)}
"""
