: -rot swap >r swap r> ;

: over >r dup r> swap ;
: 2dup over over ;
: 2drop drop drop ;
: 2over >r >r dup r> swap r> swap ;

: <> = not ;
: <= 2dup < -rot = lor ;
: > <= not ;
: >= < not ; 

: inc 1 + ;
: dec 1 - ;

: IMMEDIATE  last_word @ cfa 1 - dup c@ 1 or swap c! ;
: pull , ; IMMEDIATE
: readc inbuf readc@ inbuf c@ ;

: repeat here 8 + ; IMMEDIATE
: until 'to_lit branch_if , , ; IMMEDIATE

: ( repeat readc 41 - until ; IMMEDIATE
: " inbuf dec repeat inc dup dup readc@ c@ 34 -  until 0 swap c! inbuf print ; IMMEDIATE
: cr 10 emit ; 

: if 'to_lit not , 'to_lit branch_if , 0 , here  ; IMMEDIATE

: else 'to_lit branch , here 8 + 0 , swap here 8 + swap !  ; IMMEDIATE
: then here 8 + swap ! ; IMMEDIATE

" Welcome to forto4ka - advanced forth machine "
cr
( END OF STDLIB )
