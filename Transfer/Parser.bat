rem @echo off
rem cls


attrib -r .\BnFGameProto.h
attrib -r .\BnFGameProto.cpp
call .\ParseFile.bat .\BnFGameProto.xml .\BnFGameProto.h

goto :eof
