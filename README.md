**# embedded-lab7**

**## C and C# code from ECE09342: Intro to Embedded Lab 7 using the MSP430**

###This lab works to explore the use of digital communications by integrating the previous lab 6 into a GUI that can be interfaced to the online Internet of Things (IoT) development environment ThingSpeak. The raw ADC light and temperature data values go through UART conversion (smartHome.c, smartHome.c/GUI) and are sent into ThingSpeak to be visually interpreted into a graph.

##**smartHome.c:** This C program compiled in CCS works to convert the sampled data into 8-bit ASCII form. 
  input: sampled date 
  output: 8-bit ASCII sample

##**smartHome.c:** This C# program compiled in Visual Studios works to convert the 8-bit ASCII sample date into interger form. The program creates a GUI (graphical user interface). 
  input: 8-bit ASCII sample
  output: Integer 
