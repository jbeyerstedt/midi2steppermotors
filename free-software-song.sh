#!/bin/bash
ttyPort="/dev/ttyACM0"

ttySleep=0.05

noteG4="\x43"  # g'  (G4 = 67)
noteA4="\x45"  # a'  (A4 = 69)
noteH4="\x47"  # h'  (H4 = 71)
noteC5="\x48"  # c'' (C5 = 72)
noteD5="\x4A"  # d'' (D5 = 74)

noteVel_0="\x00"
noteVel_100="\x28"

# SETUP

stty -F /dev/ttyACM0 115200
sleep 0.5

# FREE SOFTWARE SONG
# 7/8 Takt, 120 bpm => 500 ms pro 1/4 Note, 250 ms pro 1/8 Note
break2=1
break4=0.495  # 0.500 - $ttySleep
break8=0.245  # 0.250 - $ttySleep
break38=0.745 # 0.750 - $ttySleep

# TAKT 1

#1/4 D5
echo "\x90$noteD5$noteVel_100" > $ttyPort
sleep $break4
echo "\x90$noteD5$noteVel_0" > $ttyPort
sleep $ttySleep

#1/8 C5
echo "\x90$noteC5$noteVel_100" > $ttyPort
sleep $break8
echo "\x90$noteC5$noteVel_0" > $ttyPort
sleep $ttySleep

#1/4 H4
echo "\x90$noteH4$noteVel_100" > $ttyPort
sleep $break4
echo "\x90$noteH4$noteVel_0" > $ttyPort
sleep $ttySleep

#1/4 A4
echo "\x90$noteA4$noteVel_100" > $ttyPort
sleep $break4
echo "\x90$noteA4$noteVel_0" > $ttyPort
sleep $ttySleep

# TAKT 2

#1/4 H4
echo "\x90$noteH4$noteVel_100" > $ttyPort
sleep $break4
echo "\x90$noteH4$noteVel_0" > $ttyPort
sleep $ttySleep

#1/8 C5
echo "\x90$noteC5$noteVel_100" > $ttyPort
sleep $break8
echo "\x90$noteC5$noteVel_0" > $ttyPort
sleep $ttySleep

#1/8 H4
echo "\x90$noteH4$noteVel_100" > $ttyPort
sleep $break8
echo "\x90$noteH4$noteVel_0" > $ttyPort
sleep $ttySleep

#1/8 A4
echo "\x90$noteA4$noteVel_100" > $ttyPort
sleep $break8
echo "\x90$noteA4$noteVel_0" > $ttyPort
sleep $ttySleep

#1/4 G4
echo "\x90$noteG4$noteVel_100" > $ttyPort
sleep $break4
echo "\x90$noteG4$noteVel_0" > $ttyPort
sleep $ttySleep

# TAKT 3

#3/8 G4
echo "\x90$noteG4$noteVel_100" > $ttyPort
sleep $break38
echo "\x90$noteG4$noteVel_0" > $ttyPort
sleep $ttySleep

#3/8 A4
echo "\x90$noteA4$noteVel_100" > $ttyPort
sleep $break38
echo "\x90$noteA4$noteVel_0" > $ttyPort
sleep $ttySleep

#1/8 H4
echo "\x90$noteH4$noteVel_100" > $ttyPort
sleep $break8
echo "\x90$noteH4$noteVel_0" > $ttyPort
sleep $ttySleep

# TAKT 4

#3/8 C5
echo "\x90$noteC5$noteVel_100" > $ttyPort
sleep $break38
echo "\x90$noteC5$noteVel_0" > $ttyPort
sleep $ttySleep

#1/4 H4
echo "\x90$noteH4$noteVel_100" > $ttyPort
sleep $break4
echo "\x90$noteH4$noteVel_0" > $ttyPort
sleep $ttySleep

#1/4 D5
echo "\x90$noteD5$noteVel_100" > $ttyPort
sleep $break4
echo "\x90$noteD5$noteVel_0" > $ttyPort
sleep $ttySleep

# TAKT 5

#3/8 H4
echo "\x90$noteH4$noteVel_100" > $ttyPort
sleep $break38
echo "\x90$noteH4$noteVel_0" > $ttyPort
sleep $ttySleep

#1/2 H4
echo "\x90$noteH4$noteVel_100" > $ttyPort
sleep $break2
echo "\x90$noteH4$noteVel_0" > $ttyPort
sleep $ttySleep

# TAKT 6

#1/4 D5
echo "\x90$noteD5$noteVel_100" > $ttyPort
sleep $break4
echo "\x90$noteD5$noteVel_0" > $ttyPort
sleep $ttySleep

#1/8 C5
echo "\x90$noteC5$noteVel_100" > $ttyPort
sleep $break8
echo "\x90$noteC5$noteVel_0" > $ttyPort
sleep $ttySleep

#1/2 H4
echo "\x90$noteH4$noteVel_100" > $ttyPort
sleep $break2
echo "\x90$noteH4$noteVel_0" > $ttyPort
sleep $ttySleep

# TAKT 7

#1/4 D4
echo "\x90$noteD5$noteVel_100" > $ttyPort
sleep $break4
echo "\x90$noteD5$noteVel_0" > $ttyPort
sleep $ttySleep

#1/8 C5
echo "\x90$noteC5$noteVel_100" > $ttyPort
sleep $break8
echo "\x90$noteC5$noteVel_0" > $ttyPort
sleep $ttySleep

#1/4 H4
echo "\x90$noteH4$noteVel_100" > $ttyPort
sleep $break4
echo "\x90$noteH4$noteVel_0" > $ttyPort
sleep $ttySleep

#1/4 A4
echo "\x90$noteA4$noteVel_100" > $ttyPort
sleep $break4
echo "\x90$noteA4$noteVel_0" > $ttyPort
sleep $ttySleep

# TAKT 8

#1/4 H4
echo "\x90$noteH4$noteVel_100" > $ttyPort
sleep $break4
echo "\x90$noteH4$noteVel_0" > $ttyPort
sleep $ttySleep

#1/8 C5
echo "\x90$noteC5$noteVel_100" > $ttyPort
sleep $break8
echo "\x90$noteC5$noteVel_0" > $ttyPort
sleep $ttySleep

#1/8 H4
echo "\x90$noteH4$noteVel_100" > $ttyPort
sleep $break8
echo "\x90$noteH4$noteVel_0" > $ttyPort
sleep $ttySleep

#1/8 A4
echo "\x90$noteA4$noteVel_100" > $ttyPort
sleep $break8
echo "\x90$noteA4$noteVel_0" > $ttyPort
sleep $ttySleep

#1/4 G4
echo "\x90$noteG4$noteVel_100" > $ttyPort
sleep $break4
echo "\x90$noteG4$noteVel_0" > $ttyPort
sleep $ttySleep

# TAKT 9

#3/8 G4
echo "\x90$noteG4$noteVel_100" > $ttyPort
sleep $break38
echo "\x90$noteG4$noteVel_0" > $ttyPort
sleep $ttySleep

#3/8 A4
echo "\x90$noteA4$noteVel_100" > $ttyPort
sleep $break38
echo "\x90$noteA4$noteVel_0" > $ttyPort
sleep $ttySleep

#1/8 H4
echo "\x90$noteH4$noteVel_100" > $ttyPort
sleep $break8
echo "\x90$noteH4$noteVel_0" > $ttyPort
sleep $ttySleep

# TAKT 10

#3/8 C5
echo "\x90$noteC5$noteVel_100" > $ttyPort
sleep $break38
echo "\x90$noteC5$noteVel_0" > $ttyPort
sleep $ttySleep

#1/8 H4
echo "\x90$noteH4$noteVel_100" > $ttyPort
sleep $break8
echo "\x90$noteH4$noteVel_0" > $ttyPort
sleep $ttySleep

#1/4 D5
echo "\x90$noteD5$noteVel_100" > $ttyPort
sleep $break4
echo "\x90$noteD5$noteVel_0" > $ttyPort
sleep $ttySleep

# TAKT 11

#3/8 A4
echo "\x90$noteA4$noteVel_100" > $ttyPort
sleep $break38
echo "\x90$noteA4$noteVel_0" > $ttyPort
sleep $ttySleep

#1/2 A4
echo "\x90$noteA4$noteVel_100" > $ttyPort
sleep $break2
echo "\x90$noteA4$noteVel_0" > $ttyPort
sleep $ttySleep

# TAKT 12

#3/8 A4
echo "\x90$noteA4$noteVel_100" > $ttyPort
sleep $break38
echo "\x90$noteA4$noteVel_0" > $ttyPort
sleep $ttySleep

#1/2 A4
echo "\x90$noteA4$noteVel_100" > $ttyPort
sleep $break2
echo "\x90$noteA4$noteVel_0" > $ttyPort
sleep $ttySleep


echo "song done"
