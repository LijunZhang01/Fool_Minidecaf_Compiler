          .text                         
          .data                         
          .global                       dp
dp:          .zero                         384
          .globl main                   
          .align 2                      

          .text                         
main:                                   # function entry
          sw    ra, -4(sp)              
          sw    fp, -8(sp)              
          mv    fp, sp                  
          addi  sp, sp, -8              
__LL0:                                  
                                  # ÿ
          li    t0, 0                   
                                  # ÿ
          li    t1, 1                   
                                  # ÿ
          li    t2, 2                   
                                  # T3 <- (T0 * T2)
          mul   s1, t0, t2              
                                  # T4 <- (T3 + T1)
          add   s2, s1, t1              
                                  # ÿ
          li    s3, 0                   
                                  # ÿ
          li    s4, 2                   
                                  # T7 <- (T4 * T6)
          mul   s5, s2, s4              
                                  # T8 <- (T7 + T5)
          add   s6, s5, s3              
                                  # ÿ
          li    s7, 0                   
                                  # ÿ
          li    s8, 2                   
                                  # T11 <- (T8 * T10)
          mul   s9, s6, s8              
                                  # T12 <- (T11 + T9)
          add   s10, s9, s7             
                                  # ÿ
          li    s11, 0                  
                                  # ÿ
          li    t3, 2                   
                                  # T15 <- (T12 * T14)
          mul   t4, s10, t3             
                                  # T16 <- (T15 + T13)
          add   t5, t4, s11             
                                  # ÿ
          li    t6, 1                   
                                  # ÿ
          li    t0, 3                   
                                  # T19 <- (T16 * T18)
          mul   t1, t5, t0              
                                  # T20 <- (T19 + T17)
          add   t0, t1, t6              
                                  # ÿ
          li    t1, 4                   
                                  # T22 <- (T20 * T21)
          mul   t2, t0, t1              
                                  # ÿ
          li    t0, 13                  
                                  # T24 <- LOAD_SYMBOL dp
          la    t1, dp                  
                                  # T25 <- (T24 + T22)
          add   s1, t1, t2              
                                  # STORE T23 -> T25, 0
          sw    t0, 0(s1)               
                                  # ÿ
          li    t0, 1                   
                                  # ÿ
          li    t1, 0                   
                                  # ÿ
          li    t2, 2                   
                                  # T29 <- (T26 * T28)
          mul   s1, t0, t2              
                                  # T30 <- (T29 + T27)
          add   t0, s1, t1              
                                  # ÿ
          li    t1, 0                   
                                  # ÿ
          li    t2, 2                   
                                  # T33 <- (T30 * T32)
          mul   s1, t0, t2              
                                  # T34 <- (T33 + T31)
          add   t0, s1, t1              
                                  # ÿ
          li    t1, 0                   
                                  # ÿ
          li    t2, 2                   
                                  # T37 <- (T34 * T36)
          mul   s1, t0, t2              
                                  # T38 <- (T37 + T35)
          add   t0, s1, t1              
                                  # ÿ
          li    t1, 0                   
                                  # ÿ
          li    t2, 2                   
                                  # T41 <- (T38 * T40)
          mul   s1, t0, t2              
                                  # T42 <- (T41 + T39)
          add   t0, s1, t1              
                                  # ÿ
          li    t1, 1                   
                                  # ÿ
          li    t2, 3                   
                                  # T45 <- (T42 * T44)
          mul   s1, t0, t2              
                                  # T46 <- (T45 + T43)
          add   t0, s1, t1              
                                  # ÿ
          li    t1, 4                   
                                  # T48 <- (T46 * T47)
          mul   t2, t0, t1              
                                  # ÿ
          li    t0, 12                  
                                  # T50 <- LOAD_SYMBOL dp
          la    t1, dp                  
                                  # T51 <- (T50 + T48)
          add   s1, t1, t2              
                                  # STORE T49 -> T51, 0
          sw    t0, 0(s1)               
                                  # ÿ
          li    t0, 0                   
                                  # ÿ
          li    t1, 1                   
                                  # ÿ
          li    t2, 2                   
                                  # T55 <- (T52 * T54)
          mul   s1, t0, t2              
                                  # T56 <- (T55 + T53)
          add   t0, s1, t1              
                                  # ÿ
          li    t1, 0                   
                                  # ÿ
          li    t2, 2                   
                                  # T59 <- (T56 * T58)
          mul   s1, t0, t2              
                                  # T60 <- (T59 + T57)
          add   t0, s1, t1              
                                  # ÿ
          li    t1, 0                   
                                  # ÿ
          li    t2, 2                   
                                  # T63 <- (T60 * T62)
          mul   s1, t0, t2              
                                  # T64 <- (T63 + T61)
          add   t0, s1, t1              
                                  # ÿ
          li    t1, 0                   
                                  # ÿ
          li    t2, 2                   
                                  # T67 <- (T64 * T66)
          mul   s1, t0, t2              
                                  # T68 <- (T67 + T65)
          add   t0, s1, t1              
                                  # ÿ
          li    t1, 1                   
                                  # ÿ
          li    t2, 3                   
                                  # T71 <- (T68 * T70)
          mul   s1, t0, t2              
                                  # T72 <- (T71 + T69)
          add   t0, s1, t1              
                                  # ÿ
          li    t1, 4                   
                                  # T74 <- (T72 * T73)
          mul   t2, t0, t1              
                                  # T75 <- LOAD_SYMBOL dp
          la    t0, dp                  
                                  # T76 <- (T75 + T74)
          add   t1, t0, t2              
                                  # T77 <- LOAD T76, 0
          lw    t0, 0(t1)               
          mv    a0, t0                  
          mv    sp, fp                  
          lw    ra, -4(fp)              
          lw    fp, -8(fp)              
          ret                           
