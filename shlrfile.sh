# print the commands being run cuz that be kinda nice yk
info() {
    echo "$*"
    "$@"
}

cc="gcc"

build="bin"
src="src"
obj="$build/obj"

exec="$build/shlr"

sources=$(find "$src" -name "*.c")
for source in $sources; do
    objects="$objects "$obj/$(basename ${source%.c}.o)""
done

cflags="-g -Iinclude"
lflags="-g -ggdb -fsanitize=address"

cflags_release="-O2 -Iinclude"

INSTALL_DIR="/usr/local/bin"

#targ builddev
#depends buildObjs
$cc $lflags -o $exec $objects
#endtarg

#targ buildObjs
#depends mkdirs
for source in $sources; do
    info $cc -c $cflags -o $obj/$(basename ${source%.c}.o) $source
done
#endtarg

#targ mkdirs
info mkdir -p $build
info mkdir -p $obj
#endtarg

#targ buildRelease
#depends clean
info mkdir -p $build
info $cc $cflags_release -o $exec $sources
#endtarg

#targ install
#depends buildRelease
info install -m 755 $exec $INSTALL_DIR
#endtarg

#targ uninstall
info rm -f $INSTALL_DIR/shlr
#endtarg

#targ clean
info rm -rf $build
#endtarg
