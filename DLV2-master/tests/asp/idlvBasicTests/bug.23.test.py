input = """
aMelted_fuse_1 | aMelted_fuse_2 :- aHigh_current.
aLight_off   :- aMelted_fuse_1, aMelted_fuse_2.
aLight_off   :- aPower_failure.
aLight_off   :- aBroken_bulb.
aBurns_plate_1  :- not aMelted_fuse_1, not aPower_failure.
aBurns_plate_2  :- not aMelted_fuse_2, not aPower_failure.
"""
output = """
{aBurns_plate_1, aBurns_plate_2}
"""
