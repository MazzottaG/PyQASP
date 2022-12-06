input = """
parent_of(tom,carl).
parent_of(eve,carl).
parent_of(tom,ann).
parent_of(eve,ann).
parent_of(homer,tom).
parent_of(marla,tom).
parent_of(jeff,eve).
parent_of(susan,eve).
"""
output = """
{parent_of(eve,ann), parent_of(eve,carl), parent_of(homer,tom), parent_of(jeff,eve), parent_of(marla,tom), parent_of(susan,eve), parent_of(tom,ann), parent_of(tom,carl)}
"""
