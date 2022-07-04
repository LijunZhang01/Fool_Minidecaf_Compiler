          .text                         
          .data                         
          .globl main                   
          .align 2                      

          .text                         
_FourWhile:                             # function entry
          sw    ra, -4(sp)              
          sw    fp, -8(sp)              
          mv    fp, sp                  
          addi  sp, sp, -24             
__LL0:                                  
                                  # þ
          li    t0, 5                   
                                  # þ
          mv    t0, t0                  
                                  # þ
          li    t1, 6                   
                                  # þ
          mv    t1, t1                  
                                  # þ
          li    t2, 7                   
                                  # þ
          mv    t2, t2                  
                                  # þ
          li    t3, 10                  
                                  # þ
          mv    t3, t3                  
                                  # (save modified registers before control flow changes)
          sw    t0, -24(fp)             # spill T0 from t0 to (fp-24)
          sw    t1, -20(fp)             # spill T2 from t1 to (fp-20)
          sw    t2, -16(fp)             # spill T3 from t2 to (fp-16)
          sw    t3, -12(fp)             # spill T6 from t3 to (fp-12)
          j     __LL1                   
__LL1:                                  
                                  # 
          li    t1, 20                  
                                  # T9 <- (T0 < T8)
          lw    a5, -24(fp)             # load T0 from (fp-24) into a5
          slt   t1, a5, t1              
          beqz  t1, __LL12              
          j     __LL2                   
__LL2:                                  
                                  # 
          li    t1, 3                   
                                  # T11 <- (T0 + T10)
          lw    a5, -24(fp)             # load T0 from (fp-24) into a5
          add   t0, a5, t1              
                                  # 
          mv    t0, t0                  
                                  # (save modified registers before control flow changes)
          sw    t0, -24(fp)             # spill T0 from t0 to (fp-24)
          j     __LL3                   
__LL3:                                  
                                  # 
          li    t1, 10                  
                                  # T13 <- (T2 < T12)
          lw    a5, -20(fp)             # load T2 from (fp-20) into a5
          slt   t1, a5, t1              
          beqz  t1, __LL11              
          j     __LL4                   
__LL4:                                  
                                  # 
          li    t1, 1                   
                                  # T15 <- (T2 + T14)
          lw    a5, -20(fp)             # load T2 from (fp-20) into a5
          add   t0, a5, t1              
                                  # 
          mv    t0, t0                  
                                  # (save modified registers before control flow changes)
          sw    t0, -20(fp)             # spill T2 from t0 to (fp-20)
          j     __LL5                   
__LL5:                                  
                                  # 
          li    t1, 7                   
                                  # T17 <- (T3 == T16)
          lw    a5, -16(fp)             # load T3 from (fp-16) into a5
          xor   t1, a5, t1              
          seqz  t1, t1                  
          beqz  t1, __LL10              
          j     __LL6                   
__LL6:                                  
                                  # 
          li    t1, 1                   
                                  # T19 <- (T3 - T18)
          lw    a5, -16(fp)             # load T3 from (fp-16) into a5
          sub   t0, a5, t1              
                                  # 
          mv    t0, t0                  
                                  # (save modified registers before control flow changes)
          sw    t0, -16(fp)             # spill T3 from t0 to (fp-16)
          j     __LL7                   
__LL7:                                  
                                  # 
          li    t1, 20                  
                                  # T21 <- (T6 < T20)
          lw    a5, -12(fp)             # load T6 from (fp-12) into a5
          slt   t1, a5, t1              
          beqz  t1, __LL9               
          j     __LL8                   
__LL8:                                  
                                  # 
          li    t1, 3                   
                                  # T23 <- (T6 + T22)
          lw    a5, -12(fp)             # load T6 from (fp-12) into a5
          add   t0, a5, t1              
                                  # 
          mv    t0, t0                  
                                  # (save modified registers before control flow changes)
          sw    t0, -12(fp)             # spill T6 from t0 to (fp-12)
          j     __LL7                   
__LL9:                                  
                                  # þ
          li    t1, 1                   
                                  # T25 <- (T6 - T24)
          lw    a5, -12(fp)             # load T6 from (fp-12) into a5
          sub   t0, a5, t1              
                                  # þ
          mv    t0, t0                  
                                  # (save modified registers before control flow changes)
          sw    t0, -12(fp)             # spill T6 from t0 to (fp-12)
          j     __LL5                   
__LL10:                                 
                                  # þ
          li    t1, 1                   
                                  # T27 <- (T3 + T26)
          lw    a5, -16(fp)             # load T3 from (fp-16) into a5
          add   t0, a5, t1              
                                  # þ
          mv    t0, t0                  
                                  # (save modified registers before control flow changes)
          sw    t0, -16(fp)             # spill T3 from t0 to (fp-16)
          j     __LL3                   
__LL11:                                 
                                  # þ
          li    t1, 2                   
                                  # T29 <- (T2 - T28)
          lw    a5, -20(fp)             # load T2 from (fp-20) into a5
          sub   t0, a5, t1              
                                  # þ
          mv    t0, t0                  
                                  # (save modified registers before control flow changes)
          sw    t0, -20(fp)             # spill T2 from t0 to (fp-20)
          j     __LL1                   
__LL12:                                 
                                  # T30 <- (T2 + T6)
          lw    a5, -20(fp)             # load T2 from (fp-20) into a5
          lw    a6, -12(fp)             # load T6 from (fp-12) into a6
          add   t0, a5, a6              
                                  # T31 <- (T0 + T30)
          lw    a5, -24(fp)             # load T0 from (fp-24) into a5
          add   t0, a5, t0              
                                  # T32 <- (T31 + T3)
          lw    a5, -16(fp)             # load T3 from (fp-16) into a5
          add   t0, t0, a5              
          mv    a0, t0                  
          mv    sp, fp                  
          lw    ra, -4(fp)              
          lw    fp, -8(fp)              
          ret                           

          .text                         
main:                                   # function entry
          sw    ra, -4(sp)              
          sw    fp, -8(sp)              
          mv    fp, sp                  
          addi  sp, sp, -8              
__LL13:                                 
                                  # T34 <- call _FourWhile'
          call  _FourWhile              
          mv    t0, a0                  
          mv    a0, t0                  
          mv    sp, fp                  
          lw    ra, -4(fp)              
          lw    fp, -8(fp)              
          ret                           
