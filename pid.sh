#!/bin/bash

# Function to unload the library
unload_library() {
    sudo gdb -n -q -batch                                   \
         -ex "attach $1"                                    \
         -ex "set \$dlopen = (void* (*)(char*, int))dlopen" \
         -ex "set \$dlclose = (int (*)(void*))dlclose"      \
         -ex "set \$dlerror = (char* (*)(void))dlerror"     \
                                                            \
         -ex "set \$self = \$dlopen(\"$2\", 6)"             \
         -ex "call \$dlclose(\$self)"                       \
         -ex "call \$dlclose(\$self)"                       \
                                                            \
         -ex "call \$dlerror()"                             \
         -ex "detach"                                       \
         -ex "quit"
}

# Function to debug the library
debug_library() {
    sudo gdb -n -q                                          \
        -ex "attach $1"                                    \
        -ex "set \$dlopen = (void* (*)(char*, int))dlopen" \
        -ex "set \$dlclose = (int (*)(void*))dlclose"      \
        -ex "set \$dlerror = (char* (*)(void))dlerror"     \
        -ex "call \$dlopen(\"$2\", 2)"                     \
        -ex "call \$dlerror()"                             \
        -ex "continue" # Comment this line for manual debug
}

# Function to check if the library is already loaded
is_library_loaded() {
    grep -q "$2" "/proc/$1/maps"
}

if [ "$#" -lt 1 ]; then
    echo "Usage: $0 <pid> [unload|debug]"
    exit 1
fi

pid=$1
libname="libDarkstorm.so"
libpath=$(realpath "$(dirname "$0")/$libname")

# Check for needed fonts. Delete these lines if you are sure all the needed
# fonts are installed.

# Used to echo each command. For debugging.
#set -x

if [ "$2" == "unload" ]; then
    unload_library "$pid" "$libpath"
    exit 0
elif [ "$2" == "debug" ]; then
   echo "Launching in debug mode."
   debug_library "$pid" "$libpath"
   exit 0
fi

if is_library_loaded "$pid" "$libpath"; then
    echo -e "Enoch already loaded. Reloading...\n";
    unload_library "$pid" "$libpath"
    debug_library "$pid" "$libpath"
else
    sudo gdb -n -q -batch                                   \
         -ex "attach $pid"                                  \
         -ex "set \$dlopen = (void* (*)(char*, int))dlopen" \
         -ex "set \$dlclose = (int (*)(void*))dlclose"      \
         -ex "set \$dlerror = (char* (*)(void))dlerror"     \
         -ex "call \$dlopen(\"$libpath\", 2)"               \
         -ex "call \$dlerror()"                             \
         -ex "detach"                                       \
         -ex "quit"
fi

#set +x
echo -e "\nDone."

