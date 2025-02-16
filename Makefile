build = bin
src = src
obj = $(build)/obj

exec = $(build)/shlr

source = $(shell find $(src) -name "*.c")
objects = $(patsubst $(src)/%.c, $(obj)/%.o, $(source))

INSTALL_DIR = /usr/local/bin

cflags = -g -Iinclude
lflags = -g -ggdb -fsanitize=address

cflags_release = -O2 -Iinclude
lflags_release = -O2

$(exec): $(objects)
	gcc $(lflags) -o $@ $^

$(obj)/%.o: $(src)/%.c
	@mkdir -p $(dir $@)
	gcc -c $(cflags) -o $@ $<

buildRelease: clean
	mkdir -p $(build)
	gcc $(cflags_release) $(lflags_release) -o $(exec) $(source)
	rm -rf *.o

install: buildRelease
	install -m 755 $(exec) $(INSTALL_DIR)

uninstall:
	rm -rf $(INSTALL_DIR)/shlr

clean:
	rm -rf $(build) $(exec)

