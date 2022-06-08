    T0 <- 18                    
    T1 <- 1000000007            
    memo ''                     
_main:                          
    T3 <- 5                     
    T2 <- T3                    
    T5 <- 0                     
    T4 <- T5                    
    T6 <- 1                     
    T7 <- 3                     
    T8 <- 6                     
    T9 <- (T7 * T8)             
    T10 <- (T6 + T9)            
    T11 <- 2                    
    T12 <- 2                    
    T13 <- (T11 * T12)          
    T14 <- (T10 + T13)          
    T15 <- 4                    
    T16 <- (T14 * T15)          
    T17 <- 1                    
    T18 <- (- T17)              
    T19 <- LOAD_SYMBOL dp       
    T20 <- (T19 + T16)          
    STORE T18 -> T20, 0         
    T21 <- 5                    
    return T21                  
    T22 <- 0                    
    return T22                  


