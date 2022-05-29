    memo ''                     
_main:                          
    T0 <- call _getint'         
    T1 <- LOAD_SYMBOL a         
    STORE T0 -> T1, 0           
    T2 <- call _getint'         
    T3 <- LOAD_SYMBOL b         
    STORE T2 -> T3, 0           
    T4 <- call _getint'         
    T5 <- LOAD_SYMBOL c         
    STORE T4 -> T5, 0           
    T6 <- call _getint'         
    T7 <- LOAD_SYMBOL d         
    STORE T6 -> T7, 0           
    T8 <- call _getint'         
    T9 <- LOAD_SYMBOL e         
    STORE T8 -> T9, 0           
    T11 <- 0                    
    T10 <- T11                  
    T14 <- LOAD_SYMBOL a        
    T15 <- LOAD T14, 0          
    T16 <- LOAD_SYMBOL b        
    T17 <- LOAD T16, 0          
    T18 <- LOAD_SYMBOL c        
    T19 <- LOAD T18, 0          
    T20 <- (T17 * T19)          
    T21 <- (T15 - T20)          
    T22 <- LOAD_SYMBOL d        
    T23 <- LOAD T22, 0          
    T24 <- LOAD_SYMBOL a        
    T25 <- LOAD T24, 0          
    T26 <- LOAD_SYMBOL c        
    T27 <- LOAD T26, 0          
    T28 <- (T25 / T27)          
    T29 <- (T23 - T28)          
    T30 <- (T21 != T29)         
    if (T30 == 0) jump __L13    
    T31 <- 1                    
    T13 <- T31                  
    jump   __L14                
__L13:                          
    T32 <- LOAD_SYMBOL a        
    T33 <- LOAD T32, 0          
    T34 <- LOAD_SYMBOL b        
    T35 <- LOAD T34, 0          
    T36 <- (T33 * T35)          
    T37 <- LOAD_SYMBOL c        
    T38 <- LOAD T37, 0          
    T39 <- (T36 / T38)          
    T40 <- LOAD_SYMBOL e        
    T41 <- LOAD T40, 0          
    T42 <- LOAD_SYMBOL d        
    T43 <- LOAD T42, 0          
    T44 <- (T41 + T43)          
    T45 <- (T39 == T44)         
    T46 <- (T30 || T45)         
    T13 <- T46                  
__L14:                          
    if (T13 == 0) jump __L11    
    T47 <- 1                    
    T12 <- T47                  
    jump   __L12                
__L11:                          
    T48 <- LOAD_SYMBOL a        
    T49 <- LOAD T48, 0          
    T50 <- LOAD_SYMBOL b        
    T51 <- LOAD T50, 0          
    T52 <- (T49 + T51)          
    T53 <- LOAD_SYMBOL c        
    T54 <- LOAD T53, 0          
    T55 <- (T52 + T54)          
    T56 <- LOAD_SYMBOL d        
    T57 <- LOAD T56, 0          
    T58 <- LOAD_SYMBOL e        
    T59 <- LOAD T58, 0          
    T60 <- (T57 + T59)          
    T61 <- (T55 == T60)         
    T62 <- (T13 || T61)         
    T12 <- T62                  
__L12:                          
    if (T12 == 0) jump __L9     
    T63 <- 1                    
    T10 <- T63                  
    jump   __L10                
__L9:                           
__L10:                          
    return T10                  
    T64 <- 0                    
    return T64                  


