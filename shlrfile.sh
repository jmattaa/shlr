cc="gcc"

build="bin"
src="src"
obj="$build/obj"

exec="$build/shlr"

sources=$(find "$src" -name "*.c")
objects=$(echo "$sources" | sed "s|^$src/|$obj/|; s|\.c$|\.o|")

cflags="-g -Iinclude"
lflags="-g -ggdb -fsanitize=address"

cflags_release="-O2 -Iinclude"

INSTALL_DIR="/usr/local/bin"

# this set -x will make every target verbose, if it were in the begining of
# the file it would print the whole file, now it only prints the command
# run by the target
PS4='' # lessen the noise
set -x

#targ builddev
#depends buildObjs
$cc $lflags -o $exec $objects
#endtarg

#targ buildObjs
#depends mkdirs
for source in $sources; do
  object=$(echo "$source" | sed "s|^$src/|$obj/|; s|\.c$|\.o|")
  $cc $cflags -c "$source" -o "$object"
done
#endtarg

#targ mkdirs
mkdir -p $build
mkdir -p $obj
echo "$sources" | sed "s|^$src|$obj|; s|/[^/]*$||" | sort -u | xargs -I {} mkdir -p {}
#endtarg

#targ buildRelease
#depends clean
mkdir -p $build
$cc $cflags_release -o $exec $sources
#endtarg

#targ install
#depends buildRelease
install -m 755 $exec $INSTALL_DIR
#endtarg

#targ uninstall
rm -f $INSTALL_DIR/shlr
#endtarg

#targ clean
rm -rf $build
#endtarg
