cc="gcc"
cxx="g++"

#targ build
#depends targ2
$(cc) src/main.c -o main
echo "build"
echo "done"
#endtarg

#targ targ2
echo "pre build"
#endtarg

# a normal comment just dosent start with a keyword
# (targ, depends, endtarg)

#targ clean
#depends targ2 build
rm main
echo "clean"
echo "done"
#endtarg
