: IMMEDIATE  last_word @ cfa 1 - dup c@ 1 or swap c! ;
: -rot swap >r swap  r> ;

: over >r dup r> swap ;
: 2dup over over ;

: <> = not ;
: <= 2dup < -rot =  lor ;
: > <= not ;
: >= < not ; 


: cell% 8 ;
: cells cell% * ;
: KB 1024 * ;
: MB KB KB  ;

: allot dp @ swap over + dp ! ;

: begin here ; IMMEDIATE
: again 'to_lit branch , , ; IMMEDIATE

: if 'to_lit 0branch , here 0  , ; IMMEDIATE
: else 'to_lit branch , here 0 , swap here swap !  ; IMMEDIATE
: then here swap ! ; IMMEDIATE
: endif 'to_lit then execute ; IMMEDIATE

( MY )
: IMMEDIATE  last_word @ cfa 1 - dup c@ 1 or swap c! ;
: pull , ; IMMEDIATE
: readc inbuf readc@ inbuf c@ ;

: repeat here ; IMMEDIATE
: until 'to_lit branch_if , , ; IMMEDIATE

: ( nop repeat readc 41 - until ; IMMEDIATE


: if 'to_lit not , 'to_lit branch_if , 0 , here   ; IMMEDIATE
: then here 8 + swap ! ; IMMEDIATE
( : then nop nop nop nop ' then0 pull ; IMMEDIATE )

: test 1 if drop drop drop then ;
1 2 3 4 5 6
test
.S

: test 0 if drop drop drop nop then ;
4 5 6
test
.S

: else 'to_lit branch , nop here 0 , swap nop here swap !  ; IMMEDIATE

: endif ' then execute ; IMMEDIATE

( END OF MY )

: for ' wand pull go_to_tos ;

: for 
      'to_lit swap ,
      'to_lit >r , 
      'to_lit >r , 
here  'to_lit r> , 
      'to_lit r> , 
      'to_lit 2dup , 
      'to_lit >r , 
      'to_lit >r , 
      'to_lit < ,  
      'to_lit 0branch ,  
here    0 , 
       swap ; IMMEDIATE

: endfor 
      'to_lit r> , 
      'to_lit lit , 1 ,   
        'to_lit + , 
       'to_lit >r , 
   'to_lit branch , 
            ,  here swap ! 
       'to_lit r> , 
     'to_lit drop , 
       'to_lit r> , 
     'to_lit drop ,  

;  IMMEDIATE

: do  'to_lit swap , 'to_lit >r , 'to_lit >r ,  here ; IMMEDIATE


: loop 
        'to_lit r> , 
        'to_lit lit , 1 , 
        'to_lit + , 
        'to_lit dup ,     
        'to_lit r@ , 
        'to_lit < , 
        'to_lit not , 
        'to_lit  swap , 
        'to_lit >r , 
        'to_lit 0branch , ,
        'to_lit r> , 
        'to_lit drop ,
        'to_lit r> , 
        'to_lit drop ,
 ;  IMMEDIATE

: readc inbuf readc@ inbuf c@ ;

: ( repeat readc 41 - not until ; IMMEDIATE

( old : : until  'to_lit 0branch , , ; IMMEDIATE );
