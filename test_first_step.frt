( SLS Course project, step #1 )
( Performed by Oshchepkov Artem, P3202 )

( Task #1 - is a number parity? )
: mask_smallest 1 and ;               ( a & 0x00..1 )
( a - a [is a parity?] )
: parity_num? dup mask_smallest not ; ( clearly )

( Task #2 - is a number prime? )

: dec 1 - ;
: inc 1 + ;

( a b - a b b a)
: double_mirror swap dup rot dup rot ;
( a b - a b a b )
: double double_mirror swap ;

( a b - a b [a%b == 0] )
: divide? double % 0 = ;

( a - a [is a prime?] )
: prime? 
	  parity_num? if 0 exit then
	  dup 1 =     if 0 exit then
          dup 3 =     if 1 exit then
	  3   ( a i )
          repeat
            divide?  ( a i [a%i == 0] ) 
	    if drop 0 exit then ( a 0 )  
	    inc inc     ( a i+=2 )
	    double_mirror ( a i i a )
	    2 /     ( a i i a/2 )    
            >       ( a i [i==a/2] ) ( if i == a/2, it's over )
          until
	  drop 1 ( a 1 )
;

( task #4, variant #0 )
: kollats_seq
( I'm not sure about checking correction of the function's data
  like we do it in C. It's not clear what should we do
  if a wrong value's came: just exit? drop value or not?..
  I think it depends of the specific goal of our programm.
  To make this word more portable, we'll miss such checkings )
	repeat
	parity_num?
	if 2 / else 3 * inc then
	dup .
	dup 1 = not ( if a = 1 it's over )
	until   ( 0 -> extra one iteration )
;

there_is_a_funny_way_to_left_a_message
sorry_i_was_too_tired_to_write_word_that_does_it
" wow, I've done it " cr
" ************************** " cr
" ##TEST 2.1: prime numbers " cr
" 17 " 17 prime? .
" 183 " prime? .
" 205 " prime? .
" ************************** " cr
" ##TEST 2.2: Kollat's sequence " cr
" from 17: " cr 17 kollats_seq
" from 23: "  cr 23 kollats_seq
