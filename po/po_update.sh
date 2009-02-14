#!/bin/bash
xgettext -k_ -kN_ --keyword=Q_:1g -o cugbfreer.pot ../src/*.c
msgmerge -o zh_CN.po zh_CN.po cugbfreer.pot
