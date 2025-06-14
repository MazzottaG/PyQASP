import subprocess,os,sys
import logging
from Option import FILE_UTIL

class ExternalCalls:
    # Suggested pipeline
    # gringo program.lp | lpstrip | lp2normal2 | lpcat | lp2acyc | lp2sat | lingeling

    # Currently used pipeline 
    # If -d is enabled
    #   gringo -o smodels program.lp | lpshift | lp2normal2 -e | lp2sat
    # Otherwise 
    # gringo -o smodels program.lp | lp2normal2 -e | lp2sat
    LOG_FILE_HANDLER = None
    debugger = None
    debuggercmd = None

    
    def callDLV2MultipleInput(fileNames):
        cmd = [f"{FILE_UTIL.DLV2_PATH}"]+fileNames+["--pre=lparse"]
        return subprocess.Popen(cmd,stdout=subprocess.PIPE,stderr=ExternalCalls.LOG_FILE_HANDLER)

    def callDLV2(inputFilename):
        ExternalCalls.debugger.printMessage(" ".join([f"{FILE_UTIL.DLV2_PATH}",inputFilename,"--pre=lparse"]))
        return subprocess.Popen([f"{FILE_UTIL.DLV2_PATH}",inputFilename,"--pre=lparse"],stdout=subprocess.PIPE,stderr=ExternalCalls.LOG_FILE_HANDLER)

    def callIDLV(inputFilename):
        return subprocess.Popen([f"{FILE_UTIL.DLV2_PATH}",inputFilename,"--mode=idlv"],stdout=subprocess.PIPE,stderr=ExternalCalls.LOG_FILE_HANDLER)

    def callGringo(inputFilename):
        return subprocess.Popen([f"{FILE_UTIL.GRINGO_PATH}",inputFilename,"-o","smodels"],stdout=subprocess.PIPE,stderr=ExternalCalls.LOG_FILE_HANDLER)

    def callProgramParser(filename):
        return subprocess.Popen([f"{FILE_UTIL.ASP_PARSER_PATH}",filename], stdout=subprocess.PIPE,stderr=subprocess.PIPE)

    def callOldPipeline(filename,uselpshift=False):
        if uselpshift:
            logging.info(f"\tUsing Lp2Shifth")
            lpshift = subprocess.Popen([f"{FILE_UTIL.LPSHIFT_PATH}",filename],stdout=subprocess.PIPE,stderr=ExternalCalls.LOG_FILE_HANDLER)
            lp2normal = subprocess.Popen([f"{FILE_UTIL.LP2NORMAL_PATH}","-e"],stdin=lpshift.stdout,stdout=subprocess.PIPE,stderr=ExternalCalls.LOG_FILE_HANDLER)
            lp2sat = subprocess.Popen([f"{FILE_UTIL.LP2SAT_PATH}"],stdin=lp2normal.stdout,stdout=subprocess.PIPE,stderr=ExternalCalls.LOG_FILE_HANDLER)
            return lp2sat.stdout
        else:
            logging.info(f"\tNo Lp2Shifth")
            lp2normal = subprocess.Popen([f"{FILE_UTIL.LP2NORMAL_PATH}","-e",filename],stdout=subprocess.PIPE,stderr=ExternalCalls.LOG_FILE_HANDLER)
            lp2sat = subprocess.Popen([f"{FILE_UTIL.LP2SAT_PATH}"],stdin=lp2normal.stdout,stdout=subprocess.PIPE,stderr=ExternalCalls.LOG_FILE_HANDLER)
            return lp2sat.stdout
    
    def callPipeline(filename,uselpshift=False,tight=False):
        lpshift   = None
        lp2normal = None
        lp2acyc   = None
        lp2sat    = None
        
        if uselpshift:
            ExternalCalls.debugger.printMessage("lpshift filename | lp2normal -e")
            logging.info(f"\tUsing Lp2Shifth")
            lpshift = subprocess.Popen([f"{FILE_UTIL.LPSHIFT_PATH}",filename],stdout=subprocess.PIPE,stderr=ExternalCalls.LOG_FILE_HANDLER)
            lp2normal = subprocess.Popen([f"{FILE_UTIL.LP2NORMAL_PATH}","-e"],stdin=lpshift.stdout,stdout=subprocess.PIPE,stderr=ExternalCalls.LOG_FILE_HANDLER)
        else:
            ExternalCalls.debugger.printMessage("lp2normal -e filename")
            logging.info(f"\tNo Lp2Shifth")
            lp2normal = subprocess.Popen([f"{FILE_UTIL.LP2NORMAL_PATH}","-e",filename],stdout=subprocess.PIPE,stderr=ExternalCalls.LOG_FILE_HANDLER)
        if tight:
            ExternalCalls.debugger.printMessage(" | lp2sat -b")
            logging.info(f"\tNo Lp2atomic")
            lp2sat = subprocess.Popen([f"{FILE_UTIL.LP2SAT_PATH}","-b"],stdin=lp2normal.stdout,stdout=subprocess.PIPE,stderr=ExternalCalls.LOG_FILE_HANDLER)
            #lp2sat = subprocess.Popen([f"{FILE_UTIL.LP2SAT_PATH}"],stdin=lp2normal.stdout,stdout=subprocess.PIPE,stderr=ExternalCalls.LOG_FILE_HANDLER)
        else:
            ExternalCalls.debugger.printMessage(" | lp2atomic | lp2sat -b")
            logging.info(f"\tUsing Lp2Acyc")
            lp2acyc = subprocess.Popen([f"{FILE_UTIL.LP2ATOMIC_PATH}"],stdin=lp2normal.stdout,stdout=subprocess.PIPE,stderr=ExternalCalls.LOG_FILE_HANDLER)
            lp2sat = subprocess.Popen([f"{FILE_UTIL.LP2SAT_PATH}","-b"],stdin=lp2acyc.stdout,stdout=subprocess.PIPE,stderr=ExternalCalls.LOG_FILE_HANDLER)
            #lp2sat = subprocess.Popen([f"{FILE_UTIL.LP2SAT_PATH}"],stdin=lp2acyc.stdout,stdout=subprocess.PIPE,stderr=ExternalCalls.LOG_FILE_HANDLER)

        return lp2sat

    def callFileAppend(source,destination):
        subprocess.getoutput(f"cat {source} >> {destination}")
    
    def callFileCopy(source,destination):
        subprocess.getoutput(f"cp {source} {destination}")

    def callSolver(cmd:list):
        return subprocess.Popen(cmd,stdout=subprocess.PIPE,stderr=subprocess.PIPE)

    def callSolverPipeline(cmds: list):
        steps = []
        # print("Executing: "," | ".join([" ".join(cmd) for cmd in cmds]))
        for cmd in cmds:
            currentProc = None
            if len(steps) > 0:
                currentProc = subprocess.Popen(cmd,stdin=steps[-1].stdout,stdout=subprocess.PIPE,stderr=ExternalCalls.LOG_FILE_HANDLER)
            else:
                currentProc = subprocess.Popen(cmd,stdout=subprocess.PIPE,stderr=ExternalCalls.LOG_FILE_HANDLER)
            steps.append(currentProc)
        
        if len(steps) <= 0:
            print("Error: empty solving pipeline")
            sys.exit(180)

        return steps
