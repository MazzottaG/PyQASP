input = """
#show valve/2.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%% VALVES LOCATION OPTIMIZATION %%%%%%%
%%%%%%% FO: MAX MIN SATISFIED DEM.   %%%%%%%
%%%%%%%   TYPE: PIPE REACHABILITY    %%%%%%%
%%%%%%%   ANDREA PEANO 10/10/2012    %%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

less_ico(pipe(A,B), pipe(C,D)):- pipe(A,B), pipe(C,D), A<C.
less_ico(pipe(A,B), pipe(C,D)):- pipe(A,B), pipe(C,D), A = C, B<D.

valves_number(4).
valves_per_pipe(1).
tank(1).
junction(1). pipe(1,2). pipe(1,9). dem(1,9,47). dem(1,2,205).
junction(2). pipe(2,3). pipe(2,12). dem(2,12,187). dem(2,3,205).
"""
output = """
{}
"""
