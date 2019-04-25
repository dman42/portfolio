import os
import ntpath
from random import randint

def randomString():
    alphanum = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
    randString = ""
    length = randint(8, 12)
    x = 0
    while (length > x):
        randString += alphanum[randint(-1, 61)]
        x = x + 1
                               
    return randString

print "This program will change all the .jpg image names in it's folder into random strings."
print "Are you sure you want to do this?(Y/n)"
response = raw_input()
if ("y" in response.lower()):

    path = os.getcwd()
    files = os.listdir(path)

    for file in files:
        if ".jpg" in ntpath.basename(os.path.join(path, file)).lower():
            os.rename(os.path.join(path, file), os.path.join(path, randomString() + ".jpg"))

    print "Finished"    
                               
    
