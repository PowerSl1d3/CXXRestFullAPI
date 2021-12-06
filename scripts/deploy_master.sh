#!/bin/bash

git push origin master
ssh root@164.90.163.58 "cd /root/server/CXXRestFullAPI && git pull && git checkout master && scripts/build.sh"