include part1.frt

( ***unit tests for part 1*** )
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

: test parity_num? tab . ."  - " . cr ;
: test_parity ." parity: " cr
      0        test
      1        test
      2	       test  
      88888888 test 
      88888887 test 
      -1       test 
      -12      test 
      -13      test 
   
;

: test tab dup . ." : " kollats_seq cr ;
: test_kollats_seq ." Kollat's sequence: " cr
      1	   test
      4    test
      5    test
      7    test
      17   test
      19   test
      1024 test   
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
	test_kollats_seq
	cr
	." ********" cr
	." THAT'S ALL " cr
;

( let's test it! )
test_Oshepkov_Artem's_P3202_words
( the delay happens because of 10000439 prime checking.
  notice: the delay isn't so big for such number )
