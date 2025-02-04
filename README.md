# prerequisite
# for compilation of gperftools
yum install autoconf -y
yum install automake  -y
yum install libtool  -y
yum install patch  -y
# for compilation of poco
yum install libatomic -y
# build
mkdir build && cd build
cmake .. & make
