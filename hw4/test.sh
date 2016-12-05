#!/bin/sh

$(AED_PASS="somethinglongenoughtomakethisworthwile" ./aed -e <../group3/http.c >file.enc)
$(AED_PASS="somethinglongenoughtomakethisworthwile" ./aed -d <file.enc >file.dec)

