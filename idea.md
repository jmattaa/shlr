## shlr (A shell wrapper (basically make but idk))

features:
 - Simple and clean syntax
 - Task-based, not file-based
 - Dependencies between targets with `depends`
 - Multiline shell scripting with `run`

```bash
var cc "gcc"
var cxx "g++"

targ build
    depends [someothertarget anothertarget]

    # access vars in run with {varname}
    run
        {cc} main.c -o main
        echo "build"
        ./run_a_shell_script.sh
        echo "done"
    end
end

targ mytarget
    depends [anothertarget]
    run echo "this is one line" end
end
```

running it should be
```sh
shlr mytarget # should run target named mytarget 
shlr # should run first target
```
