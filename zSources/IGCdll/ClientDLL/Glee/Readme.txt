GLeeGen consists of the the codegen programs and scripts for generating the glee library. It is Copyright 2009 Ben Woodhouse, and is licensed under the GPL 3.0. See license.txt for details. 

GLee the library is licensed under a modified BSD license. See the DATA/RELEASE/readme.txt for more details.

-----------------------------------------------------
Instructions for building, testing and releasing GLEE
-----------------------------------------------------

GENERATING DATA
- everything from Data/GLeeXMLGenInput/Specs/headers and /extspecs
- run graball.bat to grab headers and specs to DATA/GLeeXMLGenInput/Specs
- Run GLeeXMLGen to generate XML files

GENERATING SOURCE 
- Edit the version number and copyright year in readme.txt, gleeGen.cpp and license.txt and GLeeVersion.txt
- Run GleeGen to generate GLee.c and GLee.h in /DATA/output

WINDOWS
- Build GLeeLib to build GLee.lib for windows (Build the RELEASE version) 
- Build and run GLeeTest to test

LINUX
- edit package.sh and update the .tar.gz version number
- Run VM
- cd /mnt/hgfs/DOnBen/p4/GLee/GLeeScripts/linux
- run ./package.sh to copy all the needed files to DATA/outputLinux and create the .tar.gz
- run ./test.sh to build the shared library

  TODO! this step builds the shared library. Use it to build redist and binary tarballs

MAC
- compile and run GLeeTestMac in XCode
- step into render function and check initialisation works

RELEASING
- Edit prepareForRelease.bat and update the zip file version number
- cd to GLeeScripts/publish/

- Run prepareForRelease.bat to copy the following files to the RELEASE folder (this will clean the folder first)
   DATA/outputLinux/GLeeX_Y.tar.gz
   DATA/output/GLeeX_Y.zip
   DATA/output/readme.txt
   DATA/output/extensionList.txt
   
NOTES
- GLeeXMLGen does not (yet) automatically grab new types etc. These need to be manually added to exceptions.txt
- Problematic extensions can be ignored by adding to the list in GLExtIgnore.txt
- The GLeeXMLGen will by default use extensions from the headers over the ones from the specs. To override this behaviour 
  for specific extensions, add it to HeaderIgnore.txt
- Extension specs can be overridden with the PatchedSpecs directory. Specs found in here will override downloaded versions. 
  This is useful for fixing up nvidia's non-standard specs so they actually work, for example
