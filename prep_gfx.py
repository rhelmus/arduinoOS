#!/usr/bin/python

import Image
import gameduino.prep as gdprep
 
(dpic, dchr, dpal) = gdprep.encode(Image.open("lines.png"))
hdr = open("gfx.h", "w")
##gdprep.dump(hdr, "lines_pic", dpic)
gdprep.dump(hdr, "borderChars", dchr)
gdprep.dump(hdr, "borderPal", dpal)

(dpic, dchr, dpal) = gdprep.encode(Image.open("appicon.png"))
gdprep.dump(hdr, "appIconPic", dpic)
gdprep.dump(hdr, "appIconChars", dchr)
gdprep.dump(hdr, "appIconPal", dpal)

ir = gdprep.ImageRAM(hdr)
arrow = gdprep.palettize(Image.open("mouse-arrow.png"), 4)
gdprep.dump(hdr, "mouseArrowPal", gdprep.getpal(arrow))
ir.addsprites("mouseArrow", (16, 16), arrow, gdprep.PALETTE4A, center = (8, 8))
gdprep.dump(hdr, "mouseArrowImg", ir.used())

#appicon = gdprep.palettize(Image.open("appicon.png"), 4)
#gdprep.dump(hdr, "appIconPal", gdprep.getpal(appicon))
#ir.addsprites("appIcon", (16, 16), appicon, gdprep.PALETTE4A, center = (8, 8))
#gdprep.dump(hdr, "appIconImg", ir.used())
