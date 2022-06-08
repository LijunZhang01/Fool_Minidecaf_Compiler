          .text                         
          .data                         
          .global                       a
a:
          .word                         0
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
          li    t0, 10                  
                                  # T1 <- LOAD_SYMBOL a
          la    t1, a                   
                                  # T2 <- LOAD_SYMBOL a
          la    t2, a                   
                                  # T3 <- LOAD T2, 0
          lw    s1, 0(t2)               
                                  # ÿ
          li    s2, 0                   
                                  # T5 <- (T3 > T4)
          sgt   s3, s1, s2              
          beqz  s3, __LL2               
          j     __LL1                   
__LL1:                                  
                                  # 
          li    t0, 1                   
          mv    a0, t0                  
          mv    sp, fp                  
          lw    ra, -4(fp)              
          lw    fp, -8(fp)              
          ret                           
__LL2:                                  
                                  # ÿ
          li    t0, 0                   
          mv    a0, t0                  
          mv    sp, fp                  
          lw    ra, -4(fp)              
          lw    fp, -8(fp)              
          ret                           
