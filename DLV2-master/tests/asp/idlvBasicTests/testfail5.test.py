input = """
valves_number(3).
valves_per_pipe(2).
tank(1).
junction(1). pipe(1,2). pipe(1,15). dem(1,15,142). dem(1,2,116).
junction(2). pipe(2,3). pipe(2,14). dem(2,14,24). dem(2,3,24).
junction(3). pipe(3,7). pipe(3,4). pipe(3,9). dem(3,9,18). dem(3,7,32). dem(3,4,22).
junction(4). pipe(4,5). dem(4,5,26).
junction(5). pipe(5,6). dem(5,6,26).
junction(6). pipe(6,7). dem(6,7,36).
junction(7). pipe(7,8). dem(7,8,38).
junction(8). pipe(8,9). pipe(8,10). dem(8,10,28). dem(8,9,24).
junction(9).
junction(10). pipe(10,11). dem(10,11,42).
junction(11). pipe(11,12). pipe(11,18). dem(11,18,64). dem(11,12,64).
junction(12). pipe(12,13). pipe(12,17). pipe(12,19). dem(12,19,61). dem(12,17,94). dem(12,13,48).
junction(13). pipe(13,14). dem(13,14,22).
junction(14). pipe(14,15). dem(14,15,50).
junction(15). pipe(15,16). dem(15,16,91).
junction(16). pipe(16,26). pipe(16,17). dem(16,26,169). dem(16,17,109).
junction(17). pipe(17,25). dem(17,25,150).
junction(18). pipe(18,19). pipe(18,21). dem(18,21,73). dem(18,19,61).
junction(19). pipe(19,20). dem(19,20,61).
junction(20). pipe(20,24). dem(20,24,72).
junction(21). pipe(21,24). pipe(21,22). dem(21,24,75). dem(21,22,82).
junction(22). pipe(22,23). dem(22,23,97).
junction(23). pipe(23,24). dem(23,24,91).
junction(24). pipe(24,25). dem(24,25,129).
junction(25). pipe(25,26). dem(25,26,210).
junction(26).
"""
output = """
{dem(1,15,142), dem(1,2,116), dem(10,11,42), dem(11,12,64), dem(11,18,64), dem(12,13,48), dem(12,17,94), dem(12,19,61), dem(13,14,22), dem(14,15,50), dem(15,16,91), dem(16,17,109), dem(16,26,169), dem(17,25,150), dem(18,19,61), dem(18,21,73), dem(19,20,61), dem(2,14,24), dem(2,3,24), dem(20,24,72), dem(21,22,82), dem(21,24,75), dem(22,23,97), dem(23,24,91), dem(24,25,129), dem(25,26,210), dem(3,4,22), dem(3,7,32), dem(3,9,18), dem(4,5,26), dem(5,6,26), dem(6,7,36), dem(7,8,38), dem(8,10,28), dem(8,9,24), junction(1), junction(10), junction(11), junction(12), junction(13), junction(14), junction(15), junction(16), junction(17), junction(18), junction(19), junction(2), junction(20), junction(21), junction(22), junction(23), junction(24), junction(25), junction(26), junction(3), junction(4), junction(5), junction(6), junction(7), junction(8), junction(9), pipe(1,15), pipe(1,2), pipe(10,11), pipe(11,12), pipe(11,18), pipe(12,13), pipe(12,17), pipe(12,19), pipe(13,14), pipe(14,15), pipe(15,16), pipe(16,17), pipe(16,26), pipe(17,25), pipe(18,19), pipe(18,21), pipe(19,20), pipe(2,14), pipe(2,3), pipe(20,24), pipe(21,22), pipe(21,24), pipe(22,23), pipe(23,24), pipe(24,25), pipe(25,26), pipe(3,4), pipe(3,7), pipe(3,9), pipe(4,5), pipe(5,6), pipe(6,7), pipe(7,8), pipe(8,10), pipe(8,9), tank(1), valves_number(3), valves_per_pipe(2)}
"""
