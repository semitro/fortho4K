" ************************** " cr
" list of words: " cr
" word xt immediate " cr
ls
" ************************** " cr
" basics: " cr
" 3 + 2 = " 3 2 + .

" 15 - 17 = " 15 17 - .
" Using a new word: sq" cr
: sq dup * ;
" 2^2 = " 2 sq .
" 5^2 = " 5 sq .

" Using a new word that used the old one: sq3" cr
: sq3 dup sq * ;
" 3^3 = " 3 sq3 .
" 5^3 = " 5 sq3 .
 
" ************************** " cr

