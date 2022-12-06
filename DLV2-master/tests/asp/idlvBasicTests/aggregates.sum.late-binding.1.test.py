input = """
expensive(a) | expensive(b).

cheap | cheaper :- expensive(X).

cost(1,9) :- expensive(X).   %%% [9:]
cost(2,2) :- cheap.          %%% [2:]
cost(3,1) :- cheaper.        %%% [1:]

totalCost(X) :- X = #sum{ Y,I: cost(I,Y) }.
		    
"""
output = """
{cheaper, cost(1,9), cost(3,1), expensive(b), totalCost(10)}
{cheap, cost(1,9), cost(2,2), expensive(b), totalCost(11)}
{cheaper, cost(1,9), cost(3,1), expensive(a), totalCost(10)}
{cheap, cost(1,9), cost(2,2), expensive(a), totalCost(11)}
"""
