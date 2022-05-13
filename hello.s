    memo 'T2:8 T1:4 T0:0 '      
_sub_3:                         
    T3 <- (T2 - T1)             
    T4 <- (T3 - T0)             
    return T4                   
    T5 <- 0                     
    return T5                   

    memo ''                     
_main:                          
    T7 <- 3                     
    T6 <- T7                    
    T9 <- 7                     
    T8 <- T9                    
    T11 <- 10                   
    push   T11                  
    T12 <- 4                    
    push   T12                  
    T13 <- 2                    
    push   T13                  
    T14 <- call _sub_3'         
    T10 <- T14                  
    T16 <- (T6 + T10)           
    T17 <- (T16 + T8)           
    T15 <- T17                  
    return T15                  
    T18 <- 0                    
    return T18                  


