#!/bin/bash

sudo rmmod dfs

cat /var/log/messages | tail -n 5
