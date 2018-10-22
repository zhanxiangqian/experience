#
# Copyright 2018 Shanghai Suchen Information Technology Ltd, DEMXS
#
# Virgo/                      <-- Virgo root folder
#   sources/
#       solver/
#           trunk/
#              input/         <-- test folder
#                 vg_runTests.py
#                 tests/      <-- all test cases
#   install/
#       win64/
#           bin/              <-- vg_solver.exe
#
# How to run:
# 1. cd solver source directory, then cd the test folder (sources/trunk/input)
#    this script will find all .vgi files in sub folders and run all
#    of them with the option "-test", each vgi contains a block for testing
#      <!-- for testing, manually added -->
#      <testing>
#         <steps>101</steps>
#	      <correlationFile>qa_node1.disp</correlationFile>
#      </testing>
#    this block tells the vg_solver to run testing for 101 steps, instead of the 
#    regular number of steps specified in the <analysis> block. it also specifies
#    a data file to compute cross correlation for the history of a node. this 
#    data file is an output from previous job and it should have been verified to
#    be correct
# 2. run command "python vg_runTests.py", this will actually run all test cases
# 3. check the report file (vg_solver_VERSION_DATE.X.csv)
#    VERSION is the solver's version, e.g. v1.20c-002 r122
#    DATE is the date this scrip is run, e.g. 2018-04-25
#    X is an integer starting from 1 to make sure existing report files are not overwritten
#    the output report file has 6 columns, they are
#    input_file, run time in seconds, correlation 1, ... correlation 4, e.g.
#          pRod_amp.vgi, 0.626, 1, 1, 1, 1, 1
#          cantilever.vgi, 0.502, 1, 1, 1, 1, 1
#    typically, a correlation value of 1 indicates a good match
#

import os, os.path, glob, fnmatch, subprocess, sys, difflib
import argparse
from datetime import datetime
from datetime import date

def read_file(filename):
    try:
	    with open(filename,'r') as f:
		    return f.readlines()
	#except IOError:
    #    print('read %s failed' %filename)

def compare_file(file1,file2):
    file1_content = read_file(file1)
    file2_content = read_file(file2)
	res = 0
	if file1_content == file2_content:
	   res = 1
	#d = difflib.Differ()
	#diff = d.compare(file1_content,file2_content)
	return res
	
def get_immediate_subdirectories(a_dir):
    return [name for name in os.listdir(a_dir)
            if os.path.isdir(os.path.join(a_dir, name))]

def find(pattern, path):
	result = []
	for root, dirs, files in os.walk(path):
		for name in files:
			if fnmatch.fnmatch(name, pattern):
				result.append(os.path.join(root, name))
	return result

def solver_version(virgoExe):	
	p = subprocess.Popen(virgoExe, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
	retval = "no version"
	for line in p.stdout.readlines():
		if line.find("Version: ") == 0:
			retval = line.lstrip("Version: ")
			break
	#print "solver version: ", retval
	return retval
	
def run_test_case(virgoExe, vgiFiles):
	retval = ["first"]
	theque = "output succeed: "
	for inputfile in vgiFiles:
		theCmd = virgoExe + " -test " + inputfile
		print ' '
		print '**Running: ', theCmd
		ppn = subprocess.Popen(theCmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
		#for line in ppn.stdout.readlines():
		# Poll process for new output until finished
		successful = False
		while True:
			nextline = ppn.stdout.readline()
			if nextline == '' and ppn.poll() is not None:
				break
			sys.stdout.write(nextline)
			sys.stdout.flush()
			if len(nextline) < len(theque):
				continue
			#print nextline[0:len(nextline)-1]
			if nextline.find(theque) == 0:
			    vgiFile = nextline.lstrip(theque)
				vgiFileBase = vgiFile.append(.base)
				print vgiFile
				print vgiFileBase
				res = compare_file(vgiFile,vgiFileBase)	
				retval.append(inputfile,res)
				successful = True
		
		#record failed jobs
		if not successful:
			retval.append(inputfile + ", ****, aborted, ****")
		exitCode = ppn.returncode
		
	retval.pop(0)
	return retval

def run_dir(out_file):
	aDirList = get_immediate_subdirectories(".")
	#print aDirList
	for subdir in aDirList:
		#crash in these folders  "hyperelastic" "rubber"
		#if subdir == "ampTest" or subdir == "pRodTet" or subdir == "sheetform" or \
		#	subdir == "tet" or subdir == "tlsb" or \
		#	subdir == "elastoplastic" or subdir == "disp+bc" or subdir == "disp-bc":
		#	print "----------------------------------------------"
		#	print "**Skipping ", subdir
		#	continue
		
		os.chdir(subdir)
		#vgi files in this sub folder
		vgiFiles = glob.glob("*.vgi");
		if len(vgiFiles) > 0:
			print "----------------------------------------------"
			print "Running subdirectory: ", subdir
			test_result = run_test_case(virgoExe, vgiFiles)
			for line in test_result:
				out_file.write(line)
				out_file.write("\n")
		
		#recursion into this sub folder
		run_dir(out_file)
		
		os.chdir("..")
		#break
    return 
#main entry
if __name__ == '__main__':
	print "\nStarting Virgo testing"
	print str(datetime.now())

	scriptDir = os.path.dirname(os.path.realpath(__file__))
	virgoRoot = scriptDir
	virgoRoot = os.path.abspath(os.path.join(virgoRoot, os.pardir))
	virgoRoot = os.path.abspath(os.path.join(virgoRoot, os.pardir))
	virgoRoot = os.path.abspath(os.path.join(virgoRoot, os.pardir))
	virgoRoot = os.path.abspath(os.path.join(virgoRoot, os.pardir))

	#if os.name == "posix":
	#	virgoExe = os.path.join(virgoRoot, "install/linux64_opt/bin/vg_solver")
	#else:
	#	virgoExe = os.path.join(virgoRoot, "install\\win64\\bin\\vg_solver.exe")
	parser = argparse.ArgumentParser()
	parser.add_argument('-solver', required=True, help='specify solver executable')
	args = parser.parse_args()
	
	virgoExe = args.solver
	#print virgoExe
	if not os.path.isfile(virgoExe):
		print ""
		print "Error: cannot find ", virgoExe
		sys.exit("Aborted!")
	
	solverVersion = solver_version(virgoExe)
	#print ": ", solverVersion
	#raise SystemExit(0)
        
	#output file name: solver version + date + sequence
	iii = 1
	outfn = "vg_solver_" + solverVersion + "_" + str(date.today()) + "." + str(iii) + ".csv"
	outfn = os.path.join(scriptDir, outfn)
	while os.path.exists(outfn):
		outfn = "vg_solver_" + solverVersion + "_" + str(date.today()) + "." + str(iii) + ".csv"
		outfn = os.path.join(scriptDir, outfn)
		iii = iii + 1
		
	print "Virgo root dir: ", virgoRoot
	print "Virgo   : ", virgoExe
	print "Virgo version: ", solverVersion
	print "Test output   : ", outfn
	
	out_file = open(outfn, "w")
	#out_file.write(solverVersion)
	currDir = os.getcwd()
	os.chdir(scriptDir)
	
	#run tests
	run_dir(out_file)
	
	#return to the original directory 
	os.chdir(currDir)
	
	out_file.close()
	print "----------------------------------------------"
	print "Virgo test completed!"
	print str(datetime.now())
