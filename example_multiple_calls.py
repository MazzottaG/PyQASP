from pyqasp.pyqaspsolver import PyQASPSolver

def print_res(model,exit_code):
    if exit_code == 20:
        print("UNSAT")
        return
    if exit_code == 10:
        print("SAT:",model)
        return
    print("UNKNOWN")

solver = PyQASPSolver("example.adorned.aspq")
solver.ground()
model,exit_code = solver.solve([("o(1)",False),("o(2)",False)])
print("Enforce","{o(1),o(2)}")
print_res(model,exit_code)
model,exit_code = solver.solve([("o(1)",False),("o(2)",True)])
print("Enforce","{o(1)}")
print_res(model,exit_code)
model,exit_code = solver.solve([("o(1)",True),("o(2)",False)])
print("Enforce","{o(2)}")
print_res(model,exit_code)
model,exit_code = solver.solve([("o(1)",True),("o(2)",True)])
print("Enforce","{}")
print_res(model,exit_code)
