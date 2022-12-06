input = """
oraInizio(8).
oraInizio(9).
oraInizio(10).
oraInizio(11).
oraInizio(12).

precede(H,H1) :- oraInizio(H), oraInizio(H1), H < H1.
precede(G,G1) :- giornoSuccessivo(G,G1).
precede(G,G1) :- giornoSuccessivo(G,G2), precede(G2,G1).
giornoSuccessivo(lun,mar).
giornoSuccessivo(mar,mer).
giornoSuccessivo(mer,gio)."""
output = """
{giornoSuccessivo(lun,mar), giornoSuccessivo(mar,mer), giornoSuccessivo(mer,gio), oraInizio(10), oraInizio(11), oraInizio(12), oraInizio(8), oraInizio(9), precede(10,11), precede(10,12), precede(11,12), precede(8,10), precede(8,11), precede(8,12), precede(8,9), precede(9,10), precede(9,11), precede(9,12), precede(lun,gio), precede(lun,mar), precede(lun,mer), precede(mar,gio), precede(mar,mer), precede(mer,gio)}
"""
