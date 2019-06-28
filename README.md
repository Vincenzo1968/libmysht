# libmysht
C library for generic hashtable and scope handling

libmysht README
===============

- 1 - Introduction
- 2 - Building libmysht 
- 3 - Installing libmysht 

1 - Introduction
----------------
libmysht is a library that implement hashtable and scoped hashtable

2 - Building libmysht
------------------------
To build libmysht use the following steps:

   1) Unzip and untar the distribution myoptparser-1.0.tar.gz source tarball:
   
      - $ tar -xvf libmysht-1.0.0.tar.gz
      - $ cd libmysht-1.0.0
 OR

   1) Use a git client to retrieve the source code anonymously from
      the project site. 

      - $ git clone https://github.com/Vincenzo1968/libmysht.git
      - $ cd libmysht
          
   2) Run the configure script to create libmysht Makefiles.
   
      - $ ./configure --help          # (for available configuration options)
      - $ ./configure [--with-option1][--with-the-other-option]

      NOTE: You may also run configure from a different directory in order to
      keep your distribution clean, e.g.:

      - $ mkdir build
      - $ chdir build
      - $ ../configure [--with-option1][--with-the-other-option]
          
   3) Run the top level Makefile:
    
      $ make
          
3 - Installing libmysht
--------------------------
To install libmysht, build the install target in the top level Makefile
    
      $ sudo make install
