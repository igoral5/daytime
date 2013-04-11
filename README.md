daytime
=======

Simple server and client daytime

Сборка:
=======

CXXFLAGS="-ggdb -Wall -Werror" cmake .<br>
make

Сборка под Raspberry Pi:
========================
CXXFLAGS="-ggdb -Wall -Werror" cmake -DCMAKE_TOOLCHAIN_FILE=/home/igor/Dropbox/Toolchain-RaspberryPi.cmake .<br>
make
