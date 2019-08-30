import os, os.path, glob, fnmatch, subprocess, sys, difflib
import argparse
from datetime import datetime
from datetime import date

virgoExe = "D:/VirgoWorkRelease/Release/Virgo.exe"
virgoVersion = ""
reportFileName = ""


def run(virgoExe):	
	args = virgoExe
	print args
	p = subprocess.Popen(args,bufsize=0, shell = True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
	print "subProcessBagin"
	retval = "no version"
	foundVersion = 0
	uiInitializaitonFinished = 0
	print p.stdout.readline()
	print p.stdout.readline()
	print p.stdout.readline()
	print p.stdout.readline()
	print p.stdout.readline()
	print p.stdout.readline()
	for line in p.stdout.readlines()://会等待子进程结束,readline则不会
		print "readlines"
		if line.find("Version: ") == 0:
			retval = line.lstrip("Version: ")
			retval = retval[:-2]
			foundVersion = 1
			
		if line.find("Ui initialization") == 0:
			uiInitializaitonFinished = 1
		if uiInitializaitonFinished and foundVersion:
			break
			
	print "virgo version: ", retval
	return retval
	
if __name__ == "__main__":
	run(virgoExe)