( task number 1 )
: mask_smallest 1 and ;
: parity_num? dup mask_smallest not ;

( task number 2 )
( a b - b a - b a a - a a b - a [a%b] - a [a%b] 0 - a [[a%b] == 0] )
( : divide? swap dup rot dup % 0 = ; )




( a - [a-1] ) 
: dec 1 - ;
: inc 1 + ;


( a b - a b b a)
: double_mirror swap dup rot dup rot ;
( a b - a b a b )
: double double_mirror swap ;

( a b - a b [a%b == 0] )
: divide? double % 0 = ;
( wtf? )
: prime? dup dup 2 / ( a - a [a/2] 1 )
              repeat 
		( drop )
                divide? ( a i [a%i == 0] ) 
		if drop drop 0 exit then ( a 0 )  
		dec     ( a i-1 )
		dup 1 =     ( a i [i == 1] ) ( if i == 1, it's over )
              until
	      drop drop 1 ( a 1 )
;

( a - a [is a prime?] )
: prime? 
	  parity_num? if 0 exit then
	  3   ( a i )
          repeat 
            divide?  ( a i [a%i == 0] ) 
	    if drop 0 exit then ( a 0 )  
	    inc inc     ( a i+=2 )
	    double_mirror ( a i i a)
	    2 /     ( a i i a/2)    
            =       ( a i [i==a/2]) ( if i == a/2, it's over )
          until
	  drop 1 ( a 1 )
;
: test_prime ." prime: " cr 
      2 prime?          . ."  - " . cr 
      88888888 prime?   . ."  - " . cr 
      13 prime?         . ."  - " . cr 
      17 prime?         . ."  - " . cr 
      19 prime?	        . ."  - " . cr
      887 prime?        . ."  - " . cr
      889 prime?        . ."  - " . cr
;
test_prime
              
