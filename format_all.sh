#!/bin/bash

find . -type f -name "*.c" -exec c_formatter_42 {} \;
