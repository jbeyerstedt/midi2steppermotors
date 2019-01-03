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

# MIDI TEST
sleepLong=0.5
sleepShort=0.1

# each once slow
echo "\x90$noteG4$noteVel_100" > $ttyPort
echo "G4 100"
sleep $sleepLong
echo "\x90$noteG4$noteVel_0" > $ttyPort
echo "G4 0"
sleep $sleepLong

echo "\x90$noteA4$noteVel_100" > $ttyPort
echo "A4 100"
sleep $sleepLong
echo "\x90$noteA4$noteVel_0" > $ttyPort
echo "A4 0"
sleep $sleepLong

echo "\x90$noteH4$noteVel_100" > $ttyPort
echo "H4 100"
sleep $sleepLong
echo "\x90$noteH4$noteVel_0" > $ttyPort
echo "H4 0"
sleep $sleepLong

echo "\x90$noteC5$noteVel_100" > $ttyPort
echo "C5 100"
sleep $sleepLong
echo "\x90$noteC5$noteVel_0" > $ttyPort
echo "C5 0"
sleep $sleepLong

echo "\x90$noteD5$noteVel_100" > $ttyPort
echo "D5 100"
sleep $sleepLong
echo "\x90$noteD5$noteVel_0" > $ttyPort
echo "D5 0"
sleep $sleepLong

sleep 1

# each once fast
echo "\x90$noteD5$noteVel_100" > $ttyPort
echo "D5 100"
sleep $sleepShort
echo "\x90$noteD5$noteVel_0" > $ttyPort
echo "D5 0"
sleep $sleepShort

echo "\x90$noteC5$noteVel_100" > $ttyPort
echo "C5 100"
sleep $sleepShort
echo "\x90$noteC5$noteVel_0" > $ttyPort
echo "C5 0"
sleep $sleepShort

echo "\x90$noteH4$noteVel_100" > $ttyPort
echo "H4 100"
sleep $sleepShort
echo "\x90$noteH4$noteVel_0" > $ttyPort
echo "H4 0"
sleep $sleepShort

echo "\x90$noteA4$noteVel_100" > $ttyPort
echo "A4 100"
sleep $sleepShort
echo "\x90$noteA4$noteVel_0" > $ttyPort
echo "A4 0"
sleep $sleepShort

echo "\x90$noteG4$noteVel_100" > $ttyPort
echo "G4 100"
sleep $sleepShort
echo "\x90$noteG4$noteVel_0" > $ttyPort
echo "G4 0"
sleep $sleepShort

sleep 1

# random 1,3,5 on; toggle all; 2,4 off
echo "\x90$noteG4$noteVel_100" > $ttyPort
echo "G4 100"
sleep $ttySleep

echo "\x90$noteH4$noteVel_100" > $ttyPort
echo "H4 100"
sleep $ttySleep

echo "\x90$noteD5$noteVel_100" > $ttyPort
echo "D5 100"
sleep $ttySleep

sleep 1

echo "\x90$noteG4$noteVel_0" > $ttyPort
echo "G4 0"
sleep $ttySleep

echo "\x90$noteA4$noteVel_100" > $ttyPort
echo "A4 100"
sleep $ttySleep

echo "\x90$noteH4$noteVel_0" > $ttyPort
echo "H4 0"
sleep $ttySleep

echo "\x90$noteC5$noteVel_100" > $ttyPort
echo "C5 100"
sleep $ttySleep

echo "\x90$noteD5$noteVel_0" > $ttyPort
echo "D5 0"
sleep $ttySleep

sleep 1

echo "\x90$noteA4$noteVel_0" > $ttyPort
echo "A4 0"
sleep $ttySleep

echo "\x90$noteC5$noteVel_0" > $ttyPort
echo "C5 0"
sleep $ttySleep
