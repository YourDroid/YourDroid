#!/bin/bash

# Rememeber start dir
START_DIR=$PWD

# Determine which dir to deploy in and cd to that dir
if [ -d "$1" ]; then
   DEPLOY_DIR=$1
else
   DEPLOY_DIR=$PWD
fi
echo "Deploy dir: $DEPLOY_DIR"
cd $DEPLOY_DIR

# Run ldd on all files in the directory and create a list of required qt libs
flag=false
for entry in `ldd $DEPLOY_DIR/* | grep -i qt`; do
   if $flag; then
      # Only add to the array if it is not already in it
      if ! [[ $libsArray =~ $entry ]]; then
         echo "adding $entry"
         libsArray="$libsArray $entry"
      fi
      flag=false
   fi

   # If we see a "=>" then the next line will be a library
   if [ $entry == "=>" ]; then
      flag=true
   fi
done
echo 
echo

# Create the required folder structure. Note here we are need the qt audio plugin so we are going to manually copy that as well.
mkdir -p lib
mkdir -p lib/audio
# Now copy these files to the deploy directory
for entry in $libsArray; do
   echo "cp -v -f $entry $DEPLOY_DIR/lib"
   cp -v -f $entry $DEPLOY_DIR/lib
done

# Now get the audio lib - this is a plugin that we are using so we need these libs as well.
# Add other plugins here as well.
# TODO: maybe we can read this in from the *.pro file.
cp -v -f `qmake -query QT_INSTALL_BINS`/../plugins/audio/* $DEPLOY_DIR/lib/audio

# Go back to start dir
cd $START_DIR
