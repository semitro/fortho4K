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

: IMMEDIATE  last_word @ cfa 1 - dup c@ 1 or swap c! ;
: pull , ; IMMEDIATE
: readc inbuf readc@ inbuf c@ ;
: repeat here ; IMMEDIATE
: until 'to_xt ' go_to_tos_if , , ; IMMEDIATE

: ( repeat readc 41 - not until ; IMMEDIATE
: for repeat ' branch pull pull ;
: for 'to_lit wand , ' go_to_tos , ;


works! : for readc repeat ' branch pull pull ;
words! : for nop repeat ' branch pull pull ;

: until ' branch pull pull ; IMMEDIATE

: IMMEDIATE  last_word @ cfa 1 - dup c@ 1 or swap c! ;
: pull , ; IMMEDIATE
: readc inbuf readc@ inbuf c@ ;
: repeat here ; IMMEDIATE
: until 'to_xt ' go_to_tos_if , , ; IMMEDIATE
: ( repeat readc 41 - not until ; IMMEDIATE
works : ( nop repeat readc 41 - ' branch_if pull pull ; IMMEDIATE

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
