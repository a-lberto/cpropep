Building Rocketworkbench (20011016)
====================================

These build instructions were written by Geoff O'Callaghan for building the
Debian package for rocketworkbench.  They are generic enough to be useful
on just about any Unix/Linux system.  They are brief!

1.  Getting the source.... If you're reading this then you've probably
already got the source code.  If you're reading this and you don't have the
source code then hey don't worry :-)

For anonymous readonly access

export CVSROOT=:pserver:anonymous@cvs.rocketworkbench.sourceforge.net:/cvsroot/rocketworkbench

cvs login

Password is 'null'  - ie. just hit enter

cvs -z6 -q co rocketworkbench

2. Compiling the source

Rocketworkbench relies on a library called GPCP.  This is a separate
development by Antoine and his brother.  In order to compile rocketworkbench
you'll need to obtain the GPCP library and place it in the 
sources/libraries directory.

<need a download location for gpcp!>

ie.
cd rocketworkbench/sources/libraries
tar zxvf gpcp.tgz

will create a gpcp directory and rocketworkbench is already configured to 
build using GPCP in this location.

In order to compile cpropep-web and lrd-web you will also require cgilib.

cgilib can be obtained on Debian systems using:

apt-get install cgilib

Once all thats done. Just type in 'make' in the rocketworkbench directory and
it should ALL HAPPEN.


