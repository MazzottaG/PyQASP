input = """
max_value(20).
device(d1). instances(d1,1).
device(d2). instances(d2,2).
offline_instance(d2,1).
job(j1). job_device(j1,d1). job_len(j1,4).
job(j2). job_device(j2,d2). job_len(j2,5). deadline(j2,10). importance(j2,1).
precedes(j1,j2).
job(j3). job_device(j3,d2). job_len(j3,4). deadline(j3,12). importance(j3,2).
max_total_penalty(3).
curr_job_start(j1,0). curr_on_instance(j1,1).
curr_job_start(j2,4). curr_on_instance(j2,1).
curr_time(2).

time(0).
time(T+1) :- time(T), T < MT, max_value(MT).

pen_value(T) :- time(T).
td_value(T) :- time(T).

instance_of(D,1) :- device(D).
instance_of(D,I+1) :- device(D), instance_of(D,I), instances(D,N), I < N.

1 <= { start(J,S) : time(S) } <= 1 :- job(J), not checking_solution.
1 <= { on_instance(J,I) : instance_of(D,I) } <= 1 :- job(J), job_device(J,D), not checking_solution.

:- on_instance(J1,I), on_instance(J2,I), J1 != J2,
   job_device(J1,D), job_device(J2,D),
   start(J1,S1), job_len(J1,L1),
   start(J2,S2),
   S1 <= S2, S2 < S1 + L1.

:- precedes(J1,J2),
   start(J1,S1), job_len(J1,L1),
   start(J2,S2),
   S2 < S1 + L1.

td(J,S + L - D) :-
	job(J),
	start(J,S), job_len(J,L),
	deadline(J,D),
	S + L > D.

td(J,0) :-
	job(J),
	start(J,S), job_len(J,L),
	deadline(J,D),
	S + L <= D.

penalty(J,TD * I) :-
	job(J),
	td(J,TD),
	importance(J,I).

:- penalty(J,P),
   max_value(MV),
   P > MV.

tot_penalty(TP) :-
	pen_value(TP),
	TP = #sum{ P,J : penalty(J,P) }.

has_tot_penalty :-
	tot_penalty(TP).
-has_tot_penalty :-
	not has_tot_penalty.
:- -has_tot_penalty.

:- pen_value(TP), tot_penalty(TP), max_total_penalty(K),
   TP > K.

:- on_instance(J1,I), on_instance(J2,I),
   job_device(J1,D), job_device(J2,D),
   instances(D,N), N > 1,
   J1 != J2,
   start(J1,S1), start(J2,S2),
   job_len(J1,L1),
   S1 <= S2, S2 < S1 + L1.

:- on_instance(J,I),
   device(D),
   job(J), job_device(J,D),
   offline_instance(D,I),
   must_schedule(J).

already_started(J) :-
	curr_job_start(J,S),
	curr_time(CT),
	CT > S.

already_finished(J) :-
	curr_job_start(J,S),
	job_len(J,L),
	curr_time(CT),
	CT >= S + L.

must_schedule(J) :-
	job(J),
	not must_not_schedule(J).

must_not_schedule(J) :-
	already_started(J),
	not rescheduled(J).

rescheduled(J) :-
	already_started(J),
	not already_finished(J),
	job_device(J,D),
	curr_on_instance(J,I),
	offline_instance(D,I).

:- start(J,S),
   curr_time(CT),
   S < CT,
   device(D),
   job_device(J,D),
   time(S),
   must_schedule(J).

:- start(J,S),
   curr_job_start(J,CS),
   S != CS,
   job_device(J,D),
   must_not_schedule(J).

:- on_instance(J,I),
   curr_on_instance(J,CI),
   I != CI,
   must_not_schedule(J).

"""
output = """
{already_started(j1), curr_job_start(j1,0), curr_job_start(j2,4), curr_on_instance(j1,1), curr_on_instance(j2,1), curr_time(2), deadline(j2,10), deadline(j3,12), device(d1), device(d2), has_tot_penalty, importance(j2,1), importance(j3,2), instance_of(d1,1), instance_of(d2,1), instance_of(d2,2), instances(d1,1), instances(d2,2), job(j1), job(j2), job(j3), job_device(j1,d1), job_device(j2,d2), job_device(j3,d2), job_len(j1,4), job_len(j2,5), job_len(j3,4), max_total_penalty(3), max_value(20), must_not_schedule(j1), must_schedule(j2), must_schedule(j3), offline_instance(d2,1), on_instance(j1,1), on_instance(j2,2), on_instance(j3,2), pen_value(0), pen_value(1), pen_value(10), pen_value(11), pen_value(12), pen_value(13), pen_value(14), pen_value(15), pen_value(16), pen_value(17), pen_value(18), pen_value(19), pen_value(2), pen_value(20), pen_value(3), pen_value(4), pen_value(5), pen_value(6), pen_value(7), pen_value(8), pen_value(9), penalty(j2,0), penalty(j3,2), precedes(j1,j2), start(j1,0), start(j2,4), start(j3,9), td(j2,0), td(j3,1), td_value(0), td_value(1), td_value(10), td_value(11), td_value(12), td_value(13), td_value(14), td_value(15), td_value(16), td_value(17), td_value(18), td_value(19), td_value(2), td_value(20), td_value(3), td_value(4), td_value(5), td_value(6), td_value(7), td_value(8), td_value(9), time(0), time(1), time(10), time(11), time(12), time(13), time(14), time(15), time(16), time(17), time(18), time(19), time(2), time(20), time(3), time(4), time(5), time(6), time(7), time(8), time(9), tot_penalty(2)}
{already_started(j1), curr_job_start(j1,0), curr_job_start(j2,4), curr_on_instance(j1,1), curr_on_instance(j2,1), curr_time(2), deadline(j2,10), deadline(j3,12), device(d1), device(d2), has_tot_penalty, importance(j2,1), importance(j3,2), instance_of(d1,1), instance_of(d2,1), instance_of(d2,2), instances(d1,1), instances(d2,2), job(j1), job(j2), job(j3), job_device(j1,d1), job_device(j2,d2), job_device(j3,d2), job_len(j1,4), job_len(j2,5), job_len(j3,4), max_total_penalty(3), max_value(20), must_not_schedule(j1), must_schedule(j2), must_schedule(j3), offline_instance(d2,1), on_instance(j1,1), on_instance(j2,2), on_instance(j3,2), pen_value(0), pen_value(1), pen_value(10), pen_value(11), pen_value(12), pen_value(13), pen_value(14), pen_value(15), pen_value(16), pen_value(17), pen_value(18), pen_value(19), pen_value(2), pen_value(20), pen_value(3), pen_value(4), pen_value(5), pen_value(6), pen_value(7), pen_value(8), pen_value(9), penalty(j2,3), penalty(j3,0), precedes(j1,j2), start(j1,0), start(j2,8), start(j3,4), td(j2,3), td(j3,0), td_value(0), td_value(1), td_value(10), td_value(11), td_value(12), td_value(13), td_value(14), td_value(15), td_value(16), td_value(17), td_value(18), td_value(19), td_value(2), td_value(20), td_value(3), td_value(4), td_value(5), td_value(6), td_value(7), td_value(8), td_value(9), time(0), time(1), time(10), time(11), time(12), time(13), time(14), time(15), time(16), time(17), time(18), time(19), time(2), time(20), time(3), time(4), time(5), time(6), time(7), time(8), time(9), tot_penalty(3)}
{already_started(j1), curr_job_start(j1,0), curr_job_start(j2,4), curr_on_instance(j1,1), curr_on_instance(j2,1), curr_time(2), deadline(j2,10), deadline(j3,12), device(d1), device(d2), has_tot_penalty, importance(j2,1), importance(j3,2), instance_of(d1,1), instance_of(d2,1), instance_of(d2,2), instances(d1,1), instances(d2,2), job(j1), job(j2), job(j3), job_device(j1,d1), job_device(j2,d2), job_device(j3,d2), job_len(j1,4), job_len(j2,5), job_len(j3,4), max_total_penalty(3), max_value(20), must_not_schedule(j1), must_schedule(j2), must_schedule(j3), offline_instance(d2,1), on_instance(j1,1), on_instance(j2,2), on_instance(j3,2), pen_value(0), pen_value(1), pen_value(10), pen_value(11), pen_value(12), pen_value(13), pen_value(14), pen_value(15), pen_value(16), pen_value(17), pen_value(18), pen_value(19), pen_value(2), pen_value(20), pen_value(3), pen_value(4), pen_value(5), pen_value(6), pen_value(7), pen_value(8), pen_value(9), penalty(j2,3), penalty(j3,0), precedes(j1,j2), start(j1,0), start(j2,8), start(j3,3), td(j2,3), td(j3,0), td_value(0), td_value(1), td_value(10), td_value(11), td_value(12), td_value(13), td_value(14), td_value(15), td_value(16), td_value(17), td_value(18), td_value(19), td_value(2), td_value(20), td_value(3), td_value(4), td_value(5), td_value(6), td_value(7), td_value(8), td_value(9), time(0), time(1), time(10), time(11), time(12), time(13), time(14), time(15), time(16), time(17), time(18), time(19), time(2), time(20), time(3), time(4), time(5), time(6), time(7), time(8), time(9), tot_penalty(3)}
{already_started(j1), curr_job_start(j1,0), curr_job_start(j2,4), curr_on_instance(j1,1), curr_on_instance(j2,1), curr_time(2), deadline(j2,10), deadline(j3,12), device(d1), device(d2), has_tot_penalty, importance(j2,1), importance(j3,2), instance_of(d1,1), instance_of(d2,1), instance_of(d2,2), instances(d1,1), instances(d2,2), job(j1), job(j2), job(j3), job_device(j1,d1), job_device(j2,d2), job_device(j3,d2), job_len(j1,4), job_len(j2,5), job_len(j3,4), max_total_penalty(3), max_value(20), must_not_schedule(j1), must_schedule(j2), must_schedule(j3), offline_instance(d2,1), on_instance(j1,1), on_instance(j2,2), on_instance(j3,2), pen_value(0), pen_value(1), pen_value(10), pen_value(11), pen_value(12), pen_value(13), pen_value(14), pen_value(15), pen_value(16), pen_value(17), pen_value(18), pen_value(19), pen_value(2), pen_value(20), pen_value(3), pen_value(4), pen_value(5), pen_value(6), pen_value(7), pen_value(8), pen_value(9), penalty(j2,3), penalty(j3,0), precedes(j1,j2), start(j1,0), start(j2,8), start(j3,2), td(j2,3), td(j3,0), td_value(0), td_value(1), td_value(10), td_value(11), td_value(12), td_value(13), td_value(14), td_value(15), td_value(16), td_value(17), td_value(18), td_value(19), td_value(2), td_value(20), td_value(3), td_value(4), td_value(5), td_value(6), td_value(7), td_value(8), td_value(9), time(0), time(1), time(10), time(11), time(12), time(13), time(14), time(15), time(16), time(17), time(18), time(19), time(2), time(20), time(3), time(4), time(5), time(6), time(7), time(8), time(9), tot_penalty(3)}
{already_started(j1), curr_job_start(j1,0), curr_job_start(j2,4), curr_on_instance(j1,1), curr_on_instance(j2,1), curr_time(2), deadline(j2,10), deadline(j3,12), device(d1), device(d2), has_tot_penalty, importance(j2,1), importance(j3,2), instance_of(d1,1), instance_of(d2,1), instance_of(d2,2), instances(d1,1), instances(d2,2), job(j1), job(j2), job(j3), job_device(j1,d1), job_device(j2,d2), job_device(j3,d2), job_len(j1,4), job_len(j2,5), job_len(j3,4), max_total_penalty(3), max_value(20), must_not_schedule(j1), must_schedule(j2), must_schedule(j3), offline_instance(d2,1), on_instance(j1,1), on_instance(j2,2), on_instance(j3,2), pen_value(0), pen_value(1), pen_value(10), pen_value(11), pen_value(12), pen_value(13), pen_value(14), pen_value(15), pen_value(16), pen_value(17), pen_value(18), pen_value(19), pen_value(2), pen_value(20), pen_value(3), pen_value(4), pen_value(5), pen_value(6), pen_value(7), pen_value(8), pen_value(9), penalty(j2,1), penalty(j3,0), precedes(j1,j2), start(j1,0), start(j2,6), start(j3,2), td(j2,1), td(j3,0), td_value(0), td_value(1), td_value(10), td_value(11), td_value(12), td_value(13), td_value(14), td_value(15), td_value(16), td_value(17), td_value(18), td_value(19), td_value(2), td_value(20), td_value(3), td_value(4), td_value(5), td_value(6), td_value(7), td_value(8), td_value(9), time(0), time(1), time(10), time(11), time(12), time(13), time(14), time(15), time(16), time(17), time(18), time(19), time(2), time(20), time(3), time(4), time(5), time(6), time(7), time(8), time(9), tot_penalty(1)}
{already_started(j1), curr_job_start(j1,0), curr_job_start(j2,4), curr_on_instance(j1,1), curr_on_instance(j2,1), curr_time(2), deadline(j2,10), deadline(j3,12), device(d1), device(d2), has_tot_penalty, importance(j2,1), importance(j3,2), instance_of(d1,1), instance_of(d2,1), instance_of(d2,2), instances(d1,1), instances(d2,2), job(j1), job(j2), job(j3), job_device(j1,d1), job_device(j2,d2), job_device(j3,d2), job_len(j1,4), job_len(j2,5), job_len(j3,4), max_total_penalty(3), max_value(20), must_not_schedule(j1), must_schedule(j2), must_schedule(j3), offline_instance(d2,1), on_instance(j1,1), on_instance(j2,2), on_instance(j3,2), pen_value(0), pen_value(1), pen_value(10), pen_value(11), pen_value(12), pen_value(13), pen_value(14), pen_value(15), pen_value(16), pen_value(17), pen_value(18), pen_value(19), pen_value(2), pen_value(20), pen_value(3), pen_value(4), pen_value(5), pen_value(6), pen_value(7), pen_value(8), pen_value(9), penalty(j2,2), penalty(j3,0), precedes(j1,j2), start(j1,0), start(j2,7), start(j3,3), td(j2,2), td(j3,0), td_value(0), td_value(1), td_value(10), td_value(11), td_value(12), td_value(13), td_value(14), td_value(15), td_value(16), td_value(17), td_value(18), td_value(19), td_value(2), td_value(20), td_value(3), td_value(4), td_value(5), td_value(6), td_value(7), td_value(8), td_value(9), time(0), time(1), time(10), time(11), time(12), time(13), time(14), time(15), time(16), time(17), time(18), time(19), time(2), time(20), time(3), time(4), time(5), time(6), time(7), time(8), time(9), tot_penalty(2)}
{already_started(j1), curr_job_start(j1,0), curr_job_start(j2,4), curr_on_instance(j1,1), curr_on_instance(j2,1), curr_time(2), deadline(j2,10), deadline(j3,12), device(d1), device(d2), has_tot_penalty, importance(j2,1), importance(j3,2), instance_of(d1,1), instance_of(d2,1), instance_of(d2,2), instances(d1,1), instances(d2,2), job(j1), job(j2), job(j3), job_device(j1,d1), job_device(j2,d2), job_device(j3,d2), job_len(j1,4), job_len(j2,5), job_len(j3,4), max_total_penalty(3), max_value(20), must_not_schedule(j1), must_schedule(j2), must_schedule(j3), offline_instance(d2,1), on_instance(j1,1), on_instance(j2,2), on_instance(j3,2), pen_value(0), pen_value(1), pen_value(10), pen_value(11), pen_value(12), pen_value(13), pen_value(14), pen_value(15), pen_value(16), pen_value(17), pen_value(18), pen_value(19), pen_value(2), pen_value(20), pen_value(3), pen_value(4), pen_value(5), pen_value(6), pen_value(7), pen_value(8), pen_value(9), penalty(j2,2), penalty(j3,0), precedes(j1,j2), start(j1,0), start(j2,7), start(j3,2), td(j2,2), td(j3,0), td_value(0), td_value(1), td_value(10), td_value(11), td_value(12), td_value(13), td_value(14), td_value(15), td_value(16), td_value(17), td_value(18), td_value(19), td_value(2), td_value(20), td_value(3), td_value(4), td_value(5), td_value(6), td_value(7), td_value(8), td_value(9), time(0), time(1), time(10), time(11), time(12), time(13), time(14), time(15), time(16), time(17), time(18), time(19), time(2), time(20), time(3), time(4), time(5), time(6), time(7), time(8), time(9), tot_penalty(2)}
"""
