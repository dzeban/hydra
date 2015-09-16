#!/bin/sh
set -e
. ./iso.sh

cat > bochsrc.txt << EOF
megs:            32
display_library: sdl2
romimage:        file=/opt/share/bochs/BIOS-bochs-latest
vgaromimage:     file=/opt/share/bochs/VGABIOS-lgpl-latest
ata0-master:     type=cdrom, path=myos.iso, status=inserted
boot:            cdrom
log:             bochslog.txt
clock:           sync=realtime, time0=local
cpu:             count=1, ips=1000000
magic_break:     enabled=1
EOF

bochs -f bochsrc.txt -q
