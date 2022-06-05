import subprocess

class ExternalCalls:
    # Suggested pipeline
    # gringo program.lp | lpstrip | lp2normal2 | lpcat | lp2acyc | lp2sat | lingeling

    # Old pipeline 
    # gringo program.lp | lpshift | lp2normal2 | lp2lp2 | lp2sat

    asp_parser="../resources/asp-parser/main"
    toolFold="../resources/old-tool"
    lpshift=toolFold+"/lpshift-1.4"
    lp2normal=toolFold+"/lp2normal-2.27"
    lp2lp2=toolFold+"/lp2lp2-1.23"
    lp2sat=toolFold+"/lp2sat-1.24"

    def callProgramParser(program):
        return subprocess.Popen([f"./{ExternalCalls.asp_parser}",program],stdout=subprocess.PIPE).stdout

    def callGringoLp2Sat(filename,uselpshift=False):
        gringo = subprocess.Popen(["gringo","-o","smodels",filename],stdout=subprocess.PIPE)
        lpshift = None
        if uselpshift:
            lpshift = subprocess.Popen([f"./{ExternalCalls.lpshift}"],stdin=gringo.stdout,stdout=subprocess.PIPE)
        lp2normal = subprocess.Popen([f"./{ExternalCalls.lp2normal}"],stdin=lpshift.stdout if uselpshift else gringo.stdout,stdout=subprocess.PIPE)
        lp2lp2 = subprocess.Popen([f"./{ExternalCalls.lp2lp2}"],stdin=lp2normal.stdout,stdout=subprocess.PIPE)
        lp2sat = subprocess.Popen([f"./{ExternalCalls.lp2sat}"],stdin=lp2lp2.stdout,stdout=subprocess.PIPE)
        return lp2sat.stdout
    def callFileAppend(source,destination):
        subprocess.getoutput(f"cat {source} >> {destination}")