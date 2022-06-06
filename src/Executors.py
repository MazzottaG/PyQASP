import subprocess,os,sys

class ExternalCalls:
    # Suggested pipeline
    # gringo program.lp | lpstrip | lp2normal2 | lpcat | lp2acyc | lp2sat | lingeling

    # Old pipeline 
    # gringo program.lp | lpshift | lp2normal2 | lp2lp2 | lp2sat

    asp_parser=os.path.join(sys._MEIPASS,"resources","asp-parser","main")
    toolFold=os.path.join(sys._MEIPASS,"resources","old-tool")
    quabs=os.path.join(sys._MEIPASS,"resources","qbf_solvers","quabs")
    lpshift=os.path.join(sys._MEIPASS,toolFold,"lpshift-1.4")
    lp2normal=os.path.join(sys._MEIPASS,toolFold,"lp2normal-2.27")
    lp2lp2=os.path.join(sys._MEIPASS,toolFold,"lp2lp2-1.23")
    lp2sat=os.path.join(sys._MEIPASS,toolFold,"lp2sat-1.24")

    def callProgramParser(program):
        return subprocess.Popen([f"{ExternalCalls.asp_parser}",program],stdout=subprocess.PIPE).stdout

    def callGringoLp2Sat(filename,uselpshift=False):
        gringo = subprocess.Popen(["gringo","-o","smodels",filename],stdout=subprocess.PIPE)
        lpshift = None
        if uselpshift:
            lpshift = subprocess.Popen([f"{ExternalCalls.lpshift}"],stdin=gringo.stdout,stdout=subprocess.PIPE)
        lp2normal = subprocess.Popen([f"{ExternalCalls.lp2normal}"],stdin=lpshift.stdout if uselpshift else gringo.stdout,stdout=subprocess.PIPE)
        lp2lp2 = subprocess.Popen([f"{ExternalCalls.lp2lp2}"],stdin=lp2normal.stdout,stdout=subprocess.PIPE)
        lp2sat = subprocess.Popen([f"{ExternalCalls.lp2sat}"],stdin=lp2lp2.stdout,stdout=subprocess.PIPE)
        return lp2sat.stdout
    def callFileAppend(source,destination):
        # subprocess.getoutput(f"cat {source} >> {destination}")
        f1 = open(destination, 'a+')
        f2 = open(source, 'r')
        f1.write(f2.read())
        f1.close()
        f2.close()

    def callQuabs(qcirFilename):
        print(f"Running quabs on {qcirFilename}")
        gringo = subprocess.Popen([ExternalCalls.quabs,qcirFilename])
        