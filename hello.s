    T0 <- 3                     
    T1 <- 8                     
    memo 'T4:8 T3:4 T2:0 '      
_sub_3:                         
    T5 <- (T4 - T3)             
    T6 <- (T5 - T2)             
    return T6                   
    T7 <- 0                     
    return T7                   

    memo ''                     
_main:                          
    T9 <- 7                     
    T8 <- T9                    
    T11 <- LOAD_SYMBOL a        
    T12 <- LOAD T11, 0          
    T13 <- LOAD_SYMBOL b        
    T14 <- LOAD T13, 0          
    T15 <- (T12 + T14)          
    T16 <- (T15 + T8)           
    T10 <- T16                  
    return T10                  
    T17 <- 0                    
    return T17                  


