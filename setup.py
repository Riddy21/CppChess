#!/usr/bin/env python
import sys

incl_dirs = [sys.argv[-1]]
wrap_file = sys.argv[-2]
src_file = sys.argv[-3]
module_name = sys.argv[-4]
output_filename = sys.argv[-5]

sys.argv = sys.argv[:-5]
from distutils.core import setup, Extension

module = Extension('%s' % output_filename,
                   sources = [wrap_file, src_file],
                   include_dirs=incl_dirs
                   )

setup (name = '%s' % module_name,
       version = '0.1',
       author      = "SWIG Docs",
       description = """Simple swig example from docs""",
       ext_modules = [module],
       py_modules = [module_name],
       )
