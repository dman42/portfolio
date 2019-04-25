
#-----Statement of Authorship----------------------------------------#
#
#  This is an individual assessment item.  By submitting this
#  code I agree that it represents my own work.  I am aware of
#  the University rule that a student must not act in a manner
#  which constitutes academic dishonesty as stated and explained
#  in QUT's Manual of Policies and Procedures, Section C/5.3
#  "Academic Integrity" and Section E/2.1 "Student Code of Conduct".
#
#    Student no: n0993976801
#    Student name: David McClarty
#
#  NB: Files submitted without a completed copy of this statement
#  will not be marked.  All files submitted will be subjected to
#  software plagiarism analysis using the MoSS system
#  (http://theory.stanford.edu/~aiken/moss/).
#
#--------------------------------------------------------------------#



#-----Assignment Description-----------------------------------------#
#
#  BUILDING BLOCKS
#
#  This assignment tests your skills at defining functions, processing
#  data stored in lists and performing the arithmetic calculations
#  necessary to display a complex visual image.  The incomplete
#  Python script below is missing a crucial function, "stack_blocks".
#  You are required to complete this function so that when the
#  program is run it produces a picture of a pile of building blocks
#  whose arrangement is determined by data stored in a list which
#  specifies the blocks' locations.  See the instruction
#  sheet accompanying this file for full details.
#
#  Note that this assignment is in two parts, the second of which
#  will be released only just before the final deadline.  This
#  template file will be used for both parts and you will submit
#  your final solution as a single file, whether or not you
#  complete both parts of the assignment.
#
#--------------------------------------------------------------------#  



#-----Preamble-------------------------------------------------------#
#
# This section imports necessary functions and defines constant
# values used for creating the drawing canvas.  You should not change
# any of the code in this section.
#

# Import the functions needed to complete this assignment.  You
# should not need to use any other modules for your solution.

from turtle import *
from math import *

# Define constant values used in the main program that sets up
# the drawing canvas.  Do not change any of these values.

block_size = 250 # pixels
top_and_bottom_border = 75 # pixels
left_and_right_border = 150 # pixels
canvas_width = (block_size + left_and_right_border) * 2
canvas_height = (block_size + top_and_bottom_border) * 2

#
#--------------------------------------------------------------------#



#-----Functions for Managing the Canvas------------------------------#
#
# The functions in this section are called by the main program to
# set up the drawing canvas for your image.  You should not change
# any of the code in this section.
#

# Set up the canvas and draw the background for the overall image
def create_drawing_canvas():

    # Set up the drawing canvas
    setup(canvas_width, canvas_height)

    # Set the coordinate system so that location (0, 0) is centred on
    # the point where the blocks will be stacked
    setworldcoordinates(-canvas_width / 2, -top_and_bottom_border,
                        canvas_width / 2, canvas_height - top_and_bottom_border)

    # Draw as fast as possible
    tracer(False)

    # Colour the sky blue
    bgcolor('sky blue')

    # Draw the ground as a big green rectangle (sticking out of the
    # bottom edge of the drawing canvas slightly)
    overlap = 50 # pixels
    penup()
    goto(-(canvas_width / 2 + overlap), -(top_and_bottom_border + overlap)) # start at the bottom-left
    fillcolor('pale green')
    begin_fill()
    setheading(90) # face north
    forward(top_and_bottom_border + overlap)
    right(90) # face east
    forward(canvas_width + overlap * 2)
    right(90) # face south
    forward(top_and_bottom_border + overlap)
    end_fill()
    penup()

    # Draw a friendly sun peeking into the image
    goto(-canvas_width / 2, block_size * 2)
    color('yellow')
    dot(250)

    # Reset everything ready for the student's solution
    color('black')
    width(1)
    penup()
    home()
    setheading(0)
    tracer(True)
    

# As a debugging aid, mark the coordinates of the centres and corners
# of the places where the blocks will appear
def mark_coords(show_corners = False, show_centres = False):

    # Go to each coordinate, draw a dot and print the coordinate, in the given colour
    def draw_each_coordinate(colour):
        color(colour)
        for x_coord, y_coord in coordinates:
            goto(x_coord, y_coord)
            dot(4)
            write(' ' + str(x_coord) + ', ' + str(y_coord), font = ('Arial', 12, 'normal'))

    # Don't draw lines between the coordinates
    penup()

    # The list of coordinates to display
    coordinates = []

    # Only mark the corners if the corresponding argument is True
    if show_corners:
        coordinates = [[-block_size, block_size * 2], [0, block_size * 2], [block_size, block_size * 2],
                       [-block_size, block_size], [0, block_size], [block_size, block_size],
                       [-block_size, 0], [0, 0], [block_size, 0]]
        draw_each_coordinate('dark blue')

    # Only mark the centres if the corresponding argument is True
    if show_centres:
        coordinates = [[-block_size / 2, block_size / 2], [block_size / 2, block_size / 2],
                       [-block_size / 2, block_size + block_size / 2], [block_size / 2, block_size + block_size / 2]]
        draw_each_coordinate('red')

    # Put the cursor back how it was
    color('black')
    home()


# End the program by hiding the cursor and releasing the window
def release_drawing_canvas():
    tracer(True)
    hideturtle()
    done()
    
#
#--------------------------------------------------------------------#



#-----Test data------------------------------------------------------#
#
# These are the data sets you will use to test your code.
# Each of the data sets is a list specifying the locations of
# the building blocks:
#
# 1. The name of the block, from 'Block A' to 'Block D'
# 2. The place to put the block, either 'Top left', 'Top right',
#    'Bottom left' or 'Bottom right'
# 3. The block's orientation, meaning the direction in which the top
#    of the block is pointing, either 'Up', 'Down', 'Left' or 'Right'
# 4. An optional mystery value, 'X' or 'O', whose purpose will be
#    revealed only in the second part of the assignment
#
# Each data set does not necessarily mention all four blocks.
#
# You can create further data sets, but do not change any of the
# given ones below because they will be used to test your submission.
#

# The following data set doesn't require drawing any blocks
# at all.  You may find it useful as a dummy argument when you
# first start developing your "draw_attempt" function.

arrangement_00 = []

# Each of the following data sets specifies drawing just one block
# in an upright orientation.  You may find them useful when
# creating your individual pieces.

arrangement_01 = [['Block A', 'Bottom left', 'Up', 'O']]
arrangement_02 = [['Block B', 'Bottom right', 'Up', 'O']]
arrangement_03 = [['Block C', 'Bottom left', 'Up', 'O']]
arrangement_04 = [['Block D', 'Bottom right', 'Up', 'O']]

# Each of the following data sets specifies drawing just one block
# in non-upright orientations.  You may find them useful when
# ensuring that you can draw all the blocks facing in different
# directions.

arrangement_10 = [['Block A', 'Bottom left', 'Down', 'O']]
arrangement_11 = [['Block A', 'Bottom right', 'Left', 'O']]
arrangement_12 = [['Block A', 'Bottom left', 'Right', 'O']]

arrangement_13 = [['Block B', 'Bottom left', 'Down', 'O']]
arrangement_14 = [['Block B', 'Bottom right', 'Left', 'O']]
arrangement_15 = [['Block B', 'Bottom left', 'Right', 'O']]

arrangement_16 = [['Block C', 'Bottom left', 'Down', 'O']]
arrangement_17 = [['Block C', 'Bottom right', 'Left', 'O']]
arrangement_18 = [['Block C', 'Bottom left', 'Right', 'O']]

arrangement_19 = [['Block D', 'Bottom left', 'Down', 'O']]
arrangement_20 = [['Block D', 'Bottom right', 'Left', 'O']]
arrangement_21 = [['Block D', 'Bottom left', 'Right', 'O']]

# The following data sets all stack various numbers of
# blocks in various orientations

arrangement_30 = [['Block C', 'Bottom right', 'Up', 'O'],
                  ['Block D', 'Top right', 'Up', 'O']]
arrangement_31 = [['Block A', 'Bottom left', 'Up', 'O'],
                  ['Block C', 'Bottom left', 'Up', 'O']]

arrangement_32 = [['Block B', 'Bottom right', 'Up', 'O'],
                  ['Block D', 'Bottom left', 'Up', 'O'],
                  ['Block C', 'Top right', 'Up', 'O']]
arrangement_33 = [['Block B', 'Bottom right', 'Up', 'O'],
                  ['Block D', 'Bottom left', 'Up', 'O'],
                  ['Block A', 'Top left', 'Up', 'O']]
arrangement_34 = [['Block B', 'Bottom left', 'Up', 'O'],
                  ['Block A', 'Bottom right', 'Up', 'O'],
                  ['Block D', 'Top left', 'Up', 'O'],
                  ['Block C', 'Top right', 'Up', 'O']]

arrangement_40 = [['Block C', 'Bottom right', 'Left', 'O'],
                  ['Block D', 'Top right', 'Right', 'O']]
arrangement_41 = [['Block A', 'Bottom left', 'Down', 'O'],
                  ['Block C', 'Bottom left', 'Up', 'O']]

arrangement_42 = [['Block B', 'Bottom right', 'Left', 'O'],
                  ['Block D', 'Bottom left', 'Left', 'O'],
                  ['Block C', 'Top right', 'Down', 'O']]
arrangement_43 = [['Block B', 'Bottom right', 'Right', 'O'],
                  ['Block D', 'Bottom left', 'Left', 'O'],
                  ['Block A', 'Top left', 'Right', 'O']]
arrangement_44 = [['Block B', 'Bottom left', 'Down', 'O'],
                  ['Block A', 'Bottom right', 'Left', 'O'],
                  ['Block D', 'Top left', 'Right', 'O'],
                  ['Block C', 'Top right', 'Up', 'O']]

arrangement_50 = [['Block B', 'Bottom right', 'Left', 'O'],
                  ['Block D', 'Bottom left', 'Left', 'O'],
                  ['Block C', 'Top right', 'Down', 'X']]
arrangement_51 = [['Block B', 'Bottom right', 'Right', 'O'],
                  ['Block D', 'Bottom left', 'Left', 'O'],
                  ['Block A', 'Top left', 'Right', 'X']]
arrangement_52 = [['Block B', 'Bottom left', 'Down', 'O'],
                  ['Block A', 'Bottom right', 'Left', 'O'],
                  ['Block D', 'Top left', 'Right', 'O'],
                  ['Block C', 'Top right', 'Up', 'X']]

arrangement_60 = [['Block B', 'Bottom right', 'Left', 'X'],
                  ['Block D', 'Bottom left', 'Left', 'O'],
                  ['Block C', 'Top right', 'Down', 'O']]
arrangement_61 = [['Block B', 'Bottom right', 'Right', 'O'],
                  ['Block D', 'Bottom left', 'Left', 'X'],
                  ['Block A', 'Top left', 'Right', 'O']]
arrangement_62 = [['Block B', 'Bottom left', 'Down', 'X'],
                  ['Block A', 'Bottom right', 'Left', 'X'],
                  ['Block D', 'Top left', 'Right', 'O'],
                  ['Block C', 'Top right', 'Up', 'O']]

# The following arrangements create your complete image, but
# oriented the wrong way

arrangement_80 = [['Block C', 'Bottom right', 'Left', 'O'],
                  ['Block D', 'Top right', 'Left', 'X'],
                  ['Block A', 'Bottom left', 'Left', 'O'],
                  ['Block B', 'Top left', 'Left', 'O']]

arrangement_81 = [['Block B', 'Bottom right', 'Right', 'X'],
                  ['Block D', 'Bottom left', 'Right', 'X'],
                  ['Block A', 'Top right', 'Right', 'O'],
                  ['Block C', 'Top left', 'Right', 'O']]

arrangement_89 = [['Block A', 'Bottom right', 'Down', 'O'],
                  ['Block C', 'Top right', 'Down', 'O'],
                  ['Block B', 'Bottom left', 'Down', 'O'],
                  ['Block D', 'Top left', 'Down', 'O']]

# The following arrangements should create your complete image
# (but with the blocks stacked in a different order each time)

arrangement_90 = [['Block C', 'Bottom left', 'Up', 'O'],
                  ['Block D', 'Bottom right', 'Up', 'O'],
                  ['Block B', 'Top right', 'Up', 'X'],
                  ['Block A', 'Top left', 'Up', 'O']]

arrangement_91 = [['Block D', 'Bottom right', 'Up', 'X'],
                  ['Block C', 'Bottom left', 'Up', 'X'],
                  ['Block A', 'Top left', 'Up', 'O'],
                  ['Block B', 'Top right', 'Up', 'O']]

arrangement_92 = [['Block D', 'Bottom right', 'Up', 'X'],
                  ['Block B', 'Top right', 'Up', 'O'],
                  ['Block C', 'Bottom left', 'Up', 'O'],
                  ['Block A', 'Top left', 'Up', 'O']]

arrangement_99 = [['Block C', 'Bottom left', 'Up', 'O'],
                  ['Block D', 'Bottom right', 'Up', 'O'],
                  ['Block A', 'Top left', 'Up', 'O'],
                  ['Block B', 'Top right', 'Up', 'O']]

#
#--------------------------------------------------------------------#



#-----Student's Solution---------------------------------------------#
#
#  The drawing is a Turtle rendition of the album cover of Pink Floyd's #    
#  Dark Side of The Moon. #                                   

# Pythagorean Function: A function that determines the hypotenuse of #
# a triangle based on its two non-hypotenuse sides. #

def pythagorean(side1, side2):
    return sqrt((float(side1)**2)+(float(side2)**2)) # returns hypotenuse #

# Block Functions: The functions that draw the four blocks. Each block takes #
# three arguments: two to position the block and one to give the block its #
# orientation. #

# Block A #

def blockA(coordX, coordY, direction):
    # Turtle Set-up for Black Square #
    penup()
    goto(coordX, coordY)
    setheading(direction)
    color("black")
    # Draws Black Square #
    begin_fill()
    for x in range(4):
        forward(250)
        right(90)
    end_fill()
    # Turtle Set-up for Triangle Segment #
    forward(250)
    right(90)
    forward(62.5)
    color("grey")
    width(3)
    right(30)
    # Draws Triangle Segment #
    pendown()
    forward(pythagorean(187.5, 112.511)-2)
    penup()
    # Turtle Set-up for Light Beam #
    left(120)
    forward(112.511)
    left(90)
    forward(50)
    left(90)
    forward(10)
    right(90)
    color("white")
    # Draws Light Beam #
    pendown()
    left(75)
    forward(pythagorean(45, 5))
    right(140)
    forward(pythagorean(45, 5))
    left(180)
    forward(pythagorean(45, 5))
    left(7)
    forward(112)
    # Turtle Set-up for Red Outline of Black Square #
    penup()
    goto(coordX, coordY)
    setheading(direction)   
    # Draws Red Outline #
    pendown()
    color("red")
    width(2)
    for x in range(4):
        forward(250)
        right(90)
    # Finishes #
    penup()

# Block B #

def blockB(coordX, coordY, direction):
    # Turtle Set-up for Black Square #
    penup()
    goto(coordX, coordY)
    setheading(direction)
    color("black")
    # Draws Black Square #
    begin_fill()
    for x in range(4):
        forward(250)
        right(90)
    end_fill()
    # Turtle Set-up for Triangle Segment #
    right(90)
    forward(62.5)
    color("grey")
    width(3)
    left(30)
    # Draws Triangle Segment #
    pendown()
    forward(pythagorean(187.5, 112.511)-2)
    penup()
    # Turtle Set-up for Rainbow #
    right(120)
    forward(112.511)
    right(90)
    forward(40)
    right(90)
    forward(95)
    right(20)
    # Draws Rainbow #
     # Purple #
    color("medium orchid")
    begin_fill()
    forward(110) 
    left(20)
    forward(20)
    left(160)
    forward(148)
    left(140)
    forward(15)
    end_fill()
    left(180)
    forward(15)
    right(140)
     # Blue #
    color("cyan")
    begin_fill()
    forward(150) 
    left(20)
    forward(20)
    left(160)
    forward(175) 
    left(140)
    forward(15)
    end_fill()
    left(180)
    forward(15)
    right(140)
     # Green #
    color("green")
    begin_fill()
    forward(175) 
    left(20)
    forward(20)
    left(160)
    forward(200) 
    left(140)
    forward(15)
    end_fill()
    left(180)
    forward(15)
    right(140)
     # Yellow #
    color("yellow")
    begin_fill()
    forward(194) 
    left(120)
    forward(14)
    left(62)
    forward(195) 
    left(140)
    forward(15)
    end_fill()
    left(180)
    forward(15)
    right(140)
     # Orange #
    color("orange")
    begin_fill()
    forward(199) 
    left(120)
    forward(15)
    left(62)
    forward(199) 
    left(140)
    forward(15)
    end_fill()
    left(180)
    forward(15)
    right(140)
     # Red #
    color("red")
    begin_fill()
    forward(200) 
    left(113)
    forward(15)
    left(69)
    forward(200) 
    left(140)
    forward(15)
    end_fill()   
    # Turtle Set-up for Red Outline of Black Square #
    goto(coordX, coordY)
    setheading(direction)
    # Draws Red Outline #
    pendown()
    color("red")
    width(2)
    for x in range(4):
        forward(250)
        right(90)
    # Finishes #
    penup()

# Block C #

def blockC(coordX, coordY, direction):
    # Turtle Set-up for Black Square #
    penup()
    goto(coordX, coordY)
    setheading(direction)
    color("black")
    # Draws Black Square #
    begin_fill()
    for x in range(4):
        forward(250)
        right(90)
    end_fill()
    # Turtle Set-up for Triangle Segment #
    forward(142.489)
    right(120)
    color("grey")
    width(3)
    # Draws Triangle Segment #
    pendown()
    forward(pythagorean(125,74.989))
    left(120)
    forward(180.5)
    penup()
    # Turtle Set-up for Light Beam #
    left(90)
    forward(128)
    left(90)
    forward(147)
    left(32)
    color('white')
    # Draws Light Beam #
    pendown()
    forward(122)
    penup()
    # Turtle Set-up for Red Outline of Black Square #
    goto(coordX, coordY)
    setheading(direction)
    # Draws Red Outline #
    pendown()
    color("red")
    width(2)
    for x in range(4):
        forward(250)
        right(90)
    # Finishes #
    penup()

# Block D #

def blockD(coordX, coordY, direction):
    # Turtle Set-up for Black Square #
    penup()
    goto(coordX, coordY)
    setheading(direction)
    color("black")
    # Draws Black Square #
    begin_fill()
    for x in range(4):
        forward(250)
        right(90)
    end_fill()
    # Turtle Set-up for Triangle Segment #
    forward(107.511)
    right(60)
    color("grey")
    width(3)
    # Draws Triangle Segment #
    pendown()
    forward(pythagorean(125,74.989))
    right(120)
    forward(181.5)
    penup()
    # Turtle Set-up for Rainbow #
    right(90)
    forward(125)
    right(90)
    forward(192)
    right(20)
    forward(61)
    # Draws Rainbow #
     # Purple #
    color("medium orchid")
    begin_fill()
    forward(4)
    left(120)
    forward(14)
    left(62)
    forward(37) 
    left(28)
    forward(15)
    end_fill()
    right(190)
    forward(25)
    right(20)
     # Blue #
    color("cyan")
    begin_fill()
    forward(27)
    left(120)
    forward(10)
    left(60)
    forward(5)
    left(30)
    forward(5)
    end_fill()
    # Turtle Set-up for Red Outline of Black Square #
    goto(coordX, coordY)
    setheading(direction)
    # Draws Red Outline #
    pendown()
    color("red")
    width(2)
    for x in range(4):
        forward(250)
        right(90)
    # Finishes #
    penup()

# Stack Function #

def stack_blocks(arrangement):

    # These Boolean variables determine whether a block should "fall" if the #
    # block under it is "lost". There are two blocks since there are a maximum #
    # of two lost blocks that a "falling" block can be over. #
    blockFall1 = False
    blockFall2 = False
     
    for listNumber in arrangement: # Splits superlist into several lists #

        # Splits each list into individual values #
        block, position, orientation, missing = listNumber

        # Determines whether a block is "lost" #
        if missing == "O":

            # Determines whether the block will "fall" to the bottom #
            if blockFall1:
                if position == futurePosition1 == "Top left":
                    position = "Bottom left"
                    blockFall1 = False
                elif position == futurePosition1 == "Top right":
                    position = "Bottom right"
                    blockFall1 = False
            if blockFall2:
                if position == futurePosition2 == "Top left":
                    position = "Bottom left"
                    blockFall2 = False
                elif position == futurePosition2 == "Top right":
                    position = "Bottom right"
                    blockFall2 = False
            
            # Sets position of block #
            if position == "Top left":
                coordX = -250
                coordY = 500
            elif position == "Top right":
                coordX = 0
                coordY = 500
            elif position == "Bottom left":
                coordX = -250
                coordY = 250
            elif position == "Bottom right":
                coordX = 0
                coordY = 250

            # Sets orientation of block #
            if orientation == "Up":
                dX = 0  # d for displacement, Changes where turtle starts drawing #
                dY = 0
                direction = 0 # Changes where turtle sets heading to start drawing #
            elif orientation == "Down":
                dX = 250
                dY = -250
                direction = 180
            elif orientation == "Left":
                dX = 0
                dY = -250
                direction = 90
            elif orientation == "Right":
                dX = 250
                dY = 0
                direction = 270
                
            # Sets block to draw #
            if block == "Block A":
                blockA(coordX + dX, coordY + dY, direction)
            elif block == "Block B":
                blockB(coordX + dX, coordY + dY, direction)
            elif block == "Block C":
                blockC(coordX + dX, coordY + dY, direction)
            elif block == "Block D":
                blockD(coordX + dX, coordY + dY, direction)
                
        elif missing == "X":
            # Determines whether a block could possibly be under a "falling" block. #
            if blockFall1:
                if position == "Bottom left":
                    futurePosition2 = "Top left"
                    blockFall2 = True
                elif position == "Bottom right":
                    futurePosition2 = "Top right"
                    blockFall2 = True
            else:        
                if position == "Bottom left":
                    futurePosition1 = "Top left"
                    blockFall1 = True
                elif position == "Bottom right":
                    futurePosition1 = "Top right"
                    blockFall1 = True
                    
# Note: The blocks won't "fall" if the list that draws the bottom blocks are #
# called AFTER the top blocks. #
#--------------------------------------------------------------------#



#-----Main Program---------------------------------------------------#
#
# This main program sets up the background, ready for you to start
# drawing your jigsaw pieces.  Do not change any of this code except
# where indicated by comments marked '*****'.
#

# Set up the drawing canvas
create_drawing_canvas()

# Control the drawing speed
# ***** Modify the following argument if you want to adjust
# ***** the drawing speed
speed('fastest')

# Decide whether or not to show the drawing being done step-by-step
# ***** Set the following argument to False if you don't want to wait
# ***** while the cursor moves around the screen
tracer(True)

# Give the window a title
# ***** Replace this title with one that describes the picture
# ***** produced by stacking your blocks correctly
title('Describe your image here')

# Display the corner and centre coordinates of the places where
# the blocks must be placed
# ***** If you don't want to display the coordinates change the
# ***** arguments below to False
mark_coords(True, True)

### Call the student's function to display the stack of blocks
### ***** Change the argument to this function to test your
### ***** code with different data sets
stack_blocks(arrangement_99)

# Exit gracefully
release_drawing_canvas()

#
#--------------------------------------------------------------------#

