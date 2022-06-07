    memo 'T2:8 T1:4 T0:0 '      
_set:                           
    T4 <- 0                     
    T3 <- T4                    
    T6 <- 4                     
    T5 <- T6                    
    T7 <- (T1 / T5)             
    T8 <- 4                     
    T9 <- (T7 * T8)             
    T10 <- (T1 / T5)            
    T11 <- 4                    
    T12 <- (T10 * T11)          
    T13 <- (T2 + T12)           
    T14 <- LOAD T13, 0          
    T15 <- (T14 + T3)           
    T16 <- (T2 + T9)            
    STORE T15 -> T16, 0         
    T17 <- 0                    
    return T17                  
    T18 <- 0                    
    return T18                  

    memo ''                     
_main:                          
    T19 = ALLOC 16              
    T20 <- 1                    
    STORE T20 -> T19, 0         
    T21 <- 2                    
    STORE T21 -> T19, 4         
    T22 <- 3                    
    STORE T22 -> T19, 8         
    T23 <- 4                    
    STORE T23 -> T19, 12        
    T25 <- LOAD_SYMBOL a        
    T26 <- LOAD T25, 0          
    push   T26                  
    T27 <- 2                    
    push   T27                  
    T28 <- 3                    
    push   T28                  
    T29 <- call _set'           
    T24 <- T29                  
    return T24                  
    T30 <- 0                    
    return T30                  


