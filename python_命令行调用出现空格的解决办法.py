import subprocess

bat = "E:\\test\\test data\\run.bat"
resultPath = "E:\\test\\test data"

print bat
print resultPath
subprocess.call([bat, resultPath])
