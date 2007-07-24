#!/bin/sh
#
# rebuild.sh: rebuild hypertext with the previous context.
#
# Usage:
#	% sh rebuild.sh
#
cd D:\MSYS\home\Martin Tang\micron && GTAGSCONF=':suffixes=c,h,y,s,S,java,c++,cc,cpp,cxx,hxx,hpp,C,H,php,php3,phtml:skip=GPATH,GTAGS,GRTAGS,GSYMS,HTML/,HTML.pub/,html/,tags,TAGS,ID,y.tab.c,y.tab.h,.notfunction,cscope.out,.gdbinit,SCCS/,RCS/,CVS/,CVSROOT/,{arch}/,.svn/,.cvsrc,.cvsignore,.gitignore,.cvspass,.cvswrappers,.deps/,autom4te.cache/,.snprj/:GTAGS=gtags-parser.exe -dt %s:GRTAGS=gtags-parser.exe -dtr %s:GSYMS=gtags-parser.exe -dts %s:' htags
