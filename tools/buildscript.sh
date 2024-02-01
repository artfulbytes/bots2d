#!/bin/bash

WD=$PWD

findWorkDirectory () {
## The `-d` test command option see if FILE exists and is a directory ##
[ -d "testapp" ] && (return 0) || (echo "Script is NOT called from base directory" ; cd ..)

[ -d "testapp" ] && ( WD=$PWD ; return 0) || (echo "Cannot find base directory, please start script from project root!" ; exit 1)

}

findWorkDirectory

clean () {
  echo "=== Cleaning build folder ==="
  rm -rf compile_commands.json 2> /dev/null
  cd testapp || (exit 1 ; echo "testapp folder not found")
  rm -rf build/
  cd $WD || (exit 1 ; echo "work directory not found")
}

config () {
  echo "=== Configuring build folder ==="
  cd testapp || (exit 1 ; echo "Testapp folder not found")
  mkdir build || (exit 1 ; echo "Build folder already exists, clean first!")
  cd build || (exit 1 ; echo "Build folder not found")
  cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 .. || (exit 1 ; echo "Config failed")
  mv compile_commands.json $WD/compile_commands.json || (echo "Move compile_commands.json failed")
  cd $WD || (exit 1 ; echo "Work directory not found")
}

compile () {
  echo "=== Compiling Project ==="
  cd testapp || (exit 1 ; echo "Testapp folder not found")
  cd build || (exit 1 ; echo "Build folder not found, please run config first!")
  cmake --build . || (exit 1 ; echo "Compile failed!")
  cd $WD || (exit 1 ; echo "Work directory not found")
}

test () {
  echo "=== Testing App ==="
  cd testapp || (exit 1 ; echo "Testapp folder not found")
  cd build || (exit 1 ; echo "Build folder not found, please run config first!")
  ./bots2dtest || (exit 1 ; echo "Can not execute or find binary!")
  cd $WD || (exit 1 ; echo "Work directory not found")
}

case $1 in
  all)
    [ ! -d "testapp/build" ] && (clean ; config)
    compile
    test
    ;;
  clean)
    clean
    ;;
  config)
    [ -d "testapp/build" ] && (clean ; config ; exit)
    config
    ;;
  c | compile | b | build)
    compile
    ;;
  t | test | r | run)
    test
    ;;
  *)
    if [ $1 ]; then
      echo "Unknown argument: $1"
    fi
    echo "Please provide one of the following arguments:"
    echo "  clean                   Deletes the build folder"
    echo "  config                  Creates build folder and configures build System"
    echo "  c | compile | b | build Compiles the Project"
    echo "  t | test                Test and Run the test app."
    exit
  ;;
esac
