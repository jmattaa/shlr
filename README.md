<div align="center">

# shlr (basically shell scripts but with comments)

</div>

## Installation

clone this repository locally
```bash
git clone https://github.com/jmattaa/shlr
```
then build it with `shlr` or `make`
```bash
sudo make install
sudo shlr install
```
## What is this?

This is a wrapper around normal shell scripts, it lets you define targets 
give them dependencies and run them, all through writing comments in a normal
shell script.

Think of it like Makefile but instead of being file based it's task based and 
written in a normal shell script. Instead of creating a file for each target 
when doing normal shell scripts, you can do it all in one file.

## How does it look like?
You define a file called `shlrfile.sh` and here you can define your targets
and write normal shell code.
```bash
# you can define a global 'runblock'
# all this does is run for every single target
# you can define global variables or functions here
cc="gcc"
bindir="bin"
srcfile="main.c"

# would you like to make your targets verbose
# then you could add a 
# set -x or set -v at the end of the global 'runblock'

# targets are defined by a comment starting with 'targ' followed by a name
# you can add dependencies with 'depends' followed by a list of targets 
# (separated by spaces)

# targ build
# depends clean prebuild
$(cc) $(srcfile) -o $(bindir)/main
# endtarg

# targ prebuild
mkdir -p $(bindir)
# endtarg

# targ clean
rm $(bindir)/main
# endtarg

# you have to end every target with 'endtarg'

```
Now that you've defined your targets you can run them with `shlr <target>`
or just want to run the first target do `shlr`

## Flags

Here are the flags that you could use:
- `--dry-run` or `-d` will only print the commands that would be run without running them
- `--list-targets` or `-l` will list all the targets that you can run
- `--list-dependencies` or `-L` like `--list-targets` but it also lists the 
    dependencies for each target
- `--version` or `-v` will show the current version
- `--help` or `-h` will show a help message

