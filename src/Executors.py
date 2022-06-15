import subprocess,os,sys

class ExternalCalls:
    # Suggested pipeline
    # gringo program.lp | lpstrip | lp2normal2 | lpcat | lp2acyc | lp2sat | lingeling

    # Currently used pipeline 
    # If -d is enabled
    #   gringo -o smodels program.lp | lpshift | lp2normal2 -e | lp2sat
    # Otherwise 
    # gringo -o smodels program.lp | lp2normal2 -e | lp2sat

    asp_parser=os.path.join(sys._MEIPASS,"resources","asp-parser","asp_parser")
    toolFold=os.path.join(sys._MEIPASS,"resources","old-tool")
    quabs=os.path.join(sys._MEIPASS,"resources","qbf_solvers","quabs")
    gringo=os.path.join(sys._MEIPASS,toolFold,"gringo")
    lpshift=os.path.join(sys._MEIPASS,toolFold,"lpshift-1.4")
    lp2normal=os.path.join(sys._MEIPASS,toolFold,"lp2normal-2.27")
    lp2sat=os.path.join(sys._MEIPASS,toolFold,"lp2sat-1.24")
    qbf_solvers={"quabs":[quabs]}

    def callProgramParser(program):
        return subprocess.Popen([f"{ExternalCalls.asp_parser}",program],stdout=subprocess.PIPE).stdout

    def callGringoLp2Sat(filename,uselpshift=False):
        gringo = subprocess.Popen([f"{ExternalCalls.gringo}","-o","smodels",filename],stdout=subprocess.PIPE)
        lpshift = None
        if uselpshift:
            lpshift = subprocess.Popen([f"{ExternalCalls.lpshift}"],stdin=gringo.stdout,stdout=subprocess.PIPE)
        lp2normal = subprocess.Popen([f"{ExternalCalls.lp2normal}","-e"],stdin=lpshift.stdout if uselpshift  else gringo.stdout,stdout=subprocess.PIPE)
        lp2sat = subprocess.Popen([f"{ExternalCalls.lp2sat}"],stdin=lp2normal.stdout,stdout=subprocess.PIPE)
        return lp2sat.stdout
    def callFileAppend(source,destination):
        subprocess.getoutput(f"cat {source} >> {destination}")

    def callSolver(qcirFilename,solver):
        if solver in ExternalCalls.qbf_solvers:
            return subprocess.Popen(ExternalCalls.qbf_solvers[solver]+[qcirFilename],stdout=subprocess.PIPE).stdout
        print(f"Unable to find solver {solver}")
        return None