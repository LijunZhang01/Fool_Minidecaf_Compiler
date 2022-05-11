    memo 'T1:4 T0:0 '           
_sum:                           
    T2 <- (T1 + T0)             
    return T2                   
    T3 <- 0                     
    return T3                   

    memo ''                     
_main:                          
    T5 <- 2                     
    push   T5                   
    T6 <- 1                     
    push   T6                   
    T7 <- call _sum'            
    T8 <- 2                     
    push   T8                   
    T9 <- 1                     
    push   T9                   
    T10 <- call _sum'           
    T11 <- 2                    
    T12 <- (T10 / T11)          
    T13 <- 2                    
    T14 <- (T12 * T13)          
    T15 <- (T7 - T14)           
    T4 <- T15                   
    T17 <- 2                    
    T18 <- 4                    
    push   T18                  
    T19 <- 3                    
    push   T19                  
    T20 <- call _sum'           
    T21 <- (T17 * T20)          
    T22 <- 2                    
    push   T22                  
    T23 <- 1                    
    push   T23                  
    T24 <- call _sum'           
    T25 <- (T21 + T24)          
    T16 <- T25                  
    T26 <- (T16 - T4)           
    return T26                  
    T27 <- 0                    
    return T27                  


