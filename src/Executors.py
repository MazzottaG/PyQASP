import subprocess,os,sys
import logging
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
    error_log_file=subprocess.DEVNULL
    quabs=os.path.join(sys._MEIPASS,"resources","qbf_solvers","quabs")
    gringo=os.path.join(sys._MEIPASS,toolFold,"gringo")
    lpshift=os.path.join(sys._MEIPASS,toolFold,"lpshift-1.4")
    lp2normal=os.path.join(sys._MEIPASS,toolFold,"lp2normal-2.27")
    lp2sat=os.path.join(sys._MEIPASS,toolFold,"lp2sat-1.24")
    qbf_solvers={"quabs":[quabs]}
    solver_options={"quabs":["--partial-assignment"]}

    def callProgramParser(program):
        return subprocess.Popen([f"{ExternalCalls.asp_parser}",program],stdout=subprocess.PIPE,stderr=subprocess.STDOUT).stdout

    def callGringoLp2Sat(filename,uselpshift=False):
        log_handler = open(ExternalCalls.error_log_file,"w") if ExternalCalls.error_log_file != subprocess.DEVNULL else subprocess.DEVNULL
        logging.info(f"\tCalling pipeline for current program")
        
        gringo = subprocess.Popen([f"{ExternalCalls.gringo}","-o","smodels",filename],stdout=subprocess.PIPE,stderr=log_handler)
        lpshift = None
        if uselpshift:
            logging.info(f"\tUsing Lp2Shifth")
            #print("Using Lp2Shifth")
            lpshift = subprocess.Popen([f"{ExternalCalls.lpshift}"],stdin=gringo.stdout,stdout=subprocess.PIPE,stderr=log_handler)
        else:
            logging.info(f"\tNo Lp2Shifth")

        lp2normal = subprocess.Popen([f"{ExternalCalls.lp2normal}","-e"],stdin=lpshift.stdout if uselpshift  else gringo.stdout,stdout=subprocess.PIPE,stderr=log_handler)
        lp2sat = subprocess.Popen([f"{ExternalCalls.lp2sat}"],stdin=lp2normal.stdout,stdout=subprocess.PIPE,stderr=log_handler)
        if ExternalCalls.error_log_file != subprocess.DEVNULL:
            log_handler.close()
        return lp2sat.stdout
    def callFileAppend(source,destination):
        subprocess.getoutput(f"cat {source} >> {destination}")

    def callSolver(qcirFilename,solver):
        logging.info(f"Executing {solver} with following options {ExternalCalls.solver_options[solver]}")
        if solver in ExternalCalls.qbf_solvers:
            return subprocess.Popen(ExternalCalls.qbf_solvers[solver]+ExternalCalls.solver_options[solver]+[qcirFilename],stdout=subprocess.PIPE,stderr=subprocess.STDOUT).stdout
        print(f"Unable to find solver {solver}")
        return None