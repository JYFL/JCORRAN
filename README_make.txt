
Note on how to compile JCORRAN

After svn co JCORRAN go to trunk/JCORRAN dir and do

cmake .

Then you can do "make" there or under local.
Be careful "." after cmake. actually, you need to point top directory of JCORRAN.

Now mostly, you don't need "make all". 
but if you have a compile problem sometimes, then in trunk/JCORRAN

./bin/cmake-clean.sh
cmake .

Temporarily you need to use
/mnt/flustre/bschang/bin/cmake .




