
no = None
val = False

def test():
    if(no):
      print "None is 0"
    else:
      print "None is not 0"
    
    if(no != val):
      print "None is not False"
    else:
      print "None is not False"
      
    if(no == None):
      print "no is None"
      
    if(val == 0):
      print "False is 0"
    else:
      print "False is not 0"
if (__name__ == "__main__"):
   test()