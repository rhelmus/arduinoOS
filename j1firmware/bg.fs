start-microcode bg

\ COMM interface:
\ COMM+0: background colour top bar
\ COMM+2: background colour desktop
\ COMM+4-42: desktop gradient colours (16 px blocks)
\ COMM+44: set when framebuffer (fb) should be copied to char memory

h# 6fff constant FRAMEBUFFER \ fb block is at end of RAM_SPRIMG
: 1+    d# 1 + ;
: @     dup c@ swap 1+ c@ swab or ;
: !     over swab over 1+ c! c! ;
: >     swap < ;
: setbg ( -- )
        YLINE c@
        dup d# 0 = if COMM+0 @ BG_COLOR ! drop else
        dup d# 7 > over d# 300 < and if d# 4 rshift d# 2 * COMM+4 + @ h# 2000 ! COMM+2 @ BG_COLOR ! else
        drop then then ;

: main
    begin
        \ wait until command reg is nonzero
        begin
            setbg \ Update background
            d# 44 COMM+0 + c@ d# 1 = \ flag set?
        until

        \ Copy FRAMEBUFFER to RAM_PIC
        FRAMEBUFFER RAM_PIC
        d# 4096 \ 64 * 64 chars
        begin
            setbg \ Keep updating the background (avoids flickering)

            >r
            over c@ over c!
            1+ swap 1+ swap
            r> 1- dup d# 0 =
        until

        \ clear fb dump flag
        d# 0 d# 44 COMM+0 + c!
    again
;

end-microcode
