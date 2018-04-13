( SLS Course project, step #1)
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
	    double_mirror ( a i i a)
	    2 /     ( a i i a/2 )    
            >       ( a i [i==a/2] ) ( if i == a/2, it's over )
          until
	  drop 1 ( a 1 )
;


: prime?mem 
	prime?  ( a [is a prime?] )
	1 allot ( the result's type is bool so it's enough ) 
	dup     ( a p? addr addr  )
	rot	( a addr addr p? )
	swap    ( a addr p?  addr )
	!	( a addr ) 
;
( at first, I thought that it's a good idea to not destroy 
  the input value. Now I don't think so but let that code
  be 'called-saved' )

( task #3 )
( *s1 *s2 - *[s1+s2] )
: str_cat
	swap               ( let bottom string be first )
	double 		   ( addr1 addr2 addr1 addr2 )
	count swap count   ( addr1 addr2 len2  len1 )
	swap               ( it's important to save exactly len2 )
	dup rot +          ( addr1 addr2 len2 [len1+len2] )
	heap-alloc 	   ( addr1 addr2 len2 addr3 )
	( str1 copy )
	rot		   ( addr1 len2 addr3 addr2 )
	double             ( addr1 len2 addr3 addr2 addr3 addr2 )
	string-copy        ( addr1 len2 addr3 addr2 )
	heap-free          ( addr1 len2 addr3 ) ( addr2 is free )
	( we needed to know len2 to compute shift of the new addres )
	dup 		   ( addr1 len2 addr3 addr3 )
	rot 		   ( addr1 addr3 addr3 len2 )
	+		   ( addr1 addr3 [addr3+len2] )
	rot		   ( addr3 [addr3+len2] addr1 )
	dup		   ( addr3 [addr3+len2] addr1 addr1 )
	rot		   ( addr3 addr1 addr1 [addr3+len2] )
	swap		   ( addr3 addr1 [addr3+len2] addr1 )
	string-copy        ( addr3 addr1 )
	heap-free          ( addr1 is free now )
	(  addr3 )
;

( ***unit tests*** )
: tab   ."    " ;
: test 	tab str_cat dup prints heap-free cr ;
: test_str_cat ." str_cat: " cr
	m" hello,"  m"  world!"   test
	m" 12"      m" 34"        test

	m" let's try "
	m" to build a string "    str_cat
	m" by three ones!"        test

	m" empty string after:" 
        m" "                      test
	
        m" "
        m" - empty string before" test
	m" " m" " test   ( nothing is shown )
;

: test prime? tab . ."  - " . cr ;
: test_prime ." prime: " cr 
      0 	test
      1 	test
      2 	test
      88888888  test   
      10000391  test
      889 	test
      21 	test
      3 	test
      7 	test 
      13 	test 
      17 	test 
      19 	test
      887 	test
      100393	test
      10000439  test
;
: test parity_num? tab . ."  - " . cr drop ;
: test_parity ." parity: " cr
    0       test
    1       test
    2	    test  
   88888888 test 
   88888887 test 
   -1       test 
   -12      test 
   -13	    test 
   
;

: test_Oshepkov_Artem's_P3202_words
	cr 
	." *********" cr
	." TESTING:"  cr
	cr
	test_parity
	cr
	test_prime
	cr
	test_str_cat
	cr

	." ********" cr
	
;

test_Oshepkov_Artem's_P3202_words

              
