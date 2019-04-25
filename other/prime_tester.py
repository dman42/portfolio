from Tkinter import *

root = Tk()
root.title("Prime Checker")

global numberPrime

introText = "Prime Checker. Programmed by David McClarty. Last updated 01/06/2018.\n\
Type your number into the program to see if your number is prime or not.\n\
Press the 'info' button for more information and more cool things this program does.\n\
Note: Numbers as large as 9 digits will take 5 seconds or more to calculate.\n\
This program was coded in Python 2.7, written by David McClarty, and was first coded on 1/04/2017."

primeList = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97]

startNum = 0
finishNum = 0
stepsize = 0
inputStartNum = None
inputFinishNum = None
inputStepSize = None

def onButtonClick():
    numberPrime = inputPrime.get()
    outputPrime.delete(1.0, END)
    try:
        primeTest(int(numberPrime))
    except ValueError:
        if float(numberPrime) % 1 != 0:
            outputPrime.insert(END, "Decimals are not integers and therefore cannot be prime.\n")
        else:
            outputPrime.insert(END, "I don't know what " + str(numberPrime) + " means.")

def primeTest(numberPrime):
    try:
        primeBoolean = True
        
        if numberPrime < 0:
            outputPrime.insert(END, "Negative numbers aren't really prime numbers.\n")
            numberPrime = abs(numberPrime)
        if numberPrime == 1:
            outputPrime.insert(END, "1, while only divisible by 1 or itself, is not considered a prime, because it can't be used in prime factorization.\n")  
        elif numberPrime == 0:
            outputPrime.insert(END, "0 has an infinite amount of factors, and therefore isn't prime.\n")
        else:
            x = 101.0
            for number in range(len(primeList)):
                if float(numberPrime) % (primeList[number]) == 0 and primeList[number] != numberPrime:
                    primeBoolean = False
                    x = primeList[number]
                    break
            if primeBoolean:   
               while x-1 <= (numberPrime / 101.0):
                    if float(numberPrime) % x == 0:
                        primeBoolean = False
                        break
                    x = x + 2.0
            if primeBoolean:
                outputPrime.insert(END, str(numberPrime) + " is a prime\n")
            else:
                outputPrime.insert(END, str(numberPrime) + " is divisible by " + str(x) + "\n")
    except ValueError:
        outputPrime.delete(1.0, END)
        outputPrime.insert(END, "Please insert a number.")
        
outputPrime = Text(root, bg = "grey", height = 13)
outputPrime.insert(END, introText)
outputPrime.pack()
inputPrime = Entry(root, bg = "white")
inputPrime.pack()
enterButton = Button(root, text = "Enter", command = onButtonClick)
enterButton.pack()

root.geometry('500x300')
root.mainloop()
