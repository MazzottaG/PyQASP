input="""
a("&%££$").a("dsf!//").a(fesla).a(ajjs).a("0123").a(d01).
b(Z,W):-&append_str(X,Y;Z),&length_str(Z;W),a(X),a(Y).
c(L,U,P,Y):-b(Y,X),&sub_str(L,U,Y;G),a(P),&to_qstr(P;G),W=X-1,&int(W/2,X;L),&int(W/2,X;U).
sub(D,X):-a(D),&sub_str(1,4,D;X).
f(X):-&int(1,3;X).
mem(X,S):-f(X),&member_str(X,S;),a(S).
"""
output="""
{a("&%££$"), a("dsf!//"), a(fesla), a(ajjs), a("0123"), a(d01), b("&%££$&%££$",10), b("&%££$dsf!//",11), b("&%££$fesla",10), b("&%££$ajjs",9), b("&%££$0123",9), b("&%££$d01",8), b("dsf!//&%££$",11), b("dsf!//dsf!//",12), b("dsf!//fesla",11), b("dsf!//ajjs",10), b("dsf!//0123",10), b("dsf!//d01",9), b("fesla&%££$",10), b("fesladsf!//",11), b(feslafesla,10), b(feslaajjs,9), b("fesla0123",9), b(feslad01,8), b("ajjs&%££$",9), b("ajjsdsf!//",10), b(ajjsfesla,9), b(ajjsajjs,8), b("ajjs0123",8), b(ajjsd01,7), b("0123&%££$",9), b("0123dsf!//",10), b("0123fesla",9), b("0123ajjs",8), b("01230123",8), b("0123d01",7), b("d01&%££$",8), b("d01dsf!//",9), b(d01fesla,8), b(d01ajjs,7), b("d010123",7), b(d01d01,6), c(6,10,"&%££$","&%££$&%££$"), c(6,11,"dsf!//","&%££$dsf!//"), c(6,10,fesla,"&%££$fesla"), c(6,9,ajjs,"&%££$ajjs"), c(6,9,"0123","&%££$0123"), c(6,8,d01,"&%££$d01"), c(7,11,"&%££$","dsf!//&%££$"), c(7,12,"dsf!//","dsf!//dsf!//"), c(7,11,fesla,"dsf!//fesla"), c(7,10,ajjs,"dsf!//ajjs"), c(7,10,"0123","dsf!//0123"), c(7,9,d01,"dsf!//d01"), c(6,10,"&%££$","fesla&%££$"), c(6,11,"dsf!//","fesladsf!//"), c(6,10,fesla,feslafesla), c(6,9,ajjs,feslaajjs), c(6,9,"0123","fesla0123"), c(6,8,d01,feslad01), c(5,9,"&%££$","ajjs&%££$"), c(5,10,"dsf!//","ajjsdsf!//"), c(5,9,fesla,ajjsfesla), c(5,8,ajjs,ajjsajjs), c(5,8,"0123","ajjs0123"), c(5,7,d01,ajjsd01), c(5,9,"&%££$","0123&%££$"), c(5,10,"dsf!//","0123dsf!//"), c(5,9,fesla,"0123fesla"), c(5,8,ajjs,"0123ajjs"), c(5,8,"0123","01230123"), c(5,7,d01,"0123d01"), c(4,8,"&%££$","d01&%££$"), c(4,9,"dsf!//","d01dsf!//"), c(4,8,fesla,d01fesla), c(4,7,ajjs,d01ajjs), c(4,7,"0123","d010123"), c(4,6,d01,d01d01), sub("&%££$","&%££"), sub("dsf!//","dsf!"), sub(fesla,"fesl"), sub(ajjs,"ajjs"), sub("0123","0123"), f(1), f(2), f(3), mem(1,"0123"), mem(1,d01), mem(2,"0123"), mem(3,"0123")}
"""
