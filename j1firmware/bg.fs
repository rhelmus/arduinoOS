start-microcode bg

\ COMM interface:
\ COMM+0: background colour top bar
\ COMM+2-40: desktop background gradient colours (16 px blocks)

: 1+    d# 1 + ;
: @     dup c@ swap 1+ c@ swab or ;
: !     over swab over 1+ c! c! ;
: >     swap < ;
: setbg ( line -- )
        dup d# 0 = if COMM+0 @ h# 2000 ! drop else
        dup d# 8 > over d# 300 < and if d# 4 rshift d# 2 * COMM+2 + @ h# 2000 ! else
        drop then then ;

: main
    begin
        YLINE c@
        setbg
    again
;

end-microcode
