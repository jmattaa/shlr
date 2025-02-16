cc="gcc"
cxx="g++"

#endrun

#targ build
#depends [targ2]

$(cc) src/main.c -o main
echo "build"
echo "done"

#endrun

#targ targ2

echo "pre build"

#endrun
