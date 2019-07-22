import os
from setuptools import setup, Extension

module_name = 'custom'
extension_sources =  ['custommodule.cpp']

module = Extension(module_name, sources=extension_sources, language='c++')
setup(name=module_name, ext_modules = [module])
