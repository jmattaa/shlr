cc="gcc"

build="bin"
src="src"
obj="$build/obj"

exec="$build/shlr"

sources=$(find "$src" -name "*.c")
objects=$(echo "$sources" | sed "s|^$src/|$obj/|; s|\.c$|\.o|")

cflags="-g -Iinclude"
lflags="-g -ggdb -fsanitize=address"

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

#targ clean
rm -rf $build
#endtarg
