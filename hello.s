    memo 'T0:0 '                
_sum:                           
    T1 <- 2                     
    T2 <- 4                     
    T3 <- (T1 * T2)             
    T4 <- (T0 + T3)             
    T5 <- LOAD T4, 0            
    return T5                   
    T6 <- 0                     
    return T6                   

    memo ''                     
_main:                          
    T7 = ALLOC 16               
    T8 <- 1                     
    STORE T8 -> T7, 0           
    T9 <- 2                     
    STORE T9 -> T7, 4           
    T10 <- 3                    
    STORE T10 -> T7, 8          
    T11 <- 4                    
    STORE T11 -> T7, 12         
    push   T7                   
    T13 <- call _sum'           
    T12 <- T13                  
    return T12                  
    T14 <- 0                    
    return T14                  


