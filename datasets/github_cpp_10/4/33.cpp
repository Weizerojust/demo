��/ /   I n s e r t i o n . c p p   :   D e f i n e s   t h e   e n t r y   p o i n t   f o r   t h e   c o n s o l e   a p p l i c a t i o n . 
 
 / / 
 
 
 
 # i n c l u d e   " s t d a f x . h " 
 
 # i n c l u d e < i o s t r e a m > 
 
 # i n c l u d e   " I n s e r t i o n S o r t . h " 
 
 
 
 
 
 u s i n g   n a m e s p a c e   s t d ; 
 
 
 
 v o i d   s o r t ( i n t   a n A r r a y [ ] ,   i n t   n ) ; 
 
 
 
 v o i d   p r i n t A r r a y ( i n t   t h e A r r a y [ ] ,   i n t   n ) ; 
 
 
 
 i n t   m a i n ( ) 
 
 { 
 
 
 
 	 I n s e r t i o n S o r t < i n t >   s o r t   =   I n s e r t i o n S o r t < i n t > ( ) ; 
 
 
 
 	 i n t   t h e A r r a y [ 1 0 ]   =   {   1 0 , 9 , 8 , 7 , 6 , 5 , 4 , 3 , 2 , 1   } ; 
 
 	 i n t   s i z e   =   s i z e o f ( t h e A r r a y )   /   s i z e o f ( t h e A r r a y [ 0 ] ) ; 
 
 	 p r i n t A r r a y ( t h e A r r a y ,   s i z e ) ; 
 
 
 
 	 s o r t . i n s e r t i o n S o r t ( t h e A r r a y ,   s i z e ) ; 
 
 	 
 
 
 
 	 c o u t   < <   e n d l   < <   " a f t e r   i n s e r t i o n   s o r t " ; 
 
 
 
 	 p r i n t A r r a y ( t h e A r r a y , s i z e ) ; 
 
 
 
 	 c i n . g e t ( ) ; 
 
 	 c i n . g e t ( ) ; 
 
 
 
         r e t u r n   0 ; 
 
 } 
 
 v o i d   p r i n t A r r a y ( i n t   t h e A r r a y [ ] ,   i n t   n ) 
 
 { 
 
 	 c o u t   < <   e n d l   < <   e n d l   < <   " t h e   a r r a y   c o n t a i n s :   " ; 
 
 	 f o r   ( i n t   i   =   0 ;   i   <   n ; i + + ) 
 
 	 { 
 
 	 	 c o u t < <   t h e A r r a y [ i ]   < <   " ,   " ; 
 
 	 } 
 
 } 
 
 
 
 
 
 
 
 