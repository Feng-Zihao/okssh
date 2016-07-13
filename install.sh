#!/bin/sh

if [ "`uname -s`" == "Darwin" ]
then
    curl -fsSL https://raw.githubusercontent.com/Feng-Zihao/okssh/master/okssh.rb -o /usr/local/Library/Taps/homebrew/homebrew-core/Formula/okssh.rb 
    brew install okssh
#    echo "On MacOSX"
else
    echo "Only can run on Mac OSX"
fi

