          .text                         
          .data                         
          .global                       b
b:
          .word                         0
          .global                       a
a:
          .word                         3
          .globl main                   
          .align 2                      

          .text                         
_sub_3:                                 # function entry
          sw    ra, -4(sp)              
          sw    fp, -8(sp)              
          mv    fp, sp                  
          addi  sp, sp, -8              
__LL0:                                  
                                  # T4 <- (T3 - T2)
          lw    t0, 8(fp)               # load T3 from (fp+8) into t0
          lw    t1, 4(fp)               # load T2 from (fp+4) into t1
          sub   t2, t0, t1              
                                  # T5 <- (T4 - T1)
          lw    s1, 0(fp)               # load T1 from (fp+0) into s1
          sub   s2, t2, s1              
          mv    a0, s2                  
          mv    sp, fp                  
          lw    ra, -4(fp)              
          lw    fp, -8(fp)              
          ret                           

          .text                         
main:                                   # function entry
          sw    ra, -4(sp)              
          sw    fp, -8(sp)              
          mv    fp, sp                  
          addi  sp, sp, -12             
__LL1:                                  
                                  # þ
          li    t0, 7                   
                                  # þ
          mv    t1, t0                  
                                  # þ
          li    t2, 10                  
                                  # þ
          addi  sp, sp, -4              
          sw    t2, 0(sp)               
                                  # þ
          li    s1, 4                   
                                  # þ
          addi  sp, sp, -4              
          sw    s1, 0(sp)               
                                  # þ
          li    s2, 2                   
                                  # þ
          addi  sp, sp, -4              
          sw    s2, 0(sp)               
                                  # T13 <- call _sub_3'
                                  # (save modified registers before control flow changes)
          sw    t1, -12(fp)             # spill T7 from t1 to (fp-12)
          call  _sub_3                  
          mv    t0, a0                  
                                  # þ
          mv    t1, t0                  
                                  # T15 <- LOAD_SYMBOL a
          la    t2, a                   
                                  # T16 <- LOAD T15, 0
          lw    s1, 0(t2)               
                                  # T17 <- (T16 + T9)
          add   s2, s1, t1              
                                  # T18 <- (T17 + T7)
          lw    s3, -12(fp)             # load T7 from (fp-12) into s3
          add   s4, s2, s3              
                                  # þ
          mv    s5, s4                  
          mv    a0, s5                  
          mv    sp, fp                  
          lw    ra, -4(fp)              
          lw    fp, -8(fp)              
          ret                           
