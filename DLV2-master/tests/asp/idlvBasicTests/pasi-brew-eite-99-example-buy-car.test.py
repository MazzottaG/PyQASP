input = """

% Facts which are not disputable are declared as heads of 'top'

rule(top).

rule(r11). 
rule(r12). 
rule(r13). 

rule(r21).
rule(r22).
rule(r23).
rule(r24).
rule(r25).
rule(r26).
rule(r27).
rule(r28).
rule(r29).

rule(r31).
rule(r32).
rule(r33).
rule(r34).
rule(r35).
rule(r36).
rule(r37).
rule(r38).
rule(r39).

rule(r41).
rule(r42).
rule(r43).
rule(r44).
rule(r45).
rule(r46).
rule(r47).
rule(r48).
rule(r49).

head(expensive_C,top). 
head(safe_C,top).
head(safe_V,top).
head(nice_P,top).
head(fast_P,top). 


head(neg_buy_C,r11). nbl(buy_C,r11). pbl(expensive_C,r11).  
head(neg_buy_V,r12). nbl(buy_V,r12). pbl(expensive_V,r12).  
head(neg_buy_P,r13). nbl(buy_P,r13). pbl(expensive_P,r13).  

head(buy_C,r21). nbl(neg_buy_C,r21). pbl(safe_C,r21).  
head(buy_V,r22). nbl(neg_buy_V,r22). pbl(safe_V,r22).  
head(buy_P,r23). nbl(neg_buy_P,r23). pbl(safe_P,r23).  

head(neg_buy_C,r24). pbl(buy_V,r24). pbl(safe_V,r24).  
head(neg_buy_C,r27). pbl(buy_P,r27). pbl(safe_P,r27).  
head(neg_buy_V,r25). pbl(buy_P,r25). pbl(safe_P,r25).  
head(neg_buy_V,r28). pbl(buy_C,r28). pbl(safe_C,r28).  
head(neg_buy_P,r26). pbl(buy_C,r26). pbl(safe_C,r26).  
head(neg_buy_P,r29). pbl(buy_V,r29). pbl(safe_V,r29).  


head(buy_C,r31). nbl(neg_buy_C,r31). pbl(nice_C,r31).  
head(buy_V,r32). nbl(neg_buy_V,r32). pbl(nice_V,r32).  
head(buy_P,r33). nbl(neg_buy_P,r33). pbl(nice_P,r33).  

head(neg_buy_C,r34). pbl(buy_V,r34). pbl(nice_V,r34).  
head(neg_buy_C,r37). pbl(buy_P,r37). pbl(nice_P,r37).  
head(neg_buy_V,r35). pbl(buy_P,r35). pbl(nice_P,r35).  
head(neg_buy_V,r38). pbl(buy_C,r38). pbl(nice_C,r38).  
head(neg_buy_P,r36). pbl(buy_C,r36). pbl(nice_C,r36).  
head(neg_buy_P,r39). pbl(buy_V,r39). pbl(nice_V,r39).  

head(buy_C,r41). nbl(neg_buy_C,r41). pbl(fast_C,r41).  
head(buy_V,r42). nbl(neg_buy_V,r42). pbl(fast_V,r42).  
head(buy_P,r43). nbl(neg_buy_P,r43). pbl(fast_P,r43).  

head(neg_buy_C,r44). pbl(buy_V,r44). pbl(fast_V,r44).  
head(neg_buy_C,r47). pbl(buy_P,r47). pbl(fast_P,r47).  
head(neg_buy_P,r46). pbl(buy_C,r46). pbl(fast_C,r46).  
head(neg_buy_P,r49). pbl(buy_V,r49). pbl(fast_V,r49).  
head(neg_buy_V,r45). pbl(buy_P,r45). pbl(fast_P,r45).  
head(neg_buy_V,r48). pbl(buy_C,r48). pbl(fast_C,r48).  

opp(buy_C,neg_buy_C). 
opp(buy_V,neg_buy_V). 
opp(buy_P,neg_buy_P). 


% define preferences 

level(0,top).

level(1,r11). 
level(1,r12). 
level(1,r13). 

level(2,r21).
level(2,r22).
level(2,r23).
level(2,r24).
level(2,r25).
level(2,r26).
level(2,r27).
level(2,r28).
level(2,r29).

level(3,r31).
level(3,r32).
level(3,r33).
level(3,r34).
level(3,r35).
level(3,r36).
level(3,r37).
level(3,r38).
level(3,r39).

level(4,r41).
level(4,r42).
level(4,r43).
level(4,r44).
level(4,r45).
level(4,r46).
level(4,r47).
level(4,r48).
level(4,r49).

kl(0,1). kl(1,2). kl(2,3). kl(3,4).

pr(X,Y) :- kl(L1,L2), level(L1,X),level(L2,Y).
"""
output = """
{head(buy_C,r21), head(buy_C,r31), head(buy_C,r41), head(buy_P,r23), head(buy_P,r33), head(buy_P,r43), head(buy_V,r22), head(buy_V,r32), head(buy_V,r42), head(expensive_C,top), head(fast_P,top), head(neg_buy_C,r11), head(neg_buy_C,r24), head(neg_buy_C,r27), head(neg_buy_C,r34), head(neg_buy_C,r37), head(neg_buy_C,r44), head(neg_buy_C,r47), head(neg_buy_P,r13), head(neg_buy_P,r26), head(neg_buy_P,r29), head(neg_buy_P,r36), head(neg_buy_P,r39), head(neg_buy_P,r46), head(neg_buy_P,r49), head(neg_buy_V,r12), head(neg_buy_V,r25), head(neg_buy_V,r28), head(neg_buy_V,r35), head(neg_buy_V,r38), head(neg_buy_V,r45), head(neg_buy_V,r48), head(nice_P,top), head(safe_C,top), head(safe_V,top), kl(0,1), kl(1,2), kl(2,3), kl(3,4), level(0,top), level(1,r11), level(1,r12), level(1,r13), level(2,r21), level(2,r22), level(2,r23), level(2,r24), level(2,r25), level(2,r26), level(2,r27), level(2,r28), level(2,r29), level(3,r31), level(3,r32), level(3,r33), level(3,r34), level(3,r35), level(3,r36), level(3,r37), level(3,r38), level(3,r39), level(4,r41), level(4,r42), level(4,r43), level(4,r44), level(4,r45), level(4,r46), level(4,r47), level(4,r48), level(4,r49), nbl(buy_C,r11), nbl(buy_P,r13), nbl(buy_V,r12), nbl(neg_buy_C,r21), nbl(neg_buy_C,r31), nbl(neg_buy_C,r41), nbl(neg_buy_P,r23), nbl(neg_buy_P,r33), nbl(neg_buy_P,r43), nbl(neg_buy_V,r22), nbl(neg_buy_V,r32), nbl(neg_buy_V,r42), opp(buy_C,neg_buy_C), opp(buy_P,neg_buy_P), opp(buy_V,neg_buy_V), pbl(buy_C,r26), pbl(buy_C,r28), pbl(buy_C,r36), pbl(buy_C,r38), pbl(buy_C,r46), pbl(buy_C,r48), pbl(buy_P,r25), pbl(buy_P,r27), pbl(buy_P,r35), pbl(buy_P,r37), pbl(buy_P,r45), pbl(buy_P,r47), pbl(buy_V,r24), pbl(buy_V,r29), pbl(buy_V,r34), pbl(buy_V,r39), pbl(buy_V,r44), pbl(buy_V,r49), pbl(expensive_C,r11), pbl(expensive_P,r13), pbl(expensive_V,r12), pbl(fast_C,r41), pbl(fast_C,r46), pbl(fast_C,r48), pbl(fast_P,r43), pbl(fast_P,r45), pbl(fast_P,r47), pbl(fast_V,r42), pbl(fast_V,r44), pbl(fast_V,r49), pbl(nice_C,r31), pbl(nice_C,r36), pbl(nice_C,r38), pbl(nice_P,r33), pbl(nice_P,r35), pbl(nice_P,r37), pbl(nice_V,r32), pbl(nice_V,r34), pbl(nice_V,r39), pbl(safe_C,r21), pbl(safe_C,r26), pbl(safe_C,r28), pbl(safe_P,r23), pbl(safe_P,r25), pbl(safe_P,r27), pbl(safe_V,r22), pbl(safe_V,r24), pbl(safe_V,r29), pr(r11,r21), pr(r11,r22), pr(r11,r23), pr(r11,r24), pr(r11,r25), pr(r11,r26), pr(r11,r27), pr(r11,r28), pr(r11,r29), pr(r12,r21), pr(r12,r22), pr(r12,r23), pr(r12,r24), pr(r12,r25), pr(r12,r26), pr(r12,r27), pr(r12,r28), pr(r12,r29), pr(r13,r21), pr(r13,r22), pr(r13,r23), pr(r13,r24), pr(r13,r25), pr(r13,r26), pr(r13,r27), pr(r13,r28), pr(r13,r29), pr(r21,r31), pr(r21,r32), pr(r21,r33), pr(r21,r34), pr(r21,r35), pr(r21,r36), pr(r21,r37), pr(r21,r38), pr(r21,r39), pr(r22,r31), pr(r22,r32), pr(r22,r33), pr(r22,r34), pr(r22,r35), pr(r22,r36), pr(r22,r37), pr(r22,r38), pr(r22,r39), pr(r23,r31), pr(r23,r32), pr(r23,r33), pr(r23,r34), pr(r23,r35), pr(r23,r36), pr(r23,r37), pr(r23,r38), pr(r23,r39), pr(r24,r31), pr(r24,r32), pr(r24,r33), pr(r24,r34), pr(r24,r35), pr(r24,r36), pr(r24,r37), pr(r24,r38), pr(r24,r39), pr(r25,r31), pr(r25,r32), pr(r25,r33), pr(r25,r34), pr(r25,r35), pr(r25,r36), pr(r25,r37), pr(r25,r38), pr(r25,r39), pr(r26,r31), pr(r26,r32), pr(r26,r33), pr(r26,r34), pr(r26,r35), pr(r26,r36), pr(r26,r37), pr(r26,r38), pr(r26,r39), pr(r27,r31), pr(r27,r32), pr(r27,r33), pr(r27,r34), pr(r27,r35), pr(r27,r36), pr(r27,r37), pr(r27,r38), pr(r27,r39), pr(r28,r31), pr(r28,r32), pr(r28,r33), pr(r28,r34), pr(r28,r35), pr(r28,r36), pr(r28,r37), pr(r28,r38), pr(r28,r39), pr(r29,r31), pr(r29,r32), pr(r29,r33), pr(r29,r34), pr(r29,r35), pr(r29,r36), pr(r29,r37), pr(r29,r38), pr(r29,r39), pr(r31,r41), pr(r31,r42), pr(r31,r43), pr(r31,r44), pr(r31,r45), pr(r31,r46), pr(r31,r47), pr(r31,r48), pr(r31,r49), pr(r32,r41), pr(r32,r42), pr(r32,r43), pr(r32,r44), pr(r32,r45), pr(r32,r46), pr(r32,r47), pr(r32,r48), pr(r32,r49), pr(r33,r41), pr(r33,r42), pr(r33,r43), pr(r33,r44), pr(r33,r45), pr(r33,r46), pr(r33,r47), pr(r33,r48), pr(r33,r49), pr(r34,r41), pr(r34,r42), pr(r34,r43), pr(r34,r44), pr(r34,r45), pr(r34,r46), pr(r34,r47), pr(r34,r48), pr(r34,r49), pr(r35,r41), pr(r35,r42), pr(r35,r43), pr(r35,r44), pr(r35,r45), pr(r35,r46), pr(r35,r47), pr(r35,r48), pr(r35,r49), pr(r36,r41), pr(r36,r42), pr(r36,r43), pr(r36,r44), pr(r36,r45), pr(r36,r46), pr(r36,r47), pr(r36,r48), pr(r36,r49), pr(r37,r41), pr(r37,r42), pr(r37,r43), pr(r37,r44), pr(r37,r45), pr(r37,r46), pr(r37,r47), pr(r37,r48), pr(r37,r49), pr(r38,r41), pr(r38,r42), pr(r38,r43), pr(r38,r44), pr(r38,r45), pr(r38,r46), pr(r38,r47), pr(r38,r48), pr(r38,r49), pr(r39,r41), pr(r39,r42), pr(r39,r43), pr(r39,r44), pr(r39,r45), pr(r39,r46), pr(r39,r47), pr(r39,r48), pr(r39,r49), pr(top,r11), pr(top,r12), pr(top,r13), rule(r11), rule(r12), rule(r13), rule(r21), rule(r22), rule(r23), rule(r24), rule(r25), rule(r26), rule(r27), rule(r28), rule(r29), rule(r31), rule(r32), rule(r33), rule(r34), rule(r35), rule(r36), rule(r37), rule(r38), rule(r39), rule(r41), rule(r42), rule(r43), rule(r44), rule(r45), rule(r46), rule(r47), rule(r48), rule(r49), rule(top)}
"""
