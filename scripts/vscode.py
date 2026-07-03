#!/usr/bin/python3
import json
import re
from platform import uname

RE_CC=re.compile(r"CC='(.*)")
RE_CFLAGS=re.compile(r"^[a-zA-Z0-9_]+_CFLAGS='(.*)'")
RE_INCLUDE=re.compile(r"-I(\S+)")

includes=set()

with open("config.log", "r") as f:
  for line in f:
    m = RE_CFLAGS.match(line)
    if m:
      options = m.group(1)
      for dir in RE_INCLUDE.findall(options):
        includes.add(dir)

properties = {
  "configurations": [
    {
      "name": "Linux",
      "includePath": [
        "${workspaceFolder}/**"
      ],
      "defines": [],
      "compilerPath": "/usr/bin/gcc",
      "cStandard": "c17",
      "cppStandard": "c++17",
      "intelliSenseMode": "linux-gcc-x64",
    },
  ],
  "version": 4,
}


for c in properties["configurations"]:
  c["includePath"].extend(sorted(includes))
  if uname() == 'x86_64':
    c["intelliSenseMode"] = "linux-gcc-x64"
  elif uname() == 'aarch64':
    c["intelliSenseMode"] = "linux-gcc-arm64"
  
print(json.dumps(properties, indent=4))
