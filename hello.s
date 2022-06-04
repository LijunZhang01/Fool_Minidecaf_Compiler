          .text                         
          .data                         
          .global                       arr
arr:          .word                         1
          .word                         2
          .word                         33
          .word                         4
          .word                         5
          .word                         6
          .zero                         0
          .global                       N
N:
          .word                         -1
          .globl main                   
          .align 2                      

          .text                         
main:                                   # function entry
          sw    ra, -4(sp)              
          sw    fp, -8(sp)              
          mv    fp, sp                  
          addi  sp, sp, -16             
__LL0:                                  
                                  # ÿ
          li    t0, 0                   
                                  # ÿ
          mv    t1, t0                  
                                  # ÿ
          li    t2, 0                   
                                  # ÿ
          mv    s1, t2                  
                                  # (save modified registers before control flow changes)
          sw    t1, -16(fp)             # spill T2 from t1 to (fp-16)
          sw    s1, -12(fp)             # spill T4 from s1 to (fp-12)
          j     __LL1                   
__LL1:                                  
                                  # 
          li    t0, 6                   
                                  # T7 <- (T2 < T6)
          lw    t1, -16(fp)             # load T2 from (fp-16) into t1
          slt   t2, t1, t0              
          beqz  t2, __LL3               
          j     __LL2                   
__LL2:                                  
                                  # 
          li    t0, 4                   
                                  # T9 <- (T2 * T8)
          lw    t1, -16(fp)             # load T2 from (fp-16) into t1
          mul   t2, t1, t0              
                                  # T10 <- LOAD_SYMBOL arr
          la    s1, arr                 
                                  # T11 <- (T10 + T9)
          add   s2, s1, t2              
                                  # T12 <- LOAD T11, 0
          lw    s3, 0(s2)               
                                  # T13 <- (T4 + T12)
          lw    s4, -12(fp)             # load T4 from (fp-12) into s4
          add   s5, s4, s3              
                                  # 
          mv    s4, s5                  
                                  # 
          li    s6, 1                   
                                  # T15 <- (T2 + T14)
          add   s7, t1, s6              
                                  # 
          mv    t1, s7                  
                                  # (save modified registers before control flow changes)
          sw    t1, -16(fp)             # spill T2 from t1 to (fp-16)
          sw    s4, -12(fp)             # spill T4 from s4 to (fp-12)
          j     __LL1                   
__LL3:                                  
          lw    t0, -12(fp)             # load T4 from (fp-12) into t0
          mv    a0, t0                  
          mv    sp, fp                  
          lw    ra, -4(fp)              
          lw    fp, -8(fp)              
          ret                           
