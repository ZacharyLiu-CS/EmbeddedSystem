#!/bin/sh

mkdir /dev/snd

mknod /dev/snd/mixer c 14 0
mknod /dev/snd/dsp   c 14 3
mknod /dev/snd/audio c 14 4

mknod /dev/snd/controlC0 c 116 0
mknod /dev/snd/seq       c 116 1

mknod /dev/snd/pcmC0D0p c 116 16
mknod /dev/snd/pcmC0D1p c 116 17
mknod /dev/snd/pcmC0D0c c 116 24
mknod /dev/snd/pcmC0D1c c 116 25

mknod /dev/snd/timer    c 116 33
