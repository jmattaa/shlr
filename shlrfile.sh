cc="gcc"
cxx="g++"

#targ build
#depends [targ2]

$(cc) src/main.c -o main
echo "build"
echo "done"

#endtarg

#targ targ2

echo "pre build"

#endtarg
