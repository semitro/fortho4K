: -rot swap >r swap r> ;

: over >r dup r> swap ;
: 2dup over over ;

: <> = not ;
: <= 2dup < -rot = lor ;
: > <= not ;
: >= < not ; 


: IMMEDIATE  last_word @ cfa 1 - dup c@ 1 or swap c! ;
: pull , ; IMMEDIATE
: readc inbuf readc@ inbuf c@ ;

: repeat here 8 + ; IMMEDIATE
: until 'to_lit branch_if , , ; IMMEDIATE

: ( repeat readc 41 - until ; IMMEDIATE


: if 'to_lit not , 'to_lit branch_if , 0 , here  ; IMMEDIATE

: else 'to_lit branch , here 8 + 0 , swap here 8 + swap !  ; IMMEDIATE
: then here 8 + swap ! ; IMMEDIATE

( END OF STDLIB )
