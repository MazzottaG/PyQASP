input = """
% idlv_1.1.4_linux_x86-64.3_linux_x86-64

n(1..3).
max_n(M) :- M = #max{ N : n(N) }.
score(0, 0). %score(POS, VAL).

% errore:
% nello scrivere il seguente predicato è statp riscontrato il seguente errore:
% 	idlv: src/grounder/table/../atom/../table/HistoryStructure.h:106: 
% 			std::pair<unsigned int, unsigned int> DLV2::grounder::HistoryVector<T>::getElements(DLV2::grounder::SearchType, unsigned int) 
% 			[with T = DLV2::grounder::Atom*]: Assertion `current_iteration==iteration' failed.
% l'errore, causato da #max{ X : score(X,_) }

%score(P + 1, S) :- P = #max{ X : score(X,_) }, score(P, Y), S = Y + 1, P < MAX, max_n(MAX).


%---------------------


% stranezza(?):
% nel cercare di capire quale fosse il problema che generasse l'errore decritto sopra
% è stato generato un fatto diverso da quelli conosciuti da me
% il fatto ottenuto attraverso il seguente predicato è: score((0,1),1).

score(P + 1, S) :- P = 0, score(P, Y), S = Y + 1, P < MAX, max_n(MAX).

% è stato quindi replicato il comportamento con un predicato più generico che ha dato come risultato:

a(X - 1) :- X = 0. % output: a((0,1)).
b(X + 1) :- X = 1. % output: b((1,1)).
c(X * 2) :- X = 2. % output: c((2,2)).
d(X / 5) :- X = 10. % output: d((10,5)).



% se la costante viene presa da un fatto l'output è quello aspettato:

k(4).
w(Y + 1) :- k(Y). % output: w(5).
x(Y - 1) :- k(Y). % output: x(3).
y(Y * 2) :- k(Y). % output: y(8).
z(Y / 2) :- k(Y). % output: z(2).
"""
output = """
{n(1), n(2), n(3), score(0,0), k(4), x(3), z(2), y(8), a(-1), d(2), b(2), c(4), max_n(3), score(1,1), w(5)}
"""
