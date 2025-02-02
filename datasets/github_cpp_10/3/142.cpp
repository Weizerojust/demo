��/ /   A V L - T r e e . c p p   :   T h i s   f i l e   c o n t a i n s   t h e   ' m a i n '   f u n c t i o n .   P r o g r a m   e x e c u t i o n   b e g i n s   a n d   e n d s   t h e r e . 
 
 / / 
 
 
 
 # i n c l u d e   " p c h . h " 
 
 # i n c l u d e   < i o s t r e a m > 
 
 # i n c l u d e   < t i m e . h > 
 
 
 
 t y p e d e f   s t r u c t   N o d e   { 
 
 	 i n t   d a t a ;   
 
 	 N o d e   * l e f t ;   
 
 	 N o d e   * r i g h t ; 
 
 	 i n t   h e i g h t ;   
 
 } N o d e ; 
 
 
 
 t y p e d e f   s t r u c t   N o d e Q u e u e E l e m e n t   { 
 
 	 N o d e   * n o d e ; 
 
 	 N o d e Q u e u e E l e m e n t   * n e x t ; 
 
 } N o d e Q u e u e E l e m e n t ; 
 
 
 
 t y p e d e f   s t r u c t   N o d e Q u e u e _   { 
 
 	 N o d e Q u e u e E l e m e n t   * f r o n t ; 
 
 	 N o d e Q u e u e E l e m e n t   * b a c k ; 
 
 	 i n t   c o u n t ;   
 
 } N o d e Q u e u e _ ; 
 
 
 
 N o d e Q u e u e _ *   N o d e Q u e u e I n i t ( ) 
 
 { 
 
 	 N o d e Q u e u e _   * n Q u e u e   =   ( N o d e Q u e u e _ * ) m a l l o c ( s i z e o f ( N o d e Q u e u e _ ) ) ; 
 
 
 
 	 i f   ( n Q u e u e )   { 
 
 	 	 m e m s e t ( n Q u e u e ,   0 ,   s i z e o f ( N o d e Q u e u e _ ) ) ; 
 
 	 	 r e t u r n   n Q u e u e ; 
 
 	 } 
 
 
 
 	 r e t u r n   N U L L ;   
 
 } 
 
 
 
 v o i d   N o d e E n q u e u e ( N o d e Q u e u e _   * * n Q u e u e ,   N o d e   * n o d e )   { 
 
 
 
 	 i f   ( ! n o d e ) 
 
 	 	 r e t u r n ;   
 
 
 
 	 N o d e Q u e u e E l e m e n t   * n e w Q u e u e E l e m e n t   =   ( N o d e Q u e u e E l e m e n t * ) m a l l o c ( s i z e o f ( N o d e Q u e u e E l e m e n t ) ) ; 
 
 	 
 
 	 i f   ( n e w Q u e u e E l e m e n t )   { 
 
 	 	 n e w Q u e u e E l e m e n t - > n e x t   =   N U L L ; 
 
 	 	 n e w Q u e u e E l e m e n t - > n o d e   =   n o d e ; 
 
 
 
 	 	 i f   ( * n Q u e u e )   { 
 
 	 	 	 ( * n Q u e u e ) - > b a c k - > n e x t   =   n e w Q u e u e E l e m e n t ; 
 
 	 	 	 ( * n Q u e u e ) - > b a c k   =   n e w Q u e u e E l e m e n t ; 
 
 	 	 	 ( * n Q u e u e ) - > c o u n t + + ; 
 
 	 	 } 
 
 	 	 e l s e   { 
 
 	 	 	 * n Q u e u e   =   N o d e Q u e u e I n i t ( ) ; 
 
 	 	 	 ( * n Q u e u e ) - > f r o n t   =   ( * n Q u e u e ) - > b a c k   =   n e w Q u e u e E l e m e n t ; 
 
 	 	 	 ( * n Q u e u e ) - > c o u n t   =   1 ; 
 
 	 	 } 
 
 	 } 
 
 } 
 
 
 
 N o d e *   N o d e D e q u e u e ( N o d e Q u e u e _   * * n Q u e u e )   { 
 
 
 
 	 i f   ( * n Q u e u e   & &   ( * n Q u e u e ) - > c o u n t ) 
 
 	 { 
 
 	 	 N o d e Q u e u e E l e m e n t   * n e w Q u e u e E l e m e n t   =   ( * n Q u e u e ) - > f r o n t ; 
 
 	 	 N o d e   * n o d e   =   n e w Q u e u e E l e m e n t - > n o d e ; 
 
 	 	 ( * n Q u e u e ) - > f r o n t   =   ( * n Q u e u e ) - > f r o n t - > n e x t ; 
 
 	 	 ( * n Q u e u e ) - > c o u n t - - ; 	 
 
 
 
 	 	 i f   ( ! ( * n Q u e u e ) - > c o u n t ) 
 
 	 	 { 
 
 	 	 	 ( * n Q u e u e ) - > b a c k   =   N U L L ; 
 
 	 	 	 f r e e ( * n Q u e u e ) ; 
 
 	 	 	 * n Q u e u e   =   N U L L ; 
 
 	 	 } 
 
 
 
 	 	 f r e e ( n e w Q u e u e E l e m e n t ) ; 
 
 
 
 	 	 r e t u r n   n o d e ; 
 
 	 } 
 
 
 
 	 r e t u r n   N U L L ;   
 
 } 
 
 
 
 N o d e *   N o d e Q u e u e P e e k ( N o d e Q u e u e _   * n Q u e u e )   { 
 
 
 
 	 i f   ( n Q u e u e   & &   n Q u e u e - > c o u n t ) 
 
 	 { 
 
 	 	 r e t u r n   n Q u e u e - > f r o n t - > n o d e ; 
 
 	 } 
 
 
 
 	 r e t u r n   N U L L ; 
 
 } 
 
 
 
 v o i d   b f s T r e e ( N o d e   *   n o d e ) 
 
 { 
 
 	 N o d e Q u e u e _   * n Q u e u e   =   N U L L ; 
 
 	 
 
 	 p r i n t f ( " \ n T r a v e r s e   B F S \ n " ) ; 
 
 
 
 	 i f   ( n o d e ) 
 
 	 { 
 
 	 	 N o d e E n q u e u e ( & n Q u e u e ,   n o d e ) ; 
 
 
 
 	 	 w h i l e   ( n Q u e u e   & &   n Q u e u e - > c o u n t ) 
 
 	 	 { 
 
 	 	 	 N o d e   * n e x t N o d e   =   N o d e D e q u e u e ( & n Q u e u e ) ; 
 
 
 
 	 	 	 i f   ( n e x t N o d e )   { 	 	 
 
 	 	 	 	 p r i n t f ( " % 2 d ( % d )     " ,   n e x t N o d e - > d a t a ,   n e x t N o d e - > h e i g h t ) ; 
 
 	 	 	 	 N o d e E n q u e u e ( & n Q u e u e ,   n e x t N o d e - > l e f t ) ; 	 	 	 	 
 
 	 	 	 	 N o d e E n q u e u e ( & n Q u e u e ,   n e x t N o d e - > r i g h t ) ; 
 
 	 	 	 } 
 
 	 	 } 
 
 	 } 
 
 } 
 
 
 
 N o d e *   c r e a t e N o d e ( i n t   d a t a ) 
 
 { 
 
 	 N o d e   * n e w N o d e   =   ( N o d e * ) m a l l o c ( s i z e o f ( N o d e ) ) ; 
 
 
 
 	 i f   ( n e w N o d e )   { 
 
 	 	 n e w N o d e - > d a t a   =   d a t a ; 
 
 	 	 n e w N o d e - > l e f t   =   n e w N o d e - > r i g h t   =   N U L L ; 
 
 	 	 n e w N o d e - > h e i g h t   =   - 1 ; 
 
 	 	 
 
 	 	 r e t u r n   n e w N o d e ;   
 
 	 } 
 
 
 
 	 r e t u r n   N U L L ;   
 
 } 
 
 
 
 i n t   n o d e H e i g h t ( N o d e   * n o d e ) 
 
 { 
 
 	 r e t u r n   ( n o d e   ?     n o d e - > h e i g h t   :   - 1 ) ; 
 
 } 
 
 
 
 i n t   m a x O f ( i n t   a ,   i n t   b ) 
 
 { 
 
 	 r e t u r n   ( a   >   b   ?   a   :   b ) ; 
 
 } 
 
 
 
 v o i d   n o d e R o t a t e R i g h t ( N o d e   * n o d e ) 
 
 { 
 
 	 i f   ( n o d e   & &   n o d e - > l e f t )   { 
 
 	 	 N o d e   * t e m p L e f t   =   n o d e - > l e f t ; / /   T h i s   o n e   i s   g o i n g   a w a y 
 
 
 
 	 	 N o d e   * n e w N o d e   =   c r e a t e N o d e ( n o d e - > d a t a ) ; 
 
 	 	 n e w N o d e - > l e f t   =   t e m p L e f t - > r i g h t ; 
 
 	 	 n e w N o d e - > r i g h t   =   n o d e - > r i g h t ; 
 
 	 	 n e w N o d e - > h e i g h t   =   m a x O f ( n o d e H e i g h t ( n e w N o d e - > l e f t ) ,   n o d e H e i g h t ( n e w N o d e - > r i g h t ) )   +   1 ; 
 
 
 
 	 	 n o d e - > d a t a   =   t e m p L e f t - > d a t a ; 
 
 	 	 n o d e - > l e f t   =   t e m p L e f t - > l e f t ; 
 
 	 	 n o d e - > r i g h t   =   n e w N o d e ; 
 
 	 	 n o d e - > h e i g h t   =   m a x O f ( n o d e H e i g h t ( n o d e - > l e f t ) ,   n o d e H e i g h t ( n o d e - > r i g h t ) )   +   1 ; 
 
 
 
 	 	 f r e e ( t e m p L e f t ) ; 
 
 	 } 
 
 } 
 
 
 
 v o i d   n o d e R o t a t e L e f t ( N o d e   * n o d e ) 
 
 { 
 
 	 i f   ( n o d e   & &   n o d e - > r i g h t )   { 
 
 	 	 N o d e   * t e m p R i g h t   =   n o d e - > r i g h t ;   / /   T h i s   o n e   i s   g o i n g   a w a y 
 
 
 
 	 	 N o d e   * n e w N o d e   =   c r e a t e N o d e ( n o d e - > d a t a ) ; 
 
 	 	 n e w N o d e - > l e f t   =   n o d e - > l e f t ; 
 
 	 	 n e w N o d e - > r i g h t   =   t e m p R i g h t - > l e f t ; 
 
 	 	 n e w N o d e - > h e i g h t   =   m a x O f ( n o d e H e i g h t ( n e w N o d e - > l e f t ) ,   n o d e H e i g h t ( n e w N o d e - > r i g h t ) )   +   1 ; 
 
 
 
 	 	 n o d e - > d a t a   =   t e m p R i g h t - > d a t a ; 
 
 	 	 n o d e - > r i g h t   =   t e m p R i g h t - > r i g h t ; 
 
 	 	 n o d e - > l e f t   =   n e w N o d e ; 
 
 	 	 n o d e - > h e i g h t   =   m a x O f ( n o d e H e i g h t ( n o d e - > l e f t ) ,   n o d e H e i g h t ( n o d e - > r i g h t ) )   +   1 ; 
 
 
 
 	 	 f r e e ( t e m p R i g h t ) ; 
 
 	 } 
 
 } 
 
 
 
 v o i d   n o d e R o t a t e L e f t R i g h t ( N o d e   * n o d e )   
 
 { 
 
 	 i f   ( n o d e   & &   n o d e - > l e f t   & &   n o d e - > l e f t - > r i g h t ) 
 
 	 { 
 
 	 	 N o d e   * t e m p L e f t   =   n o d e - > l e f t ; 
 
 	 	 N o d e   * t e m p L e f t R i g h t   =   t e m p L e f t - > r i g h t ;   
 
 	 	 N o d e   * n e w N o d e   =   c r e a t e N o d e ( n o d e - > d a t a ) ; 
 
 
 
 	 	 n e w N o d e - > r i g h t   =   n o d e - > r i g h t ; 
 
 	 	 
 
 	 	 n o d e - > d a t a   =   t e m p L e f t R i g h t - > d a t a ; 	 	 
 
 	 	 n o d e - > r i g h t   =   n e w N o d e ; 
 
 
 
 	 	 t e m p L e f t - > r i g h t   =   t e m p L e f t R i g h t - > l e f t ; 
 
 
 
 	 	 t e m p L e f t - > h e i g h t   =   m a x O f ( n o d e H e i g h t ( t e m p L e f t - > l e f t ) ,   n o d e H e i g h t ( t e m p L e f t - > r i g h t ) )   +   1 ; 
 
 	 	 n e w N o d e - > h e i g h t   =   m a x O f ( n o d e H e i g h t ( n e w N o d e - > l e f t ) ,   n o d e H e i g h t ( n e w N o d e - > r i g h t ) )   +   1 ; 
 
 
 
 	 	 n o d e - > h e i g h t   =   m a x O f ( n o d e H e i g h t ( n o d e - > l e f t ) ,   n o d e H e i g h t ( n o d e - > r i g h t ) )   +   1 ; 
 
 	 	 
 
 	 	 f r e e ( t e m p L e f t R i g h t ) ; 
 
 	 } 
 
 } 
 
 
 
 v o i d   n o d e R o t a t e R i g h t L e f t ( N o d e   * n o d e ) 
 
 { 
 
 	 i f   ( n o d e   & &   n o d e - > r i g h t   & &   n o d e - > r i g h t - > l e f t )   
 
 	 { 
 
 	 	 N o d e   * t e m p R i g h t   =   n o d e - > r i g h t ;   
 
 	 	 N o d e   * t e m p R i g h t L e f t   =   t e m p R i g h t - > l e f t ;   
 
 	 	 N o d e   * n e w N o d e   =   c r e a t e N o d e ( n o d e - > d a t a ) ; 
 
 
 
 	 	 n e w N o d e - > l e f t   =   n o d e - > l e f t ; 
 
 
 
 	 	 n o d e - > d a t a   =   t e m p R i g h t L e f t - > d a t a ; 
 
 	 	 n o d e - > l e f t   =   n e w N o d e ; 
 
 
 
 	 	 t e m p R i g h t - > l e f t   =   t e m p R i g h t L e f t - > r i g h t ; 
 
 
 
 	 	 t e m p R i g h t - > h e i g h t   =   m a x O f ( n o d e H e i g h t ( t e m p R i g h t - > l e f t ) ,   n o d e H e i g h t ( t e m p R i g h t - > r i g h t ) )   +   1 ; 
 
 	 	 n e w N o d e - > h e i g h t   =   m a x O f ( n o d e H e i g h t ( n e w N o d e - > l e f t ) ,   n o d e H e i g h t ( n e w N o d e - > r i g h t ) )   +   1 ; 
 
 
 
 	 	 n o d e - > h e i g h t   =   m a x O f ( n o d e H e i g h t ( n o d e - > l e f t ) ,   n o d e H e i g h t ( n o d e - > r i g h t ) )   +   1 ; 
 
 
 
 	 	 f r e e ( t e m p R i g h t L e f t ) ; 
 
 	 } 
 
 } 
 
 
 
 v o i d   r e b a l a n c e N o d e ( N o d e   * c u r N o d e ) 
 
 { 
 
 	 c u r N o d e - > h e i g h t   =   m a x O f ( n o d e H e i g h t ( c u r N o d e - > l e f t ) ,   n o d e H e i g h t ( c u r N o d e - > r i g h t ) )   +   1 ; 
 
 
 
 	 i f   ( n o d e H e i g h t ( c u r N o d e - > l e f t )   -   n o d e H e i g h t ( c u r N o d e - > r i g h t )   >   1 ) 
 
 	 { 
 
 	 	 i f   ( n o d e H e i g h t ( c u r N o d e - > l e f t - > l e f t )   >   n o d e H e i g h t ( c u r N o d e - > l e f t - > r i g h t ) ) 
 
 	 	 	 n o d e R o t a t e R i g h t ( c u r N o d e ) ; 
 
 	 	 e l s e 
 
 	 	 	 n o d e R o t a t e L e f t R i g h t ( c u r N o d e ) ; 
 
 	 } 
 
 	 e l s e   i f   ( n o d e H e i g h t ( c u r N o d e - > r i g h t )   -   n o d e H e i g h t ( c u r N o d e - > l e f t )   >   1 )   
 
 	 { 
 
 	 	 i f   ( n o d e H e i g h t ( c u r N o d e - > r i g h t - > l e f t )   <   n o d e H e i g h t ( c u r N o d e - > r i g h t - > r i g h t ) ) 
 
 	 	 	 n o d e R o t a t e L e f t ( c u r N o d e ) ; 
 
 	 	 e l s e 
 
 	 	 	 n o d e R o t a t e R i g h t L e f t ( c u r N o d e ) ; 
 
 	 } 
 
 } 
 
 
 
 v o i d   a d d N o d e ( N o d e   * * n o d e ,   i n t   d a t a ) 
 
 { 
 
 	 N o d e   * c u r N o d e   =   * n o d e ; 
 
 
 
 	 i f   ( c u r N o d e )   { 
 
 	 	 i f   ( c u r N o d e - > d a t a   ! =   d a t a )   { 
 
 	 	 	 i f   ( d a t a   >   c u r N o d e - > d a t a )   { 
 
 	 	 	 	 a d d N o d e ( & c u r N o d e - > r i g h t ,   d a t a ) ; 
 
 	 	 	 } 
 
 	 	 	 e l s e   { 
 
 	 	 	 	 a d d N o d e ( & c u r N o d e - > l e f t ,   d a t a ) ; 
 
 	 	 	 } 
 
 
 
 	 	 	 r e b a l a n c e N o d e ( c u r N o d e ) ; 
 
 	 	 } 
 
 	 } 
 
 	 e l s e   { 
 
 	 	 * n o d e   =   c r e a t e N o d e ( d a t a ) ; 
 
 	 	 ( * n o d e ) - > h e i g h t + + ; 
 
 	 } 
 
 } 
 
 
 
 v o i d   t r a v e r s e N o d e ( N o d e   * n o d e ) 
 
 { 
 
 	 i f   ( n o d e ) 
 
 	 { 
 
 	 	 p r i n t f ( " % 0 2 d " ,   n o d e - > d a t a ) ; 	 	 	 	 
 
 	 	 i f   ( n o d e - > r i g h t ) 
 
 	 	 	 p r i n t f ( " - > " ) ; 
 
 	 	 t r a v e r s e N o d e ( n o d e - > r i g h t ) ; 
 
 	 	 i f   ( n o d e - > l e f t ) 
 
 	 	 	 p r i n t f ( " \ n | \ n " ) ; 
 
 	 	 t r a v e r s e N o d e ( n o d e - > l e f t ) ; 
 
 	 } 	 
 
 } 
 
 
 
 
 
 
 
 N o d e *   f i n d N o d e ( N o d e   * * n o d e ,   i n t   d a t a ) 
 
 { 
 
 	 N o d e   * c u r N o d e   =   * n o d e ;   
 
 
 
 	 i f   ( c u r N o d e )   { 
 
 	 	 i f   ( d a t a   = =   c u r N o d e - > d a t a )   { 
 
 	 	 	 r e t u r n   c u r N o d e ; 
 
 	 	 } 
 
 	 	 e l s e   i f   ( d a t a   <   c u r N o d e - > d a t a )   { 
 
 	 	 	 r e t u r n   f i n d N o d e ( & c u r N o d e - > l e f t ,   d a t a ) ; 
 
 	 	 } 
 
 	 	 e l s e   i f   ( d a t a   >   c u r N o d e - > d a t a )   { 
 
 	 	 	 r e t u r n   f i n d N o d e ( & c u r N o d e - > r i g h t ,   d a t a ) ; 
 
 	 	 } 
 
 	 } 
 
 
 
 	 r e t u r n   N U L L ; 
 
 } 
 
 
 
 N o d e *   f i n d P a r e n t N o d e ( N o d e   * * n o d e ,   i n t   d a t a ) 
 
 { 
 
 	 N o d e   * c u r N o d e   =   * n o d e ; 
 
 
 
 	 i f   ( c u r N o d e   & &   ( d a t a   = =   c u r N o d e - > d a t a ) ) 
 
 	 	 r e t u r n   N U L L ;   
 
 
 
 	 i f   ( c u r N o d e )   { 
 
 	 	 i f   ( ( c u r N o d e - > l e f t   & &   ( d a t a   = =   c u r N o d e - > l e f t - > d a t a ) )   
 
 	 	 	 | | ( c u r N o d e - > r i g h t   & &   ( d a t a   = =   c u r N o d e - > r i g h t - > d a t a ) ) ) { 
 
 	 	 	 r e t u r n   c u r N o d e ; 
 
 	 	 } 
 
 	 	 e l s e   i f   ( d a t a   <   c u r N o d e - > d a t a )   { 
 
 	 	 	 r e t u r n   f i n d P a r e n t N o d e ( & c u r N o d e - > l e f t ,   d a t a ) ; 
 
 	 	 } 
 
 	 	 e l s e   i f   ( d a t a   >   c u r N o d e - > d a t a )   { 
 
 	 	 	 r e t u r n   f i n d P a r e n t N o d e ( & c u r N o d e - > r i g h t ,   d a t a ) ; 
 
 	 	 } 
 
 	 } 
 
 
 
 	 r e t u r n   N U L L ;   
 
 } 
 
 
 
 N o d e *   f i n d L a r g e s t N o d e ( N o d e   * n o d e ) 
 
 { 
 
 	 i f   ( n o d e )   { 
 
 	 	 i f   ( n o d e - > r i g h t )   { 
 
 	 	 	 r e t u r n   f i n d L a r g e s t N o d e ( n o d e - > r i g h t ) ; 
 
 	 	 } 
 
 	 	 e l s e   { 
 
 	 	 	 r e t u r n   n o d e ; 
 
 	 	 } 
 
 	 }   
 
 
 
 	 r e t u r n   N U L L ; 
 
 } 
 
 
 
 
 
 / /   r e m o v e N o d e   n e e d s   t o   b e   r e c u r s i v e   t o   r e b a l c e   a f t e r   n o d e   r e m o v a l 
 
 v o i d   r e m o v e N o d e ( N o d e   * * n o d e ,   i n t   d a t a ) 
 
 { 
 
 	 N o d e   * c u r N o d e   =   * n o d e ; 
 
 
 
 	 i f   ( c u r N o d e )   
 
 	 { 
 
 	 	 i f   ( c u r N o d e - > d a t a   ! =   d a t a ) 
 
 	 	 { 	 
 
 	 	 	 N o d e   * p a r e n t   =   c u r N o d e ;   
 
 	 	 	 N o d e   * l e f t   =   c u r N o d e - > l e f t ; 
 
 	 	 	 N o d e   * r i g h t   =   c u r N o d e - > r i g h t ; 
 
 	 	 	 N o d e   * n o d e T o R e m o v e   =   N U L L ; 
 
 	 	 	 N o d e   * n o d e T o F r e e   =   N U L L ; 
 
 
 
 	 	 	 i f   ( ( l e f t   & &   ( d a t a   = =   l e f t - > d a t a ) )   | |   ( r i g h t   & &   ( d a t a   = =   r i g h t - > d a t a ) ) ) 
 
 	 	 	 { 
 
 	 	 	 	 n o d e T o R e m o v e   =   ( d a t a   >   c u r N o d e - > d a t a )   ?   r i g h t   :   l e f t ; 	 	 	 	 
 
 
 
 	 	 	 	 / /   C a s e   1   :   L e a f   N o d e 
 
 	 	 	 	 i f   ( ! n o d e T o R e m o v e - > l e f t   & &   ! n o d e T o R e m o v e - > r i g h t ) 
 
 	 	 	 	 { 
 
 	 	 	 	 	 i f   ( p a r e n t - > l e f t   = =   n o d e T o R e m o v e ) 
 
 	 	 	 	 	 { 
 
 	 	 	 	 	 	 p a r e n t - > l e f t   =   N U L L ; 
 
 	 	 	 	 	 } 
 
 	 	 	 	 	 e l s e   { 
 
 	 	 	 	 	 	 p a r e n t - > r i g h t   =   N U L L ; 
 
 	 	 	 	 	 } 
 
 	 	 	 	 	 f r e e ( n o d e T o R e m o v e ) ; 
 
 	 	 	 	 }   / /   C a s e   2   :   H a s   a   c h i l d   w i t h   s m a l l e r   d a t a   
 
 	 	 	 	 e l s e   i f   ( n o d e T o R e m o v e - > l e f t )   
 
 	 	 	 	 { 
 
 	 	 	 	 	 / / r e p l a c e   t h e   l a r g e s t   d a t a   l e s s   t h a n   c u r r e n t 
 
 	 	 	 	 	 n o d e T o F r e e   =   f i n d L a r g e s t N o d e ( n o d e T o R e m o v e - > l e f t ) ; 
 
 
 
 	 	 	 	 	 n o d e T o R e m o v e - > d a t a   =   n o d e T o F r e e - > d a t a ; 
 
 
 
 	 	 	 	 	 r e m o v e N o d e ( & n o d e T o R e m o v e - > l e f t ,   n o d e T o F r e e - > d a t a ) ; 
 
 
 
 	 	 	 	 }   / /   C a s e   3   :   H a s   n o   c h i l d   w i t h   s m a l l e r   d a t a 
 
 	 	 	 	 e l s e   i f   ( n o d e T o R e m o v e - > r i g h t   & &   ! n o d e T o R e m o v e - > l e f t )   
 
 	 	 	 	 { 
 
 	 	 	 	 	 i f   ( d a t a   >   p a r e n t - > d a t a )   { 
 
 	 	 	 	 	 	 p a r e n t - > r i g h t   =   n o d e T o R e m o v e - > r i g h t ; 
 
 	 	 	 	 	 } 
 
 	 	 	 	 	 e l s e 
 
 	 	 	 	 	 { 
 
 	 	 	 	 	 	 p a r e n t - > l e f t   =   n o d e T o R e m o v e - > r i g h t ; 
 
 	 	 	 	 	 } 
 
 
 
 	 	 	 	 	 f r e e ( n o d e T o R e m o v e ) ; 
 
 	 	 	 	 }   
 
 	 	 	 } 
 
 	 	 	 e l s e   
 
 	 	 	 { 
 
 	 	 	 	 i f   ( d a t a   >   c u r N o d e - > d a t a ) 
 
 	 	 	 	 { 
 
 	 	 	 	 	 r e m o v e N o d e ( & c u r N o d e - > r i g h t ,   d a t a ) ; 
 
 	 	 	 	 } 
 
 	 	 	 	 e l s e   
 
 	 	 	 	 { 
 
 	 	 	 	 	 r e m o v e N o d e ( & c u r N o d e - > l e f t ,   d a t a ) ;   
 
 	 	 	 	 } 
 
 	 	 	 } 
 
 
 
 	 	 	 r e b a l a n c e N o d e ( c u r N o d e ) ; 
 
 	 	 } 
 
 	 	 e l s e   
 
 	 	 { 
 
 	 	 	 / /   T h i s   i s   t h e   r o o t   n o d e 
 
 
 
 	 	 	 / /   C a s e   1   h a s   n o   c h i l d r e n 
 
 	 	 	 i f   ( ! c u r N o d e - > l e f t   & &   ! c u r N o d e - > r i g h t ) 
 
 	 	 	 { 
 
 	 	 	 	 f r e e ( c u r N o d e ) ; 
 
 	 	 	 	 * n o d e   =   N U L L ;   
 
 	 	 	 }   / /   C a s e   2   h a s   a   c h i l d   w i t h   s m a l l e r   d a t a 
 
 	 	 	 e l s e   i f   ( c u r N o d e - > l e f t ) 
 
 	 	 	 { 
 
 	 	 	 	 N o d e   * n o d e T o F r e e   =   f i n d L a r g e s t N o d e ( c u r N o d e - > l e f t ) ; 
 
 
 
 	 	 	 	 c u r N o d e - > d a t a   =   n o d e T o F r e e - > d a t a ; 
 
 
 
 	 	 	 	 r e m o v e N o d e ( & c u r N o d e - > l e f t ,   n o d e T o F r e e - > d a t a ) ; 
 
 
 
 	 	 	 	 r e b a l a n c e N o d e ( c u r N o d e ) ; 
 
 	 	 	 }   / /   C a s e   3   h a s   n o   c h i l d   w i t h   s m a l l e r   d a t a 
 
 	 	 	 e l s e   i f   ( c u r N o d e - > r i g h t ) 
 
 	 	 	 { 
 
 	 	 	 	 * n o d e   =   c u r N o d e - > r i g h t ; 
 
 	 	 	 	 f r e e ( c u r N o d e ) ; 
 
 	 	 	 	 r e b a l a n c e N o d e ( * n o d e ) ; 
 
 	 	 	 } 	 	 	 
 
 	 	 } 	 
 
 	 } 	 
 
 } 
 
 
 
 i n t   m a i n ( ) 
 
 { 
 
 	 N o d e   * t r e e   =   N U L L ;   
 
 
 
 	 i n t   c o u n t   =   0 ;   
 
 
 
         s t d : : c o u t   < <   " H e l l o   W o r l d ! \ n " ;   
 
 
 
 	 s r a n d ( ( u n s i g n e d ) t i m e ( N U L L ) ) ; 
 
 
 
 	 d o   { 
 
 	 	 i n t   d a t a   =   r a n d ( )   %   1 0 0 ;   
 
 	 	 p r i n t f ( " % d \ n " ,   d a t a ) ; 
 
 	 	 a d d N o d e ( & t r e e ,   d a t a ) ; 
 
 	 }   w h i l e   ( c o u n t + +   <   2 0 ) ; 
 
 	 	 
 
 	 p r i n t f ( " \ n T r a v e r s i n g   \ n " ) ; 
 
 	 t r a v e r s e N o d e ( t r e e ) ; 
 
 
 
 	 b f s T r e e ( t r e e ) ; 
 
 	 
 
 	 c o u n t   =   0 ;   
 
 
 
 	 d o   { 
 
 
 
 	 	 i n t   d a t a   =   r a n d ( )   %   1 0 0 ; 	 	 
 
 	 	 p r i n t f ( " \ n t r y   t o   r e m o v e   % d \ n " ,   d a t a ) ; 
 
 	 	 r e m o v e N o d e ( & t r e e ,   d a t a ) ; 
 
 	 	 p r i n t f ( " \ n T r a v e r s i n g   a f t e r   r e m o v a l \ n " ) ; 
 
 	 	 t r a v e r s e N o d e ( t r e e ) ; 
 
 	 	 b f s T r e e ( t r e e ) ; 
 
 
 
 	 	 i f   ( ( c o u n t   %   5 )   = =   0 ) 
 
 	 	 { 
 
 	 	 	 p r i n t f ( " \ n A d d   % d \ n " ,   d a t a   =   r a n d ( )   %   1 0 0 ) ; 
 
 	 	 	 a d d N o d e ( & t r e e ,   d a t a ) ; 
 
 	 	 	 t r a v e r s e N o d e ( t r e e ) ; 
 
 	 	 	 b f s T r e e ( t r e e ) ; 
 
 	 	 } 
 
 	 }   w h i l e   ( t r e e   & &   c o u n t + +   <   8 0 ) ; 
 
 } 
 
 
 
 / /   R u n   p r o g r a m :   C t r l   +   F 5   o r   D e b u g   >   S t a r t   W i t h o u t   D e b u g g i n g   m e n u 
 
 / /   D e b u g   p r o g r a m :   F 5   o r   D e b u g   >   S t a r t   D e b u g g i n g   m e n u 
 
 
 
 / /   T i p s   f o r   G e t t i n g   S t a r t e d :   
 
 / /       1 .   U s e   t h e   S o l u t i o n   E x p l o r e r   w i n d o w   t o   a d d / m a n a g e   f i l e s 
 
 / /       2 .   U s e   t h e   T e a m   E x p l o r e r   w i n d o w   t o   c o n n e c t   t o   s o u r c e   c o n t r o l 
 
 / /       3 .   U s e   t h e   O u t p u t   w i n d o w   t o   s e e   b u i l d   o u t p u t   a n d   o t h e r   m e s s a g e s 
 
 / /       4 .   U s e   t h e   E r r o r   L i s t   w i n d o w   t o   v i e w   e r r o r s 
 
 / /       5 .   G o   t o   P r o j e c t   >   A d d   N e w   I t e m   t o   c r e a t e   n e w   c o d e   f i l e s ,   o r   P r o j e c t   >   A d d   E x i s t i n g   I t e m   t o   a d d   e x i s t i n g   c o d e   f i l e s   t o   t h e   p r o j e c t 
 
 / /       6 .   I n   t h e   f u t u r e ,   t o   o p e n   t h i s   p r o j e c t   a g a i n ,   g o   t o   F i l e   >   O p e n   >   P r o j e c t   a n d   s e l e c t   t h e   . s l n   f i l e 
 
 