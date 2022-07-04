          .text                         
          .data                         
          .globl main                   
          .align 2                      

          .text                         
_ifElseIf:                              # function entry
          sw    ra, -4(sp)              
          sw    fp, -8(sp)              
          mv    fp, sp                  
          addi  sp, sp, -40             
__LL0:                                  
                                  # ü
          li    t0, 5                   
                                  # ü
          mv    t0, t0                  
                                  # ü
          li    t1, 10                  
                                  # ü
          mv    t1, t1                  
                                  # ü
          li    t2, 6                   
                                  # T6 <- (T0 == T5)
          xor   t2, t0, t2              
          seqz  t2, t2                  
                                  # (save modified registers before control flow changes)
          sw    t0, -20(fp)             # spill T0 from t0 to (fp-20)
          sw    t1, -16(fp)             # spill T2 from t1 to (fp-16)
          sw    t2, -12(fp)             # spill T6 from t2 to (fp-12)
          beqz  t2, __LL2               
          j     __LL1                   
__LL1:                                  
                                  # 
          li    t0, 1                   
                                  # (save modified registers before control flow changes)
          sw    t0, -24(fp)             # spill T4 from t0 to (fp-24)
          j     __LL3                   
__LL3:                                  
          beqz  t0, __LL5               
          j     __LL4                   
__LL4:                                  
          mv    a0, t0                  
          mv    sp, fp                  
          lw    ra, -4(fp)              
          lw    fp, -8(fp)              
          ret                           
__LL2:                                  
                                  # ü
          li    t2, 11                  
                                  # T8 <- (T2 == T7)
          xor   t2, t1, t2              
          seqz  t2, t2                  
                                  # T9 <- (T6 || T8)
          or    t0, t0, t2              
          snez  t0, t0                  
                                  # ü
          mv    t0, t0                  
                                  # (save modified registers before control flow changes)
          sw    t0, -24(fp)             # spill T4 from t0 to (fp-24)
          j     __LL3                   
__LL5:                                  
                                  # ü
          li    t1, 10                  
                                  # T12 <- (T2 == T11)
          xor   t1, t0, t1              
          seqz  t1, t1                  
                                  # (save modified registers before control flow changes)
          sw    t1, -28(fp)             # spill T12 from t1 to (fp-28)
          beqz  t1, __LL7               
          j     __LL6                   
__LL6:                                  
                                  # 
          li    t2, 1                   
                                  # T14 <- (T0 == T13)
          xor   t2, t0, t2              
          seqz  t2, t2                  
                                  # T15 <- (T12 && T14)
          snez  t1, t1                  
          snez  t1, t2                  
          and   t1, t1, t1              
                                  # 
          mv    t1, t1                  
                                  # (save modified registers before control flow changes)
          sw    t1, -32(fp)             # spill T10 from t1 to (fp-32)
          j     __LL8                   
__LL8:                                  
          beqz  t0, __LL10              
          j     __LL9                   
__LL9:                                  
                                  # 
          li    t0, 25                  
                                  # 
          mv    t0, t0                  
                                  # (save modified registers before control flow changes)
          sw    t0, -20(fp)             # spill T0 from t0 to (fp-20)
          j     __LL16                  
__LL16:                                 
          mv    a0, t0                  
          mv    sp, fp                  
          lw    ra, -4(fp)              
          lw    fp, -8(fp)              
          ret                           
__LL7:                                  
                                  # ü
          li    t0, 0                   
                                  # (save modified registers before control flow changes)
          sw    t0, -32(fp)             # spill T10 from t0 to (fp-32)
          j     __LL8                   
__LL10:                                 
                                  # ü
          li    t1, 10                  
                                  # T19 <- (T2 == T18)
          xor   t0, t0, t1              
          seqz  t0, t0                  
                                  # (save modified registers before control flow changes)
          sw    t0, -36(fp)             # spill T19 from t0 to (fp-36)
          beqz  t0, __LL12              
          j     __LL11                  
__LL11:                                 
                                  # 
          li    t2, 5                   
                                  # T21 <- (- T20)
          neg   t2, t2                  
                                  # T22 <- (T0 == T21)
          xor   t2, t1, t2              
          seqz  t2, t2                  
                                  # T23 <- (T19 && T22)
          snez  t0, t0                  
          snez  t0, t2                  
          and   t0, t0, t0              
                                  # 
          mv    t0, t0                  
                                  # (save modified registers before control flow changes)
          sw    t0, -40(fp)             # spill T17 from t0 to (fp-40)
          j     __LL13                  
__LL13:                                 
          beqz  t0, __LL15              
          j     __LL14                  
__LL14:                                 
                                  # 
          li    t1, 15                  
                                  # T25 <- (T0 + T24)
          add   t0, t0, t1              
                                  # 
          mv    t0, t0                  
                                  # (save modified registers before control flow changes)
          sw    t0, -20(fp)             # spill T0 from t0 to (fp-20)
          j     __LL16                  
__LL12:                                 
                                  # ü
          li    t0, 0                   
                                  # (save modified registers before control flow changes)
          sw    t0, -40(fp)             # spill T17 from t0 to (fp-40)
          j     __LL13                  
__LL15:                                 
                                  # T26 <- (- T0)
          neg   t0, t0                  
                                  # ü
          mv    t0, t0                  
                                  # (save modified registers before control flow changes)
          sw    t0, -20(fp)             # spill T0 from t0 to (fp-20)
          j     __LL16                  

          .text                         
main:                                   # function entry
          sw    ra, -4(sp)              
          sw    fp, -8(sp)              
          mv    fp, sp                  
          addi  sp, sp, -8              
__LL17:                                 
                                  # T28 <- call _ifElseIf'
          call  _ifElseIf               
          mv    t0, a0                  
                                  # ü
          mv    a0, t0                  
                                  # T29 <- call _putint'
          call  putint                  
          mv    t1, a0                  
                                  # ü
          li    t0, 0                   
          mv    a0, t0                  
          mv    sp, fp                  
          lw    ra, -4(fp)              
          lw    fp, -8(fp)              
          ret                           
