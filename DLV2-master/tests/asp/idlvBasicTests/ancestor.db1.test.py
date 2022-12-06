input = """
husband_of(greco,pugliese).
husband_of(pietro,famularo).
husband_of(luigi,nik).
husband_of(franze,salvo).
husband_of(faber,tiano).
husband_of(girsch,ursino).
husband_of(salerno,perri).

wife_of(sergio,sacca).
wife_of(palopoli,nigro).
wife_of(rullo,frank).
wife_of(bucca,pupo).
wife_of(stadler,dorn).
wife_of(reith,milos).

father_of(greco,sergio).
father_of(pietro,frank).
father_of(luigi,rullo).
father_of(luigi,palopoli).
father_of(sacca,franze).
father_of(nigro,girsch).
father_of(nigro,faber).
father_of(frank,salvo).
father_of(frank,tiano).
father_of(pupo,ursino).
father_of(franze,dorn).
father_of(dorn,havelka).
father_of(dorn,stary).

mother_of(pugliese,nigro).
mother_of(nik,pupo).
mother_of(rullo,salerno).
mother_of(bucca,perri).
mother_of(tiano,milos).
mother_of(ursino,stadler).
mother_of(perri,reith).
mother_of(reith,eiter).
"""
output = """
{father_of(dorn,havelka), father_of(dorn,stary), father_of(frank,salvo), father_of(frank,tiano), father_of(franze,dorn), father_of(greco,sergio), father_of(luigi,palopoli), father_of(luigi,rullo), father_of(nigro,faber), father_of(nigro,girsch), father_of(pietro,frank), father_of(pupo,ursino), father_of(sacca,franze), husband_of(faber,tiano), husband_of(franze,salvo), husband_of(girsch,ursino), husband_of(greco,pugliese), husband_of(luigi,nik), husband_of(pietro,famularo), husband_of(salerno,perri), mother_of(bucca,perri), mother_of(nik,pupo), mother_of(perri,reith), mother_of(pugliese,nigro), mother_of(reith,eiter), mother_of(rullo,salerno), mother_of(tiano,milos), mother_of(ursino,stadler), wife_of(bucca,pupo), wife_of(palopoli,nigro), wife_of(reith,milos), wife_of(rullo,frank), wife_of(sergio,sacca), wife_of(stadler,dorn)}
"""
