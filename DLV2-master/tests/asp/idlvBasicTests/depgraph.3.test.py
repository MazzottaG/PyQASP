input = """
% As of 2000-06-18 this generates empty components in some DEPGRAPHs.

c :- not b.                                                                     
a :- c.                                                                         
b :- a.                                                                         
a :- true.                                                                      
"""
output = """
INCOHERENT
"""
