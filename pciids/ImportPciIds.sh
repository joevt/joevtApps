#! /bin/bash
# joevt Aug 7,2021

sudo update-pciids
perl convertpciids.pl < /usr/local/share/pci.ids > pciidsInternal.c

printf "\xff\xfe" > pci.ids.ucs-2le # add a BOM
iconv -f UTF-8 -t UCS-2LE /usr/local/share/pci.ids >> pci.ids.ucs-2le
