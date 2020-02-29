#!/bin/bash

#cppcheck
cppcheck --xml --xml-version=2 -i ./Unity . 2> cppcheck-report.xml


