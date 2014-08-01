#!/urs/bin/env python

#
# Only support .cpp files
#

import sys
import subprocess
from subprocess import call
import os
import glob

GCC = "g++ -O2 -std=gnu++0x -Wall -Wshadow "

def print_usage():

    print "Usage:"
    print "python ping128-tester.py filename                -- expected filename.in"
    print "python ping128-tester.py filename compare        -- expected filename.in filename.ans"
    print "python ping128-tester.py filename path_to_folder input_file_format output_file_format"

def run(arg, testfile="", ansfile="", first=True):
    good = True
    try:
        filename = arg + ".cpp"
        actualFileName = os.path.splitext(filename)[0]
        if testfile == "":
            print colored("Testing: " + filename, "blue")
            inputFile = actualFileName + ".in"
            outputFile = actualFileName + ".ans"
        else:
            print colored("Testing: " + os.path.basename(testfile), "blue")
            inputFile = testfile
            outputFile = ansfile
        if first:
            subprocess.call(GCC + filename, shell = True)
        subprocess.call("./a.out < " + inputFile + " > temp_", shell = True)
        subprocess.call("diff -y --suppress-common-lines temp_ " + outputFile, shell = True)
        subprocess.check_output("diff -y --suppress-common-lines temp_ " + outputFile, shell = True)
        subprocess.call("rm temp_", shell = True)
    except Exception, e:
        good = False
    if good:
        print colored("Accepted", "green")
    else:
        print colored("Wrong Answer", "red")

if __name__ == '__main__':

    try:
        from termcolor import colored
    except ImportError, e:
        print "Need to install termcolor to see colored outputs"

    if len(sys.argv) == 2:
        filename = sys.argv[1] + ".cpp"
        actualFileName = os.path.splitext(filename)[0]
        print colored("Testing: " + filename, "blue")
        subprocess.call(GCC + filename, shell = True)
        print colored("Output:", "blue")
        subprocess.call("./a.out < " + actualFileName + ".in", shell = True)
    elif len(sys.argv) == 3 and sys.argv[2] == "compare":
        run(sys.argv[1])
    elif len(sys.argv) == 5:
        filename = sys.argv[1] + ".cpp"
        actualFileName = os.path.splitext(filename)[0]
        path = sys.argv[2]
        inputFileFormat = sys.argv[3]
        outputFileFormat = sys.argv[4]
        s = os.path.abspath(path) + "/*" + inputFileFormat
        tests = glob.glob(s)
        s = os.path.abspath(path) + "/*" + outputFileFormat
        ans = glob.glob(s)
        for i in range(0, len(tests)):
            if i > 0:
                run(sys.argv[1], tests[i], ans[i])
            else:
                run(sys.argv[1], tests[i], ans[i], False)
    else:
        print colored("Unknown Command", "red")
        print_usage()


