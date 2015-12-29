# Arduino-Communication-Lib
A small bunch of Code to make your C/C++ Code talking to your Arduino

To Build this code you need the option -lpthread (enable threading) and -std=c++0x (Use c++ 2011 standard), tested with gcc 4.7

How to use:
It's braindeath-simple. After creating an instance of the class, run init() and then you are good to go. Everything else should explain itself.


Known Issues:
The query-method fetches the input char-by-char. That is not nice, but I don't know any other fix to make it run on my CentOS 6.7 Machine.
