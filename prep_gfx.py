#!/usr/bin/python

import Image
import gameduino.prep as gdprep
 
(dpic, dchr, dpal) = gdprep.encode(Image.open("window.png"))
hdr = open("gfx.h", "w")
gdprep.dump(hdr, "windowChars", dchr)
gdprep.dump(hdr, "windowPal", dpal)

(dpic, dchr, dpal) = gdprep.encode(Image.open("appicon.png"))
gdprep.dump(hdr, "appIconPic", dpic)
gdprep.dump(hdr, "appIconChars", dchr)
gdprep.dump(hdr, "appIconPal", dpal)

ir = gdprep.ImageRAM(hdr)
arrow = gdprep.palettize(Image.open("mouse-arrow.png"), 4)
gdprep.dump(hdr, "mouseArrowPal", gdprep.getpal(arrow))
ir.addsprites("mouseArrow", (16, 16), arrow, gdprep.PALETTE4A, center = (8, 8))
gdprep.dump(hdr, "mouseArrowImg", ir.used())
