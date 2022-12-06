input = """
aF1_melts | aF2_melts :- aB_high.
aF1_melts | aF2_melts :- aR1_low, aR2_low.
aLight_off :- aF1_melts.
aLight_off :- aF2_melts.
aLight_off :- aBulb_broken.
"""
output = """
{}
"""
