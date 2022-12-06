input = """
husband_of(greco,pugliese).
husband_of(pietro,famularo).
husband_of(luigi,nik).
husband_of(franze,salvo).
husband_of(faber,tiano).
husband_of(girsch,ursino).
husband_of(salerno,perri).

husband_of(havelka,stary).
husband_of(eiter,seyr).
husband_of(gottlob,laura).
husband_of(herzog,koch).
husband_of(veniva,peratel).
husband_of(leone,scarcell).
husband_of(citrigno,francesco).
husband_of(mainieri,sali).
husband_of(jme,pfeifer).


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

father_of(havelka,gottlob).
father_of(havelka,herzog).
father_of(havelka,veniva).
father_of(eiter,peratel).
father_of(eiter,koch).
father_of(eiter,laura).
father_of(eiter,leone).
father_of(gottlob,citrigno).
father_of(gottlob,mainieri).
father_of(herzog,jme).
father_of(veniva,ehritz).
father_of(leone,pfeifer).
father_of(leone,sali).
father_of(leone,francesco).
father_of(citrigno,giulio).
father_of(citrigno,daniele).
father_of(mainieri,stefano).
father_of(jme,gerald).

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
{father_of(citrigno,daniele), father_of(citrigno,giulio), father_of(dorn,havelka), father_of(dorn,stary), father_of(eiter,koch), father_of(eiter,laura), father_of(eiter,leone), father_of(eiter,peratel), father_of(frank,salvo), father_of(frank,tiano), father_of(franze,dorn), father_of(gottlob,citrigno), father_of(gottlob,mainieri), father_of(greco,sergio), father_of(havelka,gottlob), father_of(havelka,herzog), father_of(havelka,veniva), father_of(herzog,jme), father_of(jme,gerald), father_of(leone,francesco), father_of(leone,pfeifer), father_of(leone,sali), father_of(luigi,palopoli), father_of(luigi,rullo), father_of(mainieri,stefano), father_of(nigro,faber), father_of(nigro,girsch), father_of(pietro,frank), father_of(pupo,ursino), father_of(sacca,franze), father_of(veniva,ehritz), husband_of(citrigno,francesco), husband_of(eiter,seyr), husband_of(faber,tiano), husband_of(franze,salvo), husband_of(girsch,ursino), husband_of(gottlob,laura), husband_of(greco,pugliese), husband_of(havelka,stary), husband_of(herzog,koch), husband_of(jme,pfeifer), husband_of(leone,scarcell), husband_of(luigi,nik), husband_of(mainieri,sali), husband_of(pietro,famularo), husband_of(salerno,perri), husband_of(veniva,peratel), mother_of(bucca,perri), mother_of(nik,pupo), mother_of(perri,reith), mother_of(pugliese,nigro), mother_of(reith,eiter), mother_of(rullo,salerno), mother_of(tiano,milos), mother_of(ursino,stadler), wife_of(bucca,pupo), wife_of(palopoli,nigro), wife_of(reith,milos), wife_of(rullo,frank), wife_of(sergio,sacca), wife_of(stadler,dorn)}
"""
