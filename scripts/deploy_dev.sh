#!/bin/bash

git push origin develop
ssh root@164.90.163.58 "cd /root/server/CXXRestFullAPI && git pull && git checkout develop && scripts/build.sh"