#!/usr/bin/env python

import sys
import os

from BeautifulSoup import BeautifulSoup
from tornado import template


DEBUG =True

os.chdir(os.path.dirname(os.path.abspath(__file__)))

file_in = 'BnFGameProto.xml'

PREFIXES = dict(
    char= "ch",
    byte= "by",
    short= "sh",
    word= "w",
    int= "n",
    dword= "dw",
    int64= "ll",
    uint64= "qw",
    float= "f",
    string= "sz",
)

TYPE_MAP = dict(
    byte= "unsigned char",
    char= "signed char",
    short= "short",
    word= "unsigned short",
    int= "int",
    dword= "unsigned int",
    int64= "long long",
    uint64= "usigned long long",
    float= "float",
)


if not os.access(file_in, os.F_OK):
    print 'Input file "%s" does not exist.' % file_in
    sys.exit(1)

xml = BeautifulSoup(open(file_in, 'rb').read())

def output(content, filename=None):
    if filename and '--savetofiles' in sys.argv:
        open(filename, 'wb').write(content)
    else:
        print content

def nonprefix_varname(typename, name):
    return name

def prefix_varname(typename, varname):
    assert typename in PREFIXES, 'Invalid type %s' % typename
    return PREFIXES[typename] + varname


def type_def(typename, varname, length=None):
    assert typename in PREFIXES, 'Invalid type %s' % typename

    prefixed_varname = prefix_varname(typename, varname)
    if typename == 'string':
        return '%s\t%s[%s]' % ('char', varname, length)

    return '%s\t%s' % (TYPE_MAP[typename], varname)


context = dict(xml=xml, type_def=type_def, prefix_varname=prefix_varname, nonprefix_varname=nonprefix_varname)


def generate_cpp_files():
    " Generate cpp files. "

    H_TMPL = 'lang-tmpls/cpp/h.tmpl'
    CPP_TMPL = 'lang-tmpls/cpp/cpp.tmpl'
    
    t = template.Template(open(H_TMPL, 'rb').read())
    
    h_out = file_in.lower().split('.')[0] + '.h'
    output(t.generate(**context).replace('\n\n', '\n'), h_out)

    t = template.Template(open(CPP_TMPL, 'rb').read())

    cpp_out = file_in.lower().split('.')[0] + '.cpp'
    output(t.generate(**context).replace('\n\n', '\n'), cpp_out)

generate_cpp_files()

