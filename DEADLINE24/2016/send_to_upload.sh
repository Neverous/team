#!/bin/bash

if [ "$#" -ne 1 ]; then
      echo "Illegal number of parameters. Expecting one parameter: task code."
      exit 0
fi

if [ ! -d packs ]; then
  mkdir packs
fi

task=$1
FILE_NAME=$RANDOM
FILE_NAME=${FILE_NAME}${RANDOM}.zip
zip packs/${FILE_NAME} ${task}*.ans
scp packs/$FILE_NAME uploader@uploader.mpost.pl:~/waiting/$task/

echo "${FILE_NAME} sent!"
