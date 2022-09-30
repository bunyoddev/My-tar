# Welcome to My Tar
***
Hello, my peer reviwer, I hope you will be kind while checking the project, Thank you!

## Task

TODO - My Tar

## Description

my_tar is a command to manipulate tape archive. The first option to tar is a mode indicator from the following list.
-c Create a new archive containing the specified items.
-r Like -c, but new entries are appended to the archive. The -f option is required.
-t List archive contents to stdout.
-u Like -r, but new entries are added only if they have a modification date newer than the corresponding entry in the archive. The -f option is required.
-x Extract to disk from the archive. If a file with the same name appears more than once in the archive, each copy will be extracted, with later copies overwriting (replacing) earlier copies.




## Installation

make

## Usage

```
    cd my-tar
    make
    ./my_tar -cf file.tar source.c source.h
    ./my_tar -tf file.tar
    ./my_tar -xf file.tar return the file
    ./my_tar -uf file.tar

```
