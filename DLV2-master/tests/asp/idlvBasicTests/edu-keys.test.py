input = """
teacher :- class.
class   :- hour, room.
room    :- hour, teacher.
room    :- hour, student.

key_found :- teacher, class, hour, room, student.


"""
output = """
{}
"""
