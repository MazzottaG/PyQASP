input = """
% instance

% Csp
node1(0..2). node2(0..2).
%  0  1 ( DC )
lc(0,1,rEQ). lc(0,1,rEC). lc(0,1,rPO).
lc(0,1,rTPP). lc(0,1,rNTPP). lc(0,1,rTPPI).
lc(0,1,rNTPPI).
%  1  2 ( PO )
lc(1,2,rEQ). lc(1,2,rDC). lc(1,2,rEC).
lc(1,2,rTPP). lc(1,2,rNTPP). lc(1,2,rTPPI).
lc(1,2,rNTPPI).

:- not label(1,2,rd).
:- not label(0,1,rpi).

% encoding

% A Disjunctive Logic Program for IA Constraint Networks 

% relations 
rel(req).
rel(rp).
rel(rpi).
rel(rd).
rel(rdi).
rel(ro).
rel(roi).
rel(rm).
rel(rmi).
rel(rs).
rel(rsi).
rel(rf).
rel(rfi).
% Choice rule for clasp
1 <= {label(X,Y,L) : rel(L)} <= 1 :- node1(X), node2(Y), X<Y.
:- label(X,Y,L), lc(X,Y,L), node1(X), node2(Y), rel(L).

% Composition table
% req o req = r= 
label(X,Z,req) :- label(X,Y,req), label(Y,Z,req).
% req o rp = r< 
label(X,Z,rp) :- label(X,Y,req), label(Y,Z,rp).
% req o rpi = r> 
label(X,Z,rpi) :- label(X,Y,req), label(Y,Z,rpi).
% req o rd = rd 
label(X,Z,rd) :- label(X,Y,req), label(Y,Z,rd).
% req o rdi = rdi 
label(X,Z,rdi) :- label(X,Y,req), label(Y,Z,rdi).
% req o rs = rs 
label(X,Z,rs) :- label(X,Y,req), label(Y,Z,rs).
% req o rsi = rsi 
label(X,Z,rsi) :- label(X,Y,req), label(Y,Z,rsi).
% req o rf = rf 
label(X,Z,rf) :- label(X,Y,req), label(Y,Z,rf).
% req o rfi = rfi 
label(X,Z,rfi) :- label(X,Y,req), label(Y,Z,rfi).
% req o rm = rm 
label(X,Z,rm) :- label(X,Y,req), label(Y,Z,rm).
% req o rmi = rmi 
label(X,Z,rmi) :- label(X,Y,req), label(Y,Z,rmi).
% req o ro = ro 
label(X,Z,ro) :- label(X,Y,req), label(Y,Z,ro).
% req o roi = roi 
label(X,Z,roi) :- label(X,Y,req), label(Y,Z,roi).
% rp o req = r< 
label(X,Z,rp) :- label(X,Y,rp), label(Y,Z,req).
% rp o rp = r< 
label(X,Z,rp) :- label(X,Y,rp), label(Y,Z,rp).
% rp o rpi = r= < > d di s si f fi m mi o oi 
label(X,Z,req) | label(X,Z,rp) | label(X,Z,rpi) | label(X,Z,rd) | label(X,Z,rdi) | label(X,Z,rs) | label(X,Z,rsi) | label(X,Z,rf) | label(X,Z,rfi) | label(X,Z,rm) | label(X,Z,rmi) | label(X,Z,ro) | label(X,Z,roi) :- label(X,Y,rp), label(Y,Z,rpi).
% rp o rd = r< d s m o 
label(X,Z,rp) | label(X,Z,rd) | label(X,Z,rs) | label(X,Z,rm) | label(X,Z,ro) :- label(X,Y,rp), label(Y,Z,rd).
% rp o rdi = r< 
label(X,Z,rp) :- label(X,Y,rp), label(Y,Z,rdi).
% rp o rs = r< 
label(X,Z,rp) :- label(X,Y,rp), label(Y,Z,rs).
% rp o rsi = r< 
label(X,Z,rp) :- label(X,Y,rp), label(Y,Z,rsi).
% rp o rf = r< d s m o 
label(X,Z,rp) | label(X,Z,rd) | label(X,Z,rs) | label(X,Z,rm) | label(X,Z,ro) :- label(X,Y,rp), label(Y,Z,rf).
% rp o rfi = r< 
label(X,Z,rp) :- label(X,Y,rp), label(Y,Z,rfi).
% rp o rm = r< 
label(X,Z,rp) :- label(X,Y,rp), label(Y,Z,rm).
% rp o rmi = r< d s m o 
label(X,Z,rp) | label(X,Z,rd) | label(X,Z,rs) | label(X,Z,rm) | label(X,Z,ro) :- label(X,Y,rp), label(Y,Z,rmi).
% rp o ro = r< 
label(X,Z,rp) :- label(X,Y,rp), label(Y,Z,ro).
% rp o roi = r< d s m o 
label(X,Z,rp) | label(X,Z,rd) | label(X,Z,rs) | label(X,Z,rm) | label(X,Z,ro) :- label(X,Y,rp), label(Y,Z,roi).
% rpi o req = r> 
label(X,Z,rpi) :- label(X,Y,rpi), label(Y,Z,req).
% rpi o rp = r= < > d di s si f fi m mi o oi 
label(X,Z,req) | label(X,Z,rp) | label(X,Z,rpi) | label(X,Z,rd) | label(X,Z,rdi) | label(X,Z,rs) | label(X,Z,rsi) | label(X,Z,rf) | label(X,Z,rfi) | label(X,Z,rm) | label(X,Z,rmi) | label(X,Z,ro) | label(X,Z,roi) :- label(X,Y,rpi), label(Y,Z,rp).
% rpi o rpi = r> 
label(X,Z,rpi) :- label(X,Y,rpi), label(Y,Z,rpi).
% rpi o rd = r> d f mi oi 
label(X,Z,rpi) | label(X,Z,rd) | label(X,Z,rf) | label(X,Z,rmi) | label(X,Z,roi) :- label(X,Y,rpi), label(Y,Z,rd).
% rpi o rdi = r> 
label(X,Z,rpi) :- label(X,Y,rpi), label(Y,Z,rdi).
% rpi o rs = r> d f mi oi 
label(X,Z,rpi) | label(X,Z,rd) | label(X,Z,rf) | label(X,Z,rmi) | label(X,Z,roi) :- label(X,Y,rpi), label(Y,Z,rs).
% rpi o rsi = r> 
label(X,Z,rpi) :- label(X,Y,rpi), label(Y,Z,rsi).
% rpi o rf = r> 
label(X,Z,rpi) :- label(X,Y,rpi), label(Y,Z,rf).
% rpi o rfi = r> 
label(X,Z,rpi) :- label(X,Y,rpi), label(Y,Z,rfi).
% rpi o rm = r> d f mi oi 
label(X,Z,rpi) | label(X,Z,rd) | label(X,Z,rf) | label(X,Z,rmi) | label(X,Z,roi) :- label(X,Y,rpi), label(Y,Z,rm).
% rpi o rmi = r> 
label(X,Z,rpi) :- label(X,Y,rpi), label(Y,Z,rmi).
% rpi o ro = r> d f mi oi 
label(X,Z,rpi) | label(X,Z,rd) | label(X,Z,rf) | label(X,Z,rmi) | label(X,Z,roi) :- label(X,Y,rpi), label(Y,Z,ro).
% rpi o roi = r> 
label(X,Z,rpi) :- label(X,Y,rpi), label(Y,Z,roi).
% rd o req = rd 
label(X,Z,rd) :- label(X,Y,rd), label(Y,Z,req).
% rd o rp = r< 
label(X,Z,rp) :- label(X,Y,rd), label(Y,Z,rp).
% rd o rpi = r> 
label(X,Z,rpi) :- label(X,Y,rd), label(Y,Z,rpi).
% rd o rd = rd 
label(X,Z,rd) :- label(X,Y,rd), label(Y,Z,rd).
% rd o rdi = r= < > d di s si f fi m mi o oi 
label(X,Z,req) | label(X,Z,rp) | label(X,Z,rpi) | label(X,Z,rd) | label(X,Z,rdi) | label(X,Z,rs) | label(X,Z,rsi) | label(X,Z,rf) | label(X,Z,rfi) | label(X,Z,rm) | label(X,Z,rmi) | label(X,Z,ro) | label(X,Z,roi) :- label(X,Y,rd), label(Y,Z,rdi).
% rd o rs = rd 
label(X,Z,rd) :- label(X,Y,rd), label(Y,Z,rs).
% rd o rsi = r> d f mi oi 
label(X,Z,rpi) | label(X,Z,rd) | label(X,Z,rf) | label(X,Z,rmi) | label(X,Z,roi) :- label(X,Y,rd), label(Y,Z,rsi).
% rd o rf = rd 
label(X,Z,rd) :- label(X,Y,rd), label(Y,Z,rf).
% rd o rfi = r< d s m o 
label(X,Z,rp) | label(X,Z,rd) | label(X,Z,rs) | label(X,Z,rm) | label(X,Z,ro) :- label(X,Y,rd), label(Y,Z,rfi).
% rd o rm = r< 
label(X,Z,rp) :- label(X,Y,rd), label(Y,Z,rm).
% rd o rmi = r> 
label(X,Z,rpi) :- label(X,Y,rd), label(Y,Z,rmi).
% rd o ro = r< d s m o 
label(X,Z,rp) | label(X,Z,rd) | label(X,Z,rs) | label(X,Z,rm) | label(X,Z,ro) :- label(X,Y,rd), label(Y,Z,ro).
% rd o roi = r> d f mi oi 
label(X,Z,rpi) | label(X,Z,rd) | label(X,Z,rf) | label(X,Z,rmi) | label(X,Z,roi) :- label(X,Y,rd), label(Y,Z,roi).
% rdi o req = rdi 
label(X,Z,rdi) :- label(X,Y,rdi), label(Y,Z,req).
% rdi o rp = r< di fi m o 
label(X,Z,rp) | label(X,Z,rdi) | label(X,Z,rfi) | label(X,Z,rm) | label(X,Z,ro) :- label(X,Y,rdi), label(Y,Z,rp).
% rdi o rpi = r> di si mi oi 
label(X,Z,rpi) | label(X,Z,rdi) | label(X,Z,rsi) | label(X,Z,rmi) | label(X,Z,roi) :- label(X,Y,rdi), label(Y,Z,rpi).
% rdi o rd = r= d di s si f fi o oi 
label(X,Z,req) | label(X,Z,rd) | label(X,Z,rdi) | label(X,Z,rs) | label(X,Z,rsi) | label(X,Z,rf) | label(X,Z,rfi) | label(X,Z,ro) | label(X,Z,roi) :- label(X,Y,rdi), label(Y,Z,rd).
% rdi o rdi = rdi 
label(X,Z,rdi) :- label(X,Y,rdi), label(Y,Z,rdi).
% rdi o rs = rdi fi o 
label(X,Z,rdi) | label(X,Z,rfi) | label(X,Z,ro) :- label(X,Y,rdi), label(Y,Z,rs).
% rdi o rsi = rdi 
label(X,Z,rdi) :- label(X,Y,rdi), label(Y,Z,rsi).
% rdi o rf = rdi si oi 
label(X,Z,rdi) | label(X,Z,rsi) | label(X,Z,roi) :- label(X,Y,rdi), label(Y,Z,rf).
% rdi o rfi = rdi 
label(X,Z,rdi) :- label(X,Y,rdi), label(Y,Z,rfi).
% rdi o rm = rdi fi o 
label(X,Z,rdi) | label(X,Z,rfi) | label(X,Z,ro) :- label(X,Y,rdi), label(Y,Z,rm).
% rdi o rmi = rdi si oi 
label(X,Z,rdi) | label(X,Z,rsi) | label(X,Z,roi) :- label(X,Y,rdi), label(Y,Z,rmi).
% rdi o ro = rdi fi o 
label(X,Z,rdi) | label(X,Z,rfi) | label(X,Z,ro) :- label(X,Y,rdi), label(Y,Z,ro).
% rdi o roi = rdi si oi 
label(X,Z,rdi) | label(X,Z,rsi) | label(X,Z,roi) :- label(X,Y,rdi), label(Y,Z,roi).
% rs o req = rs 
label(X,Z,rs) :- label(X,Y,rs), label(Y,Z,req).
% rs o rp = r< 
label(X,Z,rp) :- label(X,Y,rs), label(Y,Z,rp).
% rs o rpi = r> 
label(X,Z,rpi) :- label(X,Y,rs), label(Y,Z,rpi).
% rs o rd = rd 
label(X,Z,rd) :- label(X,Y,rs), label(Y,Z,rd).
% rs o rdi = r< di fi m o 
label(X,Z,rp) | label(X,Z,rdi) | label(X,Z,rfi) | label(X,Z,rm) | label(X,Z,ro) :- label(X,Y,rs), label(Y,Z,rdi).
% rs o rs = rs 
label(X,Z,rs) :- label(X,Y,rs), label(Y,Z,rs).
% rs o rsi = r= s si 
label(X,Z,req) | label(X,Z,rs) | label(X,Z,rsi) :- label(X,Y,rs), label(Y,Z,rsi).
% rs o rf = rd 
label(X,Z,rd) :- label(X,Y,rs), label(Y,Z,rf).
% rs o rfi = r< m o 
label(X,Z,rp) | label(X,Z,rm) | label(X,Z,ro) :- label(X,Y,rs), label(Y,Z,rfi).
% rs o rm = r< 
label(X,Z,rp) :- label(X,Y,rs), label(Y,Z,rm).
% rs o rmi = rmi 
label(X,Z,rmi) :- label(X,Y,rs), label(Y,Z,rmi).
% rs o ro = r< m o 
label(X,Z,rp) | label(X,Z,rm) | label(X,Z,ro) :- label(X,Y,rs), label(Y,Z,ro).
% rs o roi = rd f oi 
label(X,Z,rd) | label(X,Z,rf) | label(X,Z,roi) :- label(X,Y,rs), label(Y,Z,roi).
% rsi o req = rsi 
label(X,Z,rsi) :- label(X,Y,rsi), label(Y,Z,req).
% rsi o rp = r< di fi m o 
label(X,Z,rp) | label(X,Z,rdi) | label(X,Z,rfi) | label(X,Z,rm) | label(X,Z,ro) :- label(X,Y,rsi), label(Y,Z,rp).
% rsi o rpi = r> 
label(X,Z,rpi) :- label(X,Y,rsi), label(Y,Z,rpi).
% rsi o rd = rd f oi 
label(X,Z,rd) | label(X,Z,rf) | label(X,Z,roi) :- label(X,Y,rsi), label(Y,Z,rd).
% rsi o rdi = rdi 
label(X,Z,rdi) :- label(X,Y,rsi), label(Y,Z,rdi).
% rsi o rs = r= s si 
label(X,Z,req) | label(X,Z,rs) | label(X,Z,rsi) :- label(X,Y,rsi), label(Y,Z,rs).
% rsi o rsi = rsi 
label(X,Z,rsi) :- label(X,Y,rsi), label(Y,Z,rsi).
% rsi o rf = roi 
label(X,Z,roi) :- label(X,Y,rsi), label(Y,Z,rf).
% rsi o rfi = rdi 
label(X,Z,rdi) :- label(X,Y,rsi), label(Y,Z,rfi).
% rsi o rm = rdi fi o 
label(X,Z,rdi) | label(X,Z,rfi) | label(X,Z,ro) :- label(X,Y,rsi), label(Y,Z,rm).
% rsi o rmi = rmi 
label(X,Z,rmi) :- label(X,Y,rsi), label(Y,Z,rmi).
% rsi o ro = rdi fi o 
label(X,Z,rdi) | label(X,Z,rfi) | label(X,Z,ro) :- label(X,Y,rsi), label(Y,Z,ro).
% rsi o roi = roi 
label(X,Z,roi) :- label(X,Y,rsi), label(Y,Z,roi).
% rf o req = rf 
label(X,Z,rf) :- label(X,Y,rf), label(Y,Z,req).
% rf o rp = r< 
label(X,Z,rp) :- label(X,Y,rf), label(Y,Z,rp).
% rf o rpi = r> 
label(X,Z,rpi) :- label(X,Y,rf), label(Y,Z,rpi).
% rf o rd = rd 
label(X,Z,rd) :- label(X,Y,rf), label(Y,Z,rd).
% rf o rdi = r> di si mi oi 
label(X,Z,rpi) | label(X,Z,rdi) | label(X,Z,rsi) | label(X,Z,rmi) | label(X,Z,roi) :- label(X,Y,rf), label(Y,Z,rdi).
% rf o rs = rd 
label(X,Z,rd) :- label(X,Y,rf), label(Y,Z,rs).
% rf o rsi = r> mi oi 
label(X,Z,rpi) | label(X,Z,rmi) | label(X,Z,roi) :- label(X,Y,rf), label(Y,Z,rsi).
% rf o rf = rf 
label(X,Z,rf) :- label(X,Y,rf), label(Y,Z,rf).
% rf o rfi = r= f fi 
label(X,Z,req) | label(X,Z,rf) | label(X,Z,rfi) :- label(X,Y,rf), label(Y,Z,rfi).
% rf o rm = rm 
label(X,Z,rm) :- label(X,Y,rf), label(Y,Z,rm).
% rf o rmi = r> 
label(X,Z,rpi) :- label(X,Y,rf), label(Y,Z,rmi).
% rf o ro = rd s o 
label(X,Z,rd) | label(X,Z,rs) | label(X,Z,ro) :- label(X,Y,rf), label(Y,Z,ro).
% rf o roi = r> mi oi 
label(X,Z,rpi) | label(X,Z,rmi) | label(X,Z,roi) :- label(X,Y,rf), label(Y,Z,roi).
% rfi o req = rfi 
label(X,Z,rfi) :- label(X,Y,rfi), label(Y,Z,req).
% rfi o rp = r< 
label(X,Z,rp) :- label(X,Y,rfi), label(Y,Z,rp).
% rfi o rpi = r> di si mi oi 
label(X,Z,rpi) | label(X,Z,rdi) | label(X,Z,rsi) | label(X,Z,rmi) | label(X,Z,roi) :- label(X,Y,rfi), label(Y,Z,rpi).
% rfi o rd = rd s o 
label(X,Z,rd) | label(X,Z,rs) | label(X,Z,ro) :- label(X,Y,rfi), label(Y,Z,rd).
% rfi o rdi = rdi 
label(X,Z,rdi) :- label(X,Y,rfi), label(Y,Z,rdi).
% rfi o rs = ro 
label(X,Z,ro) :- label(X,Y,rfi), label(Y,Z,rs).
% rfi o rsi = rdi 
label(X,Z,rdi) :- label(X,Y,rfi), label(Y,Z,rsi).
% rfi o rf = r= f fi 
label(X,Z,req) | label(X,Z,rf) | label(X,Z,rfi) :- label(X,Y,rfi), label(Y,Z,rf).
% rfi o rfi = rfi 
label(X,Z,rfi) :- label(X,Y,rfi), label(Y,Z,rfi).
% rfi o rm = rm 
label(X,Z,rm) :- label(X,Y,rfi), label(Y,Z,rm).
% rfi o rmi = rdi si oi 
label(X,Z,rdi) | label(X,Z,rsi) | label(X,Z,roi) :- label(X,Y,rfi), label(Y,Z,rmi).
% rfi o ro = ro 
label(X,Z,ro) :- label(X,Y,rfi), label(Y,Z,ro).
% rfi o roi = rdi si oi 
label(X,Z,rdi) | label(X,Z,rsi) | label(X,Z,roi) :- label(X,Y,rfi), label(Y,Z,roi).
% rm o req = rm 
label(X,Z,rm) :- label(X,Y,rm), label(Y,Z,req).
% rm o rp = r< 
label(X,Z,rp) :- label(X,Y,rm), label(Y,Z,rp).
% rm o rpi = r> di si mi oi 
label(X,Z,rpi) | label(X,Z,rdi) | label(X,Z,rsi) | label(X,Z,rmi) | label(X,Z,roi) :- label(X,Y,rm), label(Y,Z,rpi).
% rm o rd = rd s o 
label(X,Z,rd) | label(X,Z,rs) | label(X,Z,ro) :- label(X,Y,rm), label(Y,Z,rd).
% rm o rdi = r< 
label(X,Z,rp) :- label(X,Y,rm), label(Y,Z,rdi).
% rm o rs = rm 
label(X,Z,rm) :- label(X,Y,rm), label(Y,Z,rs).
% rm o rsi = rm 
label(X,Z,rm) :- label(X,Y,rm), label(Y,Z,rsi).
% rm o rf = rd s o 
label(X,Z,rd) | label(X,Z,rs) | label(X,Z,ro) :- label(X,Y,rm), label(Y,Z,rf).
% rm o rfi = r< 
label(X,Z,rp) :- label(X,Y,rm), label(Y,Z,rfi).
% rm o rm = r< 
label(X,Z,rp) :- label(X,Y,rm), label(Y,Z,rm).
% rm o rmi = r= f fi 
label(X,Z,req) | label(X,Z,rf) | label(X,Z,rfi) :- label(X,Y,rm), label(Y,Z,rmi).
% rm o ro = r< 
label(X,Z,rp) :- label(X,Y,rm), label(Y,Z,ro).
% rm o roi = rd s o 
label(X,Z,rd) | label(X,Z,rs) | label(X,Z,ro) :- label(X,Y,rm), label(Y,Z,roi).
% rmi o req = rmi 
label(X,Z,rmi) :- label(X,Y,rmi), label(Y,Z,req).
% rmi o rp = r< di fi m o 
label(X,Z,rp) | label(X,Z,rdi) | label(X,Z,rfi) | label(X,Z,rm) | label(X,Z,ro) :- label(X,Y,rmi), label(Y,Z,rp).
% rmi o rpi = r> 
label(X,Z,rpi) :- label(X,Y,rmi), label(Y,Z,rpi).
% rmi o rd = rd f oi 
label(X,Z,rd) | label(X,Z,rf) | label(X,Z,roi) :- label(X,Y,rmi), label(Y,Z,rd).
% rmi o rdi = r> 
label(X,Z,rpi) :- label(X,Y,rmi), label(Y,Z,rdi).
% rmi o rs = rd f oi 
label(X,Z,rd) | label(X,Z,rf) | label(X,Z,roi) :- label(X,Y,rmi), label(Y,Z,rs).
% rmi o rsi = r> 
label(X,Z,rpi) :- label(X,Y,rmi), label(Y,Z,rsi).
% rmi o rf = rmi 
label(X,Z,rmi) :- label(X,Y,rmi), label(Y,Z,rf).
% rmi o rfi = rmi 
label(X,Z,rmi) :- label(X,Y,rmi), label(Y,Z,rfi).
% rmi o rm = r= s si 
label(X,Z,req) | label(X,Z,rs) | label(X,Z,rsi) :- label(X,Y,rmi), label(Y,Z,rm).
% rmi o rmi = r> 
label(X,Z,rpi) :- label(X,Y,rmi), label(Y,Z,rmi).
% rmi o ro = rd f oi 
label(X,Z,rd) | label(X,Z,rf) | label(X,Z,roi) :- label(X,Y,rmi), label(Y,Z,ro).
% rmi o roi = r> 
label(X,Z,rpi) :- label(X,Y,rmi), label(Y,Z,roi).
% ro o req = ro 
label(X,Z,ro) :- label(X,Y,ro), label(Y,Z,req).
% ro o rp = r< 
label(X,Z,rp) :- label(X,Y,ro), label(Y,Z,rp).
% ro o rpi = r> di si mi oi 
label(X,Z,rpi) | label(X,Z,rdi) | label(X,Z,rsi) | label(X,Z,rmi) | label(X,Z,roi) :- label(X,Y,ro), label(Y,Z,rpi).
% ro o rd = rd s o 
label(X,Z,rd) | label(X,Z,rs) | label(X,Z,ro) :- label(X,Y,ro), label(Y,Z,rd).
% ro o rdi = r< di fi m o 
label(X,Z,rp) | label(X,Z,rdi) | label(X,Z,rfi) | label(X,Z,rm) | label(X,Z,ro) :- label(X,Y,ro), label(Y,Z,rdi).
% ro o rs = ro 
label(X,Z,ro) :- label(X,Y,ro), label(Y,Z,rs).
% ro o rsi = rdi fi o 
label(X,Z,rdi) | label(X,Z,rfi) | label(X,Z,ro) :- label(X,Y,ro), label(Y,Z,rsi).
% ro o rf = rd s o 
label(X,Z,rd) | label(X,Z,rs) | label(X,Z,ro) :- label(X,Y,ro), label(Y,Z,rf).
% ro o rfi = r< m o 
label(X,Z,rp) | label(X,Z,rm) | label(X,Z,ro) :- label(X,Y,ro), label(Y,Z,rfi).
% ro o rm = r< 
label(X,Z,rp) :- label(X,Y,ro), label(Y,Z,rm).
% ro o rmi = rdi si oi 
label(X,Z,rdi) | label(X,Z,rsi) | label(X,Z,roi) :- label(X,Y,ro), label(Y,Z,rmi).
% ro o ro = r< m o 
label(X,Z,rp) | label(X,Z,rm) | label(X,Z,ro) :- label(X,Y,ro), label(Y,Z,ro).
% ro o roi = r= d di s si f fi o oi 
label(X,Z,req) | label(X,Z,rd) | label(X,Z,rdi) | label(X,Z,rs) | label(X,Z,rsi) | label(X,Z,rf) | label(X,Z,rfi) | label(X,Z,ro) | label(X,Z,roi) :- label(X,Y,ro), label(Y,Z,roi).
% roi o req = roi 
label(X,Z,roi) :- label(X,Y,roi), label(Y,Z,req).
% roi o rp = r< di fi m o 
label(X,Z,rp) | label(X,Z,rdi) | label(X,Z,rfi) | label(X,Z,rm) | label(X,Z,ro) :- label(X,Y,roi), label(Y,Z,rp).
% roi o rpi = r> 
label(X,Z,rpi) :- label(X,Y,roi), label(Y,Z,rpi).
% roi o rd = rd f oi 
label(X,Z,rd) | label(X,Z,rf) | label(X,Z,roi) :- label(X,Y,roi), label(Y,Z,rd).
% roi o rdi = r> di si mi oi 
label(X,Z,rpi) | label(X,Z,rdi) | label(X,Z,rsi) | label(X,Z,rmi) | label(X,Z,roi) :- label(X,Y,roi), label(Y,Z,rdi).
% roi o rs = rd f oi 
label(X,Z,rd) | label(X,Z,rf) | label(X,Z,roi) :- label(X,Y,roi), label(Y,Z,rs).
% roi o rsi = r> mi oi 
label(X,Z,rpi) | label(X,Z,rmi) | label(X,Z,roi) :- label(X,Y,roi), label(Y,Z,rsi).
% roi o rf = roi 
label(X,Z,roi) :- label(X,Y,roi), label(Y,Z,rf).
% roi o rfi = rdi si oi 
label(X,Z,rdi) | label(X,Z,rsi) | label(X,Z,roi) :- label(X,Y,roi), label(Y,Z,rfi).
% roi o rm = rdi fi o 
label(X,Z,rdi) | label(X,Z,rfi) | label(X,Z,ro) :- label(X,Y,roi), label(Y,Z,rm).
% roi o rmi = r> 
label(X,Z,rpi) :- label(X,Y,roi), label(Y,Z,rmi).
% roi o ro = r= d di s si f fi o oi 
label(X,Z,req) | label(X,Z,rd) | label(X,Z,rdi) | label(X,Z,rs) | label(X,Z,rsi) | label(X,Z,rf) | label(X,Z,rfi) | label(X,Z,ro) | label(X,Z,roi) :- label(X,Y,roi), label(Y,Z,ro).
% roi o roi = r> mi oi 
label(X,Z,rpi) | label(X,Z,rmi) | label(X,Z,roi) :- label(X,Y,roi), label(Y,Z,roi).

"""
output = """
{label(0,1,rpi), label(0,2,rmi), label(1,2,rd), lc(0,1,rEC), lc(0,1,rEQ), lc(0,1,rNTPP), lc(0,1,rNTPPI), lc(0,1,rPO), lc(0,1,rTPP), lc(0,1,rTPPI), lc(1,2,rDC), lc(1,2,rEC), lc(1,2,rEQ), lc(1,2,rNTPP), lc(1,2,rNTPPI), lc(1,2,rTPP), lc(1,2,rTPPI), node1(0), node1(1), node1(2), node2(0), node2(1), node2(2), rel(rd), rel(rdi), rel(req), rel(rf), rel(rfi), rel(rm), rel(rmi), rel(ro), rel(roi), rel(rp), rel(rpi), rel(rs), rel(rsi)}
{label(0,1,rpi), label(0,2,rf), label(1,2,rd), lc(0,1,rEC), lc(0,1,rEQ), lc(0,1,rNTPP), lc(0,1,rNTPPI), lc(0,1,rPO), lc(0,1,rTPP), lc(0,1,rTPPI), lc(1,2,rDC), lc(1,2,rEC), lc(1,2,rEQ), lc(1,2,rNTPP), lc(1,2,rNTPPI), lc(1,2,rTPP), lc(1,2,rTPPI), node1(0), node1(1), node1(2), node2(0), node2(1), node2(2), rel(rd), rel(rdi), rel(req), rel(rf), rel(rfi), rel(rm), rel(rmi), rel(ro), rel(roi), rel(rp), rel(rpi), rel(rs), rel(rsi)}
{label(0,1,rpi), label(0,2,rd), label(1,2,rd), lc(0,1,rEC), lc(0,1,rEQ), lc(0,1,rNTPP), lc(0,1,rNTPPI), lc(0,1,rPO), lc(0,1,rTPP), lc(0,1,rTPPI), lc(1,2,rDC), lc(1,2,rEC), lc(1,2,rEQ), lc(1,2,rNTPP), lc(1,2,rNTPPI), lc(1,2,rTPP), lc(1,2,rTPPI), node1(0), node1(1), node1(2), node2(0), node2(1), node2(2), rel(rd), rel(rdi), rel(req), rel(rf), rel(rfi), rel(rm), rel(rmi), rel(ro), rel(roi), rel(rp), rel(rpi), rel(rs), rel(rsi)}
{label(0,1,rpi), label(0,2,rpi), label(1,2,rd), lc(0,1,rEC), lc(0,1,rEQ), lc(0,1,rNTPP), lc(0,1,rNTPPI), lc(0,1,rPO), lc(0,1,rTPP), lc(0,1,rTPPI), lc(1,2,rDC), lc(1,2,rEC), lc(1,2,rEQ), lc(1,2,rNTPP), lc(1,2,rNTPPI), lc(1,2,rTPP), lc(1,2,rTPPI), node1(0), node1(1), node1(2), node2(0), node2(1), node2(2), rel(rd), rel(rdi), rel(req), rel(rf), rel(rfi), rel(rm), rel(rmi), rel(ro), rel(roi), rel(rp), rel(rpi), rel(rs), rel(rsi)}
{label(0,1,rpi), label(0,2,roi), label(1,2,rd), lc(0,1,rEC), lc(0,1,rEQ), lc(0,1,rNTPP), lc(0,1,rNTPPI), lc(0,1,rPO), lc(0,1,rTPP), lc(0,1,rTPPI), lc(1,2,rDC), lc(1,2,rEC), lc(1,2,rEQ), lc(1,2,rNTPP), lc(1,2,rNTPPI), lc(1,2,rTPP), lc(1,2,rTPPI), node1(0), node1(1), node1(2), node2(0), node2(1), node2(2), rel(rd), rel(rdi), rel(req), rel(rf), rel(rfi), rel(rm), rel(rmi), rel(ro), rel(roi), rel(rp), rel(rpi), rel(rs), rel(rsi)}
"""
