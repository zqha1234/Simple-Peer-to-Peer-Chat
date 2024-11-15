1.get the hostname of the machine (such as asb9838nu-d04.csil.sfu.ca)
hostname --fqdn

2.compile and link the code
make

3.run the program
./a2

4.initiate the talk, input "s-talk [local port number] [destination hostname] [destination port number]"
s-talk 6060 asb9838nu-d04.csil.sfu.ca 6001

5.start chatting
-the maximum input length is 256, if you want to test message which is longer than 256, please manually change the buffer size

6.input '!' means exiting the program on both terminals, the other terminal will output '!' and exiting the program

7.clean the .o and program (a2)
make clean


