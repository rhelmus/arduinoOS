#!/usr/bin/python

import Image
import gameduino.prep as gdprep
 
(dpic, dchr, dpal) = gdprep.encode(Image.open("lines.png"))
hdr = open("gfx.h", "w")
##gdprep.dump(hdr, "lines_pic", dpic)
gdprep.dump(hdr, "borderChars", dchr)
gdprep.dump(hdr, "borderPal", dpal)

ir = gdprep.ImageRAM(hdr)
arrow = gdprep.palettize(Image.open("mouse-arrow.png"), 4)
gdprep.dump(hdr, "mouseArrowPal", gdprep.getpal(arrow))
ir.addsprites("mouseArrow", (16, 16), arrow, gdprep.PALETTE4A, center = (8, 8))
gdprep.dump(hdr, "mouseArrowImg", ir.used())
