build = bin
src = src
obj = $(build)/obj

exec = $(build)/shlr

source = $(shell find $(src) -name "*.c")
objects = $(patsubst $(src)/%.c, $(obj)/%.o, $(source))

INSTALL_DIR = /usr/local/bin

cflags = -g -Iinclude
lflags = -g -ggdb 

$(exec): $(objects)
	gcc $(lflags) -o $@ $^

$(obj)/%.o: $(src)/%.c
	@mkdir -p $(dir $@)
	gcc -c $(cflags) -o $@ $<

install: $(exec)
	install -m 755 $(exec) $(INSTALL_DIR)

clean:
	rm -rf $(build)

