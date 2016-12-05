#!/bin/sh

echo "simple test:"
echo "simple test works" | AED_PASS="simpletest" ./aed -e | AED_PASS="simpletest" ./aed -d
echo ""

echo "../group3/http.c test:"
AED_PASS="somethinglongenoughtomakethisworthwile" ./aed -e <../group3/http.c >file.enc
AED_PASS="somethinglongenoughtomakethisworthwile" ./aed -d <file.enc >file.dec
DIFF=$(diff ../group3/http.c file.dec)
if [ "$DIFF" == "" ]; then
    echo "PASS"
else
    echo $DIFF
fi
echo ""

rm file.enc
rm file.dec
