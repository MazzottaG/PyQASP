input = """
fuelcost(10,a,b). fuelcost(10,b,a).

at(t0,a).
fuel(t0,56).
at(p0,a).
goal(p0,b).

step(1). step(2). step(3). step(4).
step(5). step(6). step(7). step(8).
step(9). step(10).

:- drive(t0,a,b,4).
:- drive(t0,a,b,5).
:- drive(t0,a,b,1).
:- drive(t0,b,a,6).
:- unload(p0,t0,b,3).
:- not unload(p0,t0,b,4).
:- not drive(t0,a,b,6).
:- not load(p0,t0,b,7).
:- not load(p0,t0,a,1).
:- not drive(t0,a,b,2).
:- not drive(t0,b,a,9).

truck(T) :- fuel(T,_).
package(P) :- at(P,L), not truck(P).
location(L) :- fuelcost(_,L,_).
location(L) :- fuelcost(_,_,L).
locatable(O) :- at(O,L).
at(O,L,0) :- at(O,L).
fuel(T,F,0) :- fuel(T,F).

1 <= { unload( P,T,L,S ) : 
        package( P ) , 
 	truck( T ) , 
	location( L ); 
    load( P,T,L,S ) : 
	package( P ) , 
	truck( T ) , 
	location( L ); 
    drive( T,L1,L2,S ) : 
	fuelcost( Fueldelta,L1,L2 ) , 
	truck( T );
    noop(S)
  } <= 1 :- step(S), S > 0.

at( P,L,S ) :- unload( P,T,L,S ).
del( in( P,T ),S ) :- unload( P,T,L,S ).

del( at( P,L ),S ) :- load( P,T,L,S ).
in( P,T,S ) :- load( P,T,L,S ).

del( at( T,L1 ), S ) :- drive( T,L1,L2,S ).
at( T,L2,S ) :- drive( T,L1,L2,S). 
del( fuel( T,Fuelpre ),S ) :- drive( T,L1,L2,S ), fuel(T, Fuelpre,S-1).
fuel( T,Fuelpost,S ) :- drive( T,L1,L2,S ), fuelcost(Fueldelta,L1,L2), fuel(T,Fuelpre,S-1), Fuelpost = Fuelpre - Fueldelta.

at( O,L,S ) :- at( O,L,S-1 ), not del( at( O,L ),S  ), step(S).
in( P,T,S ) :- in( P,T,S-1 ), not del( in( P,T ),S  ), step(S).
fuel( T,Level,S ) :- fuel( T,Level,S-1 ), not del( fuel( T,Level) ,S ), truck( T ), step(S).

:- unload( P,T,L,S ), not preconditions_u( P,T,L,S ).
preconditions_u( P,T,L,S ) :- step(S), at( T,L,S-1 ), in( P,T,S-1 ), package( P ), truck( T ).

 :- load( P,T,L,S ), not preconditions_l( P,T,L,S ).
preconditions_l( P,T,L,S ) :- step(S), at( T,L,S-1 ), at( P,L,S-1 ).

 :- drive( T,L1,L2,S ), not preconditions_d( T,L1,L2,S ).
preconditions_d( T,L1,L2,S ) :- step(S), at( T,L1,S-1 ), fuel( T, Fuelpre, S-1), fuelcost(Fueldelta,L1,L2), Fuelpre - Fueldelta >= 0.

goalreached :- step(S),  N = #count{ P,L,S : at(P,L,S) , goal(P,L) }, N = #count{ P,L : goal(P,L) }.
:- not goalreached.

"""
output = """
{at(p0,a), at(p0,a,0), at(p0,b,10), at(p0,b,4), at(p0,b,5), at(p0,b,6), at(p0,b,8), at(p0,b,9), at(t0,a), at(t0,a,0), at(t0,a,1), at(t0,a,10), at(t0,a,5), at(t0,a,9), at(t0,b,2), at(t0,b,3), at(t0,b,4), at(t0,b,6), at(t0,b,7), at(t0,b,8), del(at(p0,a),1), del(at(p0,b),7), del(at(t0,a),2), del(at(t0,a),6), del(at(t0,b),5), del(at(t0,b),9), del(fuel(t0,26),9), del(fuel(t0,36),6), del(fuel(t0,46),5), del(fuel(t0,56),2), del(in(p0,t0),4), del(in(p0,t0),8), drive(t0,a,b,2), drive(t0,a,b,6), drive(t0,b,a,5), drive(t0,b,a,9), fuel(t0,16,10), fuel(t0,16,9), fuel(t0,26,6), fuel(t0,26,7), fuel(t0,26,8), fuel(t0,36,5), fuel(t0,46,2), fuel(t0,46,3), fuel(t0,46,4), fuel(t0,56), fuel(t0,56,0), fuel(t0,56,1), fuelcost(10,a,b), fuelcost(10,b,a), goal(p0,b), goalreached, in(p0,t0,1), in(p0,t0,2), in(p0,t0,3), in(p0,t0,7), load(p0,t0,a,1), load(p0,t0,b,7), locatable(p0), locatable(t0), location(a), location(b), noop(10), noop(3), package(p0), preconditions_d(t0,a,b,1), preconditions_d(t0,a,b,10), preconditions_d(t0,a,b,2), preconditions_d(t0,a,b,6), preconditions_d(t0,b,a,3), preconditions_d(t0,b,a,4), preconditions_d(t0,b,a,5), preconditions_d(t0,b,a,7), preconditions_d(t0,b,a,8), preconditions_d(t0,b,a,9), preconditions_l(p0,p0,a,1), preconditions_l(p0,p0,b,10), preconditions_l(p0,p0,b,5), preconditions_l(p0,p0,b,6), preconditions_l(p0,p0,b,7), preconditions_l(p0,p0,b,9), preconditions_l(p0,t0,a,1), preconditions_l(p0,t0,b,5), preconditions_l(p0,t0,b,7), preconditions_l(p0,t0,b,9), preconditions_l(t0,p0,a,1), preconditions_l(t0,p0,b,5), preconditions_l(t0,p0,b,7), preconditions_l(t0,p0,b,9), preconditions_l(t0,t0,a,1), preconditions_l(t0,t0,a,10), preconditions_l(t0,t0,a,2), preconditions_l(t0,t0,a,6), preconditions_l(t0,t0,b,3), preconditions_l(t0,t0,b,4), preconditions_l(t0,t0,b,5), preconditions_l(t0,t0,b,7), preconditions_l(t0,t0,b,8), preconditions_l(t0,t0,b,9), preconditions_u(p0,t0,a,2), preconditions_u(p0,t0,b,3), preconditions_u(p0,t0,b,4), preconditions_u(p0,t0,b,8), step(1), step(10), step(2), step(3), step(4), step(5), step(6), step(7), step(8), step(9), truck(t0), unload(p0,t0,b,4), unload(p0,t0,b,8)}
{at(p0,a), at(p0,a,0), at(p0,b,4), at(p0,b,5), at(p0,b,6), at(t0,a), at(t0,a,0), at(t0,a,1), at(t0,a,10), at(t0,a,5), at(t0,a,9), at(t0,b,2), at(t0,b,3), at(t0,b,4), at(t0,b,6), at(t0,b,7), at(t0,b,8), del(at(p0,a),1), del(at(p0,b),7), del(at(t0,a),2), del(at(t0,a),6), del(at(t0,b),5), del(at(t0,b),9), del(fuel(t0,26),9), del(fuel(t0,36),6), del(fuel(t0,46),5), del(fuel(t0,56),2), del(in(p0,t0),4), drive(t0,a,b,2), drive(t0,a,b,6), drive(t0,b,a,5), drive(t0,b,a,9), fuel(t0,16,10), fuel(t0,16,9), fuel(t0,26,6), fuel(t0,26,7), fuel(t0,26,8), fuel(t0,36,5), fuel(t0,46,2), fuel(t0,46,3), fuel(t0,46,4), fuel(t0,56), fuel(t0,56,0), fuel(t0,56,1), fuelcost(10,a,b), fuelcost(10,b,a), goal(p0,b), goalreached, in(p0,t0,1), in(p0,t0,10), in(p0,t0,2), in(p0,t0,3), in(p0,t0,7), in(p0,t0,8), in(p0,t0,9), load(p0,t0,a,1), load(p0,t0,b,7), locatable(p0), locatable(t0), location(a), location(b), noop(10), noop(3), noop(8), package(p0), preconditions_d(t0,a,b,1), preconditions_d(t0,a,b,10), preconditions_d(t0,a,b,2), preconditions_d(t0,a,b,6), preconditions_d(t0,b,a,3), preconditions_d(t0,b,a,4), preconditions_d(t0,b,a,5), preconditions_d(t0,b,a,7), preconditions_d(t0,b,a,8), preconditions_d(t0,b,a,9), preconditions_l(p0,p0,a,1), preconditions_l(p0,p0,b,5), preconditions_l(p0,p0,b,6), preconditions_l(p0,p0,b,7), preconditions_l(p0,t0,a,1), preconditions_l(p0,t0,b,5), preconditions_l(p0,t0,b,7), preconditions_l(t0,p0,a,1), preconditions_l(t0,p0,b,5), preconditions_l(t0,p0,b,7), preconditions_l(t0,t0,a,1), preconditions_l(t0,t0,a,10), preconditions_l(t0,t0,a,2), preconditions_l(t0,t0,a,6), preconditions_l(t0,t0,b,3), preconditions_l(t0,t0,b,4), preconditions_l(t0,t0,b,5), preconditions_l(t0,t0,b,7), preconditions_l(t0,t0,b,8), preconditions_l(t0,t0,b,9), preconditions_u(p0,t0,a,10), preconditions_u(p0,t0,a,2), preconditions_u(p0,t0,b,3), preconditions_u(p0,t0,b,4), preconditions_u(p0,t0,b,8), preconditions_u(p0,t0,b,9), step(1), step(10), step(2), step(3), step(4), step(5), step(6), step(7), step(8), step(9), truck(t0), unload(p0,t0,b,4)}
{at(p0,a), at(p0,a,0), at(p0,a,10), at(p0,b,4), at(p0,b,5), at(p0,b,6), at(t0,a), at(t0,a,0), at(t0,a,1), at(t0,a,10), at(t0,a,5), at(t0,a,9), at(t0,b,2), at(t0,b,3), at(t0,b,4), at(t0,b,6), at(t0,b,7), at(t0,b,8), del(at(p0,a),1), del(at(p0,b),7), del(at(t0,a),2), del(at(t0,a),6), del(at(t0,b),5), del(at(t0,b),9), del(fuel(t0,26),9), del(fuel(t0,36),6), del(fuel(t0,46),5), del(fuel(t0,56),2), del(in(p0,t0),10), del(in(p0,t0),4), drive(t0,a,b,2), drive(t0,a,b,6), drive(t0,b,a,5), drive(t0,b,a,9), fuel(t0,16,10), fuel(t0,16,9), fuel(t0,26,6), fuel(t0,26,7), fuel(t0,26,8), fuel(t0,36,5), fuel(t0,46,2), fuel(t0,46,3), fuel(t0,46,4), fuel(t0,56), fuel(t0,56,0), fuel(t0,56,1), fuelcost(10,a,b), fuelcost(10,b,a), goal(p0,b), goalreached, in(p0,t0,1), in(p0,t0,2), in(p0,t0,3), in(p0,t0,7), in(p0,t0,8), in(p0,t0,9), load(p0,t0,a,1), load(p0,t0,b,7), locatable(p0), locatable(t0), location(a), location(b), noop(3), noop(8), package(p0), preconditions_d(t0,a,b,1), preconditions_d(t0,a,b,10), preconditions_d(t0,a,b,2), preconditions_d(t0,a,b,6), preconditions_d(t0,b,a,3), preconditions_d(t0,b,a,4), preconditions_d(t0,b,a,5), preconditions_d(t0,b,a,7), preconditions_d(t0,b,a,8), preconditions_d(t0,b,a,9), preconditions_l(p0,p0,a,1), preconditions_l(p0,p0,b,5), preconditions_l(p0,p0,b,6), preconditions_l(p0,p0,b,7), preconditions_l(p0,t0,a,1), preconditions_l(p0,t0,b,5), preconditions_l(p0,t0,b,7), preconditions_l(t0,p0,a,1), preconditions_l(t0,p0,b,5), preconditions_l(t0,p0,b,7), preconditions_l(t0,t0,a,1), preconditions_l(t0,t0,a,10), preconditions_l(t0,t0,a,2), preconditions_l(t0,t0,a,6), preconditions_l(t0,t0,b,3), preconditions_l(t0,t0,b,4), preconditions_l(t0,t0,b,5), preconditions_l(t0,t0,b,7), preconditions_l(t0,t0,b,8), preconditions_l(t0,t0,b,9), preconditions_u(p0,t0,a,10), preconditions_u(p0,t0,a,2), preconditions_u(p0,t0,b,3), preconditions_u(p0,t0,b,4), preconditions_u(p0,t0,b,8), preconditions_u(p0,t0,b,9), step(1), step(10), step(2), step(3), step(4), step(5), step(6), step(7), step(8), step(9), truck(t0), unload(p0,t0,a,10), unload(p0,t0,b,4)}
{at(p0,a), at(p0,a,0), at(p0,b,10), at(p0,b,4), at(p0,b,5), at(p0,b,6), at(p0,b,8), at(p0,b,9), at(t0,a), at(t0,a,0), at(t0,a,1), at(t0,a,5), at(t0,a,9), at(t0,b,10), at(t0,b,2), at(t0,b,3), at(t0,b,4), at(t0,b,6), at(t0,b,7), at(t0,b,8), del(at(p0,a),1), del(at(p0,b),7), del(at(t0,a),10), del(at(t0,a),2), del(at(t0,a),6), del(at(t0,b),5), del(at(t0,b),9), del(fuel(t0,16),10), del(fuel(t0,26),9), del(fuel(t0,36),6), del(fuel(t0,46),5), del(fuel(t0,56),2), del(in(p0,t0),4), del(in(p0,t0),8), drive(t0,a,b,10), drive(t0,a,b,2), drive(t0,a,b,6), drive(t0,b,a,5), drive(t0,b,a,9), fuel(t0,16,9), fuel(t0,26,6), fuel(t0,26,7), fuel(t0,26,8), fuel(t0,36,5), fuel(t0,46,2), fuel(t0,46,3), fuel(t0,46,4), fuel(t0,56), fuel(t0,56,0), fuel(t0,56,1), fuel(t0,6,10), fuelcost(10,a,b), fuelcost(10,b,a), goal(p0,b), goalreached, in(p0,t0,1), in(p0,t0,2), in(p0,t0,3), in(p0,t0,7), load(p0,t0,a,1), load(p0,t0,b,7), locatable(p0), locatable(t0), location(a), location(b), noop(3), package(p0), preconditions_d(t0,a,b,1), preconditions_d(t0,a,b,10), preconditions_d(t0,a,b,2), preconditions_d(t0,a,b,6), preconditions_d(t0,b,a,3), preconditions_d(t0,b,a,4), preconditions_d(t0,b,a,5), preconditions_d(t0,b,a,7), preconditions_d(t0,b,a,8), preconditions_d(t0,b,a,9), preconditions_l(p0,p0,a,1), preconditions_l(p0,p0,b,10), preconditions_l(p0,p0,b,5), preconditions_l(p0,p0,b,6), preconditions_l(p0,p0,b,7), preconditions_l(p0,p0,b,9), preconditions_l(p0,t0,a,1), preconditions_l(p0,t0,b,5), preconditions_l(p0,t0,b,7), preconditions_l(p0,t0,b,9), preconditions_l(t0,p0,a,1), preconditions_l(t0,p0,b,5), preconditions_l(t0,p0,b,7), preconditions_l(t0,p0,b,9), preconditions_l(t0,t0,a,1), preconditions_l(t0,t0,a,10), preconditions_l(t0,t0,a,2), preconditions_l(t0,t0,a,6), preconditions_l(t0,t0,b,3), preconditions_l(t0,t0,b,4), preconditions_l(t0,t0,b,5), preconditions_l(t0,t0,b,7), preconditions_l(t0,t0,b,8), preconditions_l(t0,t0,b,9), preconditions_u(p0,t0,a,2), preconditions_u(p0,t0,b,3), preconditions_u(p0,t0,b,4), preconditions_u(p0,t0,b,8), step(1), step(10), step(2), step(3), step(4), step(5), step(6), step(7), step(8), step(9), truck(t0), unload(p0,t0,b,4), unload(p0,t0,b,8)}
{at(p0,a), at(p0,a,0), at(p0,b,4), at(p0,b,5), at(p0,b,6), at(t0,a), at(t0,a,0), at(t0,a,1), at(t0,a,5), at(t0,a,9), at(t0,b,10), at(t0,b,2), at(t0,b,3), at(t0,b,4), at(t0,b,6), at(t0,b,7), at(t0,b,8), del(at(p0,a),1), del(at(p0,b),7), del(at(t0,a),10), del(at(t0,a),2), del(at(t0,a),6), del(at(t0,b),5), del(at(t0,b),9), del(fuel(t0,16),10), del(fuel(t0,26),9), del(fuel(t0,36),6), del(fuel(t0,46),5), del(fuel(t0,56),2), del(in(p0,t0),4), drive(t0,a,b,10), drive(t0,a,b,2), drive(t0,a,b,6), drive(t0,b,a,5), drive(t0,b,a,9), fuel(t0,16,9), fuel(t0,26,6), fuel(t0,26,7), fuel(t0,26,8), fuel(t0,36,5), fuel(t0,46,2), fuel(t0,46,3), fuel(t0,46,4), fuel(t0,56), fuel(t0,56,0), fuel(t0,56,1), fuel(t0,6,10), fuelcost(10,a,b), fuelcost(10,b,a), goal(p0,b), goalreached, in(p0,t0,1), in(p0,t0,10), in(p0,t0,2), in(p0,t0,3), in(p0,t0,7), in(p0,t0,8), in(p0,t0,9), load(p0,t0,a,1), load(p0,t0,b,7), locatable(p0), locatable(t0), location(a), location(b), noop(3), noop(8), package(p0), preconditions_d(t0,a,b,1), preconditions_d(t0,a,b,10), preconditions_d(t0,a,b,2), preconditions_d(t0,a,b,6), preconditions_d(t0,b,a,3), preconditions_d(t0,b,a,4), preconditions_d(t0,b,a,5), preconditions_d(t0,b,a,7), preconditions_d(t0,b,a,8), preconditions_d(t0,b,a,9), preconditions_l(p0,p0,a,1), preconditions_l(p0,p0,b,5), preconditions_l(p0,p0,b,6), preconditions_l(p0,p0,b,7), preconditions_l(p0,t0,a,1), preconditions_l(p0,t0,b,5), preconditions_l(p0,t0,b,7), preconditions_l(t0,p0,a,1), preconditions_l(t0,p0,b,5), preconditions_l(t0,p0,b,7), preconditions_l(t0,t0,a,1), preconditions_l(t0,t0,a,10), preconditions_l(t0,t0,a,2), preconditions_l(t0,t0,a,6), preconditions_l(t0,t0,b,3), preconditions_l(t0,t0,b,4), preconditions_l(t0,t0,b,5), preconditions_l(t0,t0,b,7), preconditions_l(t0,t0,b,8), preconditions_l(t0,t0,b,9), preconditions_u(p0,t0,a,10), preconditions_u(p0,t0,a,2), preconditions_u(p0,t0,b,3), preconditions_u(p0,t0,b,4), preconditions_u(p0,t0,b,8), preconditions_u(p0,t0,b,9), step(1), step(10), step(2), step(3), step(4), step(5), step(6), step(7), step(8), step(9), truck(t0), unload(p0,t0,b,4)}
"""
