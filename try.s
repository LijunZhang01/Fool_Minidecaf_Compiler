          .text                         
          .data                         
          .global                       seed
seed:          .word                         19971231
          .word                         19981013
          .word                         1000000007
          .zero                         0
          .global                       a
a:          .word                         0
          .zero                         39996
          .global                       staticvalue
staticvalue:
          .word                         0
          .globl main                   
          .align 2                      

          .text                         
_set:                                   # function entry
          sw    ra, -4(sp)              
          sw    fp, -8(sp)              
          mv    fp, sp                  
          addi  sp, sp, -20             
__LL0:                                  
                                  # ÿ
          li    t0, 30                  
                                  # ÿ
                                  # T5 = ALLOC 124
          addi  sp, sp, -124            
          mv    t1, sp                  
                                  # ÿ
          li    t2, 0                   
                                  # STORE T6 -> T5, 0
          sw    t2, 0(t1)               
                                  # ÿ
          li    s1, 0                   
                                  # ÿ
          li    s2, 4                   
                                  # T9 <- (T7 * T8)
          mul   s3, s1, s2              
                                  # ÿ
          li    s4, 1                   
                                  # T11 <- (T5 + T9)
          add   s5, t1, s3              
                                  # STORE T10 -> T11, 0
          sw    s4, 0(s5)               
                                  # ÿ
          li    s6, 1                   
                                  # ÿ
          li    s7, 4                   
                                  # T14 <- (T12 * T13)
          mul   s8, s6, s7              
                                  # ÿ
          li    s9, 0                   
                                  # ÿ
          li    s10, 4                  
                                  # T17 <- (T15 * T16)
          mul   s11, s9, s10            
                                  # T18 <- (T5 + T17)
          add   t3, t1, s11             
                                  # T19 <- LOAD T18, 0
          lw    t4, 0(t3)               
                                  # ÿ
          li    t5, 2                   
                                  # T21 <- (T19 * T20)
          mul   t6, t4, t5              
                                  # T22 <- (T5 + T14)
          add   t0, t1, s8              
                                  # STORE T21 -> T22, 0
          sw    t6, 0(t0)               
                                  # ÿ
          li    t0, 2                   
                                  # ÿ
          li    t2, 4                   
                                  # T25 <- (T23 * T24)
          mul   s1, t0, t2              
                                  # ÿ
          li    t0, 1                   
                                  # ÿ
          li    t2, 4                   
                                  # T28 <- (T26 * T27)
          mul   s2, t0, t2              
                                  # T29 <- (T5 + T28)
          add   t0, t1, s2              
                                  # T30 <- LOAD T29, 0
          lw    t2, 0(t0)               
                                  # ÿ
          li    t0, 2                   
                                  # T32 <- (T30 * T31)
          mul   s2, t2, t0              
                                  # T33 <- (T5 + T25)
          add   t0, t1, s1              
                                  # STORE T32 -> T33, 0
          sw    s2, 0(t0)               
                                  # ÿ
          li    t0, 3                   
                                  # ÿ
          li    t2, 4                   
                                  # T36 <- (T34 * T35)
          mul   s1, t0, t2              
                                  # ÿ
          li    t0, 2                   
                                  # ÿ
          li    t2, 4                   
                                  # T39 <- (T37 * T38)
          mul   s2, t0, t2              
                                  # T40 <- (T5 + T39)
          add   t0, t1, s2              
                                  # T41 <- LOAD T40, 0
          lw    t2, 0(t0)               
                                  # ÿ
          li    t0, 2                   
                                  # T43 <- (T41 * T42)
          mul   s2, t2, t0              
                                  # T44 <- (T5 + T36)
          add   t0, t1, s1              
                                  # STORE T43 -> T44, 0
          sw    s2, 0(t0)               
                                  # ÿ
          li    t0, 4                   
                                  # ÿ
          li    t2, 4                   
                                  # T47 <- (T45 * T46)
          mul   s1, t0, t2              
                                  # ÿ
          li    t0, 3                   
                                  # ÿ
          li    t2, 4                   
                                  # T50 <- (T48 * T49)
          mul   s2, t0, t2              
                                  # T51 <- (T5 + T50)
          add   t0, t1, s2              
                                  # T52 <- LOAD T51, 0
          lw    t2, 0(t0)               
                                  # ÿ
          li    t0, 2                   
                                  # T54 <- (T52 * T53)
          mul   s2, t2, t0              
                                  # T55 <- (T5 + T47)
          add   t0, t1, s1              
                                  # STORE T54 -> T55, 0
          sw    s2, 0(t0)               
                                  # ÿ
          li    t0, 5                   
                                  # ÿ
          li    t2, 4                   
                                  # T58 <- (T56 * T57)
          mul   s1, t0, t2              
                                  # ÿ
          li    t0, 4                   
                                  # ÿ
          li    t2, 4                   
                                  # T61 <- (T59 * T60)
          mul   s2, t0, t2              
                                  # T62 <- (T5 + T61)
          add   t0, t1, s2              
                                  # T63 <- LOAD T62, 0
          lw    t2, 0(t0)               
                                  # ÿ
          li    t0, 2                   
                                  # T65 <- (T63 * T64)
          mul   s2, t2, t0              
                                  # T66 <- (T5 + T58)
          add   t0, t1, s1              
                                  # STORE T65 -> T66, 0
          sw    s2, 0(t0)               
                                  # ÿ
          li    t0, 6                   
                                  # ÿ
          li    t2, 4                   
                                  # T69 <- (T67 * T68)
          mul   s1, t0, t2              
                                  # ÿ
          li    t0, 5                   
                                  # ÿ
          li    t2, 4                   
                                  # T72 <- (T70 * T71)
          mul   s2, t0, t2              
                                  # T73 <- (T5 + T72)
          add   t0, t1, s2              
                                  # T74 <- LOAD T73, 0
          lw    t2, 0(t0)               
                                  # ÿ
          li    t0, 2                   
                                  # T76 <- (T74 * T75)
          mul   s2, t2, t0              
                                  # T77 <- (T5 + T69)
          add   t0, t1, s1              
                                  # STORE T76 -> T77, 0
          sw    s2, 0(t0)               
                                  # ÿ
          li    t0, 7                   
                                  # ÿ
          li    t2, 4                   
                                  # T80 <- (T78 * T79)
          mul   s1, t0, t2              
                                  # ÿ
          li    t0, 6                   
                                  # ÿ
          li    t2, 4                   
                                  # T83 <- (T81 * T82)
          mul   s2, t0, t2              
                                  # T84 <- (T5 + T83)
          add   t0, t1, s2              
                                  # T85 <- LOAD T84, 0
          lw    t2, 0(t0)               
                                  # ÿ
          li    t0, 2                   
                                  # T87 <- (T85 * T86)
          mul   s2, t2, t0              
                                  # T88 <- (T5 + T80)
          add   t0, t1, s1              
                                  # STORE T87 -> T88, 0
          sw    s2, 0(t0)               
                                  # ÿ
          li    t0, 8                   
                                  # ÿ
          li    t2, 4                   
                                  # T91 <- (T89 * T90)
          mul   s1, t0, t2              
                                  # ÿ
          li    t0, 7                   
                                  # ÿ
          li    t2, 4                   
                                  # T94 <- (T92 * T93)
          mul   s2, t0, t2              
                                  # T95 <- (T5 + T94)
          add   t0, t1, s2              
                                  # T96 <- LOAD T95, 0
          lw    t2, 0(t0)               
                                  # ÿ
          li    t0, 2                   
                                  # T98 <- (T96 * T97)
          mul   s2, t2, t0              
                                  # T99 <- (T5 + T91)
          add   t0, t1, s1              
                                  # STORE T98 -> T99, 0
          sw    s2, 0(t0)               
                                  # ÿ
          li    t0, 9                   
                                  # ÿ
          li    t2, 4                   
                                  # T102 <- (T100 * T101)
          mul   s1, t0, t2              
                                  # ÿ
          li    t0, 8                   
                                  # ÿ
          li    t2, 4                   
                                  # T105 <- (T103 * T104)
          mul   s2, t0, t2              
                                  # T106 <- (T5 + T105)
          add   t0, t1, s2              
                                  # T107 <- LOAD T106, 0
          lw    t2, 0(t0)               
                                  # ÿ
          li    t0, 2                   
                                  # T109 <- (T107 * T108)
          mul   s2, t2, t0              
                                  # T110 <- (T5 + T102)
          add   t0, t1, s1              
                                  # STORE T109 -> T110, 0
          sw    s2, 0(t0)               
                                  # ÿ
          li    t0, 10                  
                                  # ÿ
          li    t2, 4                   
                                  # T113 <- (T111 * T112)
          mul   s1, t0, t2              
                                  # ÿ
          li    t0, 9                   
                                  # ÿ
          li    t2, 4                   
                                  # T116 <- (T114 * T115)
          mul   s2, t0, t2              
                                  # T117 <- (T5 + T116)
          add   t0, t1, s2              
                                  # T118 <- LOAD T117, 0
          lw    t2, 0(t0)               
                                  # ÿ
          li    t0, 2                   
                                  # T120 <- (T118 * T119)
          mul   s2, t2, t0              
                                  # T121 <- (T5 + T113)
          add   t0, t1, s1              
                                  # STORE T120 -> T121, 0
          sw    s2, 0(t0)               
                                  # ÿ
          li    t0, 10                  
                                  # T122 <- T123
          mv    t2, t0                  
                                  # (save modified registers before control flow changes)
          sw    t1, -12(fp)             # spill T5 from t1 to (fp-12)
          sw    t2, -16(fp)             # spill T122 from t2 to (fp-16)
          j     __LL1                   
__LL1:                                  
                                  # 
          li    t0, 30                  
                                  # T125 <- (T122 < T124)
          lw    t1, -16(fp)             # load T122 from (fp-16) into t1
          slt   t2, t1, t0              
          beqz  t2, __LL3               
          j     __LL2                   
__LL2:                                  
                                  # 
          li    t0, 1                   
                                  # T127 <- (T122 + T126)
          lw    t1, -16(fp)             # load T122 from (fp-16) into t1
          add   t2, t1, t0              
                                  # T122 <- T127
          mv    t1, t2                  
                                  # 
          li    s1, 4                   
                                  # T129 <- (T122 * T128)
          mul   s2, t1, s1              
                                  # 
          li    s3, 1                   
                                  # T131 <- (T122 - T130)
          sub   s4, t1, s3              
                                  # 
          li    s5, 4                   
                                  # T133 <- (T131 * T132)
          mul   s6, s4, s5              
                                  # T134 <- (T5 + T133)
          lw    s7, -12(fp)             # load T5 from (fp-12) into s7
          add   s8, s7, s6              
                                  # T135 <- LOAD T134, 0
          lw    s9, 0(s8)               
                                  # 
          li    s10, 2                  
                                  # T137 <- (T135 * T136)
          mul   s11, s9, s10            
                                  # T138 <- (T5 + T129)
          add   t3, s7, s2              
                                  # STORE T137 -> T138, 0
          sw    s11, 0(t3)              
                                  # (save modified registers before control flow changes)
          sw    t1, -16(fp)             # spill T122 from t1 to (fp-16)
          j     __LL1                   
__LL3:                                  
                                  # ÿ
          li    t0, 0                   
                                  # T139 <- T140
          mv    t1, t0                  
                                  # ÿ
          li    t2, 30                  
                                  # T142 <- (T1 / T141)
          lw    s1, 4(fp)               # load T1 from (fp+4) into s1
          div   s2, s1, t2              
                                  # T143 <- 10000
          li    s3, 10000               
                                  # T144 <- (T142 <= T143)
          slt   s4, s2, s3              
          xori  s4, s4, 1               
                                  # (save modified registers before control flow changes)
          sw    t1, -20(fp)             # spill T139 from t1 to (fp-20)
          beqz  s4, __LL5               
          j     __LL4                   
__LL4:                                  
                                  # 
          li    t0, 0                   
          mv    a0, t0                  
          mv    sp, fp                  
          lw    ra, -4(fp)              
          lw    fp, -8(fp)              
          ret                           
__LL5:                                  
                                  # ÿ
          li    t0, 30                  
                                  # T147 <- (T1 / T146)
          lw    t1, 4(fp)               # load T1 from (fp+4) into t1
          div   t2, t1, t0              
                                  # ÿ
          li    s1, 4                   
                                  # T149 <- (T147 * T148)
          mul   s2, t2, s1              
                                  # T150 <- (T2 + T149)
          lw    s3, 8(fp)               # load T2 from (fp+8) into s3
          add   s4, s3, s2              
                                  # T151 <- LOAD T150, 0
          lw    s5, 0(s4)               
                                  # ÿ
          li    s6, 30                  
                                  # T153 <- (T1 % T152)
          rem   s7, t1, s6              
                                  # ÿ
          li    s8, 4                   
                                  # T155 <- (T153 * T154)
          mul   s9, s7, s8              
                                  # T156 <- (T5 + T155)
          lw    s10, -12(fp)            # load T5 from (fp-12) into s10
          add   s11, s10, s9            
                                  # T157 <- LOAD T156, 0
          lw    t3, 0(s11)              
                                  # T158 <- (T151 / T157)
          div   t4, s5, t3              
                                  # ÿ
          li    t5, 2                   
                                  # T160 <- (T158 % T159)
          rem   t6, t4, t5              
                                  # T161 <- (T160 != T0)
          lw    t0, 0(fp)               # load T0 from (fp+0) into t0
          sub   t2, t6, t0              
          snez  t2, t2                  
          beqz  t2, __LL12              
          j     __LL6                   
__LL6:                                  
                                  # 
          li    t0, 30                  
                                  # T163 <- (T1 / T162)
          lw    t1, 4(fp)               # load T1 from (fp+4) into t1
          div   t2, t1, t0              
                                  # 
          li    s1, 4                   
                                  # T165 <- (T163 * T164)
          mul   s2, t2, s1              
                                  # T166 <- (T2 + T165)
          lw    s3, 8(fp)               # load T2 from (fp+8) into s3
          add   s4, s3, s2              
                                  # T167 <- LOAD T166, 0
          lw    s5, 0(s4)               
                                  # 
          li    s6, 30                  
                                  # T169 <- (T1 % T168)
          rem   s7, t1, s6              
                                  # 
          li    s8, 4                   
                                  # T171 <- (T169 * T170)
          mul   s9, s7, s8              
                                  # T172 <- (T5 + T171)
          lw    s10, -12(fp)            # load T5 from (fp-12) into s10
          add   s11, s10, s9            
                                  # T173 <- LOAD T172, 0
          lw    t3, 0(s11)              
                                  # T174 <- (T167 / T173)
          div   t4, s5, t3              
                                  # 
          li    t5, 2                   
                                  # T176 <- (T174 % T175)
          rem   t6, t4, t5              
                                  # 
          li    t0, 0                   
                                  # T178 <- (T176 == T177)
          xor   t2, t6, t0              
          seqz  t2, t2                  
          beqz  t2, __LL9               
          j     __LL7                   
__LL7:                                  
                                  # 
          li    t0, 1                   
                                  # T180 <- (T0 == T179)
          lw    t1, 0(fp)               # load T0 from (fp+0) into t1
          xor   t2, t1, t0              
          seqz  t2, t2                  
          beqz  t2, __LL9               
          j     __LL8                   
__LL8:                                  
                                  # 
          li    t0, 30                  
                                  # T182 <- (T1 % T181)
          lw    t1, 4(fp)               # load T1 from (fp+4) into t1
          rem   t2, t1, t0              
                                  # 
          li    s1, 4                   
                                  # T184 <- (T182 * T183)
          mul   s2, t2, s1              
                                  # T185 <- (T5 + T184)
          lw    s3, -12(fp)             # load T5 from (fp-12) into s3
          add   s4, s3, s2              
                                  # T186 <- LOAD T185, 0
          lw    s5, 0(s4)               
                                  # T139 <- T186
          mv    s6, s5                  
                                  # (save modified registers before control flow changes)
          sw    s6, -20(fp)             # spill T139 from s6 to (fp-20)
          j     __LL9                   
__LL9:                                  
                                  # 
          li    t0, 30                  
                                  # T188 <- (T1 / T187)
          lw    t1, 4(fp)               # load T1 from (fp+4) into t1
          div   t2, t1, t0              
                                  # 
          li    s1, 4                   
                                  # T190 <- (T188 * T189)
          mul   s2, t2, s1              
                                  # T191 <- (T2 + T190)
          lw    s3, 8(fp)               # load T2 from (fp+8) into s3
          add   s4, s3, s2              
                                  # T192 <- LOAD T191, 0
          lw    s5, 0(s4)               
                                  # 
          li    s6, 30                  
                                  # T194 <- (T1 % T193)
          rem   s7, t1, s6              
                                  # 
          li    s8, 4                   
                                  # T196 <- (T194 * T195)
          mul   s9, s7, s8              
                                  # T197 <- (T5 + T196)
          lw    s10, -12(fp)            # load T5 from (fp-12) into s10
          add   s11, s10, s9            
                                  # T198 <- LOAD T197, 0
          lw    t3, 0(s11)              
                                  # T199 <- (T192 / T198)
          div   t4, s5, t3              
                                  # 
          li    t5, 2                   
                                  # T201 <- (T199 % T200)
          rem   t6, t4, t5              
                                  # 
          li    t0, 1                   
                                  # T203 <- (T201 == T202)
          xor   t2, t6, t0              
          seqz  t2, t2                  
          beqz  t2, __LL12              
          j     __LL10                  
__LL10:                                 
                                  # 
          li    t0, 0                   
                                  # T205 <- (T0 == T204)
          lw    t1, 0(fp)               # load T0 from (fp+0) into t1
          xor   t2, t1, t0              
          seqz  t2, t2                  
          beqz  t2, __LL12              
          j     __LL11                  
__LL11:                                 
                                  # 
          li    t0, 30                  
                                  # T207 <- (T1 % T206)
          lw    t1, 4(fp)               # load T1 from (fp+4) into t1
          rem   t2, t1, t0              
                                  # 
          li    s1, 4                   
                                  # T209 <- (T207 * T208)
          mul   s2, t2, s1              
                                  # T210 <- (T5 + T209)
          lw    s3, -12(fp)             # load T5 from (fp-12) into s3
          add   s4, s3, s2              
                                  # T211 <- LOAD T210, 0
          lw    s5, 0(s4)               
                                  # T212 <- (T139 - T211)
          lw    s6, -20(fp)             # load T139 from (fp-20) into s6
          sub   s7, s6, s5              
                                  # T139 <- T212
          mv    s6, s7                  
                                  # (save modified registers before control flow changes)
          sw    s6, -20(fp)             # spill T139 from s6 to (fp-20)
          j     __LL12                  
__LL12:                                 
                                  # 
          li    t0, 30                  
                                  # T214 <- (T1 / T213)
          lw    t1, 4(fp)               # load T1 from (fp+4) into t1
          div   t2, t1, t0              
                                  # 
          li    s1, 4                   
                                  # T216 <- (T214 * T215)
          mul   s2, t2, s1              
                                  # 
          li    s3, 30                  
                                  # T218 <- (T1 / T217)
          div   s4, t1, s3              
                                  # 
          li    s5, 4                   
                                  # T220 <- (T218 * T219)
          mul   s6, s4, s5              
                                  # T221 <- (T2 + T220)
          lw    s7, 8(fp)               # load T2 from (fp+8) into s7
          add   s8, s7, s6              
                                  # T222 <- LOAD T221, 0
          lw    s9, 0(s8)               
                                  # T223 <- (T222 + T139)
          lw    s10, -20(fp)            # load T139 from (fp-20) into s10
          add   s11, s9, s10            
                                  # T224 <- (T2 + T216)
          add   t3, s7, s2              
                                  # STORE T223 -> T224, 0
          sw    s11, 0(t3)              
                                  # 
          li    t4, 0                   
          mv    a0, t4                  
          mv    sp, fp                  
          lw    ra, -4(fp)              
          lw    fp, -8(fp)              
          ret                           

          .text                         
_rand:                                  # function entry
          sw    ra, -4(sp)              
          sw    fp, -8(sp)              
          mv    fp, sp                  
          addi  sp, sp, -8              
__LL13:                                 
                                  # T228 <- LOAD_SYMBOL staticvalue
          la    t0, staticvalue         
                                  # T229 <- LOAD T228, 0
          lw    t1, 0(t0)               
                                  # ÿ
          li    t2, 0                   
                                  # ÿ
          li    s1, 4                   
                                  # T232 <- (T230 * T231)
          mul   s2, t2, s1              
                                  # T233 <- LOAD_SYMBOL seed
          la    s3, seed                
                                  # T234 <- (T233 + T232)
          add   s4, s3, s2              
                                  # T235 <- LOAD T234, 0
          lw    s5, 0(s4)               
                                  # T236 <- (T229 * T235)
          mul   s6, t1, s5              
                                  # ÿ
          li    s7, 1                   
                                  # ÿ
          li    s8, 4                   
                                  # T239 <- (T237 * T238)
          mul   s9, s7, s8              
                                  # T240 <- LOAD_SYMBOL seed
          la    s10, seed               
                                  # T241 <- (T240 + T239)
          add   s11, s10, s9            
                                  # T242 <- LOAD T241, 0
          lw    t3, 0(s11)              
                                  # T243 <- (T236 + T242)
          add   t4, s6, t3              
                                  # T244 <- LOAD_SYMBOL staticvalue
          la    t5, staticvalue         
                                  # STORE T243 -> T244, 0
          sw    t4, 0(t5)               
                                  # T245 <- LOAD_SYMBOL staticvalue
          la    t6, staticvalue         
                                  # T246 <- LOAD T245, 0
          lw    t0, 0(t6)               
                                  # ÿ
          li    t1, 2                   
                                  # ÿ
          li    t2, 4                   
                                  # T249 <- (T247 * T248)
          mul   s1, t1, t2              
                                  # T250 <- LOAD_SYMBOL seed
          la    t1, seed                
                                  # T251 <- (T250 + T249)
          add   t2, t1, s1              
                                  # T252 <- LOAD T251, 0
          lw    t1, 0(t2)               
                                  # T253 <- (T246 % T252)
          rem   t2, t0, t1              
                                  # T254 <- LOAD_SYMBOL staticvalue
          la    t0, staticvalue         
                                  # STORE T253 -> T254, 0
          sw    t2, 0(t0)               
                                  # T255 <- LOAD_SYMBOL staticvalue
          la    t0, staticvalue         
                                  # T256 <- LOAD T255, 0
          lw    t1, 0(t0)               
                                  # ÿ
          li    t0, 0                   
                                  # T258 <- (T256 < T257)
          slt   t2, t1, t0              
          beqz  t2, __LL15              
          j     __LL14                  
__LL14:                                 
                                  # 
          li    t0, 2                   
                                  # 
          li    t1, 4                   
                                  # T261 <- (T259 * T260)
          mul   t2, t0, t1              
                                  # T262 <- LOAD_SYMBOL seed
          la    s1, seed                
                                  # T263 <- (T262 + T261)
          add   s2, s1, t2              
                                  # T264 <- LOAD T263, 0
          lw    s3, 0(s2)               
                                  # T265 <- LOAD_SYMBOL staticvalue
          la    s4, staticvalue         
                                  # T266 <- LOAD T265, 0
          lw    s5, 0(s4)               
                                  # T267 <- (T264 + T266)
          add   s6, s3, s5              
                                  # T268 <- LOAD_SYMBOL staticvalue
          la    s7, staticvalue         
                                  # STORE T267 -> T268, 0
          sw    s6, 0(s7)               
          j     __LL15                  
__LL15:                                 
                                  # T269 <- LOAD_SYMBOL staticvalue
          la    t0, staticvalue         
                                  # T270 <- LOAD T269, 0
          lw    t1, 0(t0)               
          mv    a0, t1                  
          mv    sp, fp                  
          lw    ra, -4(fp)              
          lw    fp, -8(fp)              
          ret                           

          .text                         
main:                                   # function entry
          sw    ra, -4(sp)              
          sw    fp, -8(sp)              
          mv    fp, sp                  
          addi  sp, sp, -36             
__LL16:                                 
                                  # T273 <- 10000000
          li    t0, 10000000            
                                  # T272 <- T273
          mv    t1, t0                  
                                  # T274 <- 197123
          li    t2, 197123              
                                  # T275 <- LOAD_SYMBOL staticvalue
          la    s1, staticvalue         
                                  # STORE T274 -> T275, 0
          sw    t2, 0(s1)               
                                  # T276 <- call _starttime'
                                  # (save modified registers before control flow changes)
          sw    t1, -32(fp)             # spill T272 from t1 to (fp-32)
          call  _sysy_starttime         
          mv    zero, a0                
          j     __LL17                  
__LL17:                                 
                                  # 
          li    t0, 0                   
                                  # T280 <- (T272 > T279)
          lw    t1, -32(fp)             # load T272 from (fp-32) into t1
          sgt   t2, t1, t0              
          beqz  t2, __LL19              
          j     __LL18                  
__LL18:                                 
                                  # 
          li    t0, 1                   
                                  # T282 <- (T272 - T281)
          lw    t1, -32(fp)             # load T272 from (fp-32) into t1
          sub   t2, t1, t0              
                                  # T272 <- T282
          mv    t1, t2                  
                                  # T283 <- call _rand'
                                  # (save modified registers before control flow changes)
          sw    t1, -32(fp)             # spill T272 from t1 to (fp-32)
          call  _rand                   
          mv    t0, a0                  
                                  # T284 <- 300000
          li    t1, 300000              
                                  # T285 <- (T283 % T284)
          rem   t2, t0, t1              
                                  # T277 <- T285
          mv    s1, t2                  
                                  # T286 <- call _rand'
                                  # (save modified registers before control flow changes)
          sw    t0, -28(fp)             # spill T283 from t0 to (fp-28)
          sw    s1, -36(fp)             # spill T277 from s1 to (fp-36)
          call  _rand                   
          mv    t0, a0                  
                                  # 
          li    t1, 2                   
                                  # T288 <- (T286 % T287)
          rem   t2, t0, t1              
                                  # T278 <- T288
          mv    s1, t2                  
                                  # T289 <- LOAD_SYMBOL a
          la    s2, a                   
                                  # T290 <- LOAD T289, 0
          lw    s3, 0(s2)               
                                  # 
          addi  sp, sp, -4              
          sw    s3, 0(sp)               
                                  # 
          lw    s4, -36(fp)             # load T277 from (fp-36) into s4
          addi  sp, sp, -4              
          sw    s4, 0(sp)               
                                  # 
          addi  sp, sp, -4              
          sw    s1, 0(sp)               
                                  # T291 <- call _set'
                                  # (save modified registers before control flow changes)
          sw    t0, -24(fp)             # spill T286 from t0 to (fp-24)
          call  _set                    
          mv    t0, a0                  
                                  # (save modified registers before control flow changes)
          sw    t0, -20(fp)             # spill T291 from t0 to (fp-20)
          j     __LL17                  
__LL19:                                 
                                  # T292 <- call _stoptime'
          call  _sysy_stoptime          
          mv    zero, a0                
                                  # T293 <- 10000
          li    t0, 10000               
                                  # push1   T293
          mv    a0, t0                  
                                  # T294 <- LOAD_SYMBOL a
          la    t1, a                   
                                  # T295 <- LOAD T294, 0
          lw    t2, 0(t1)               
                                  # push1   T295
          mv    a0, t2                  
                                  # T296 <- call _putarray'
          call  putarray                
          mv    zero, a0                
                                  # ÿ
          li    t0, 0                   
          mv    a0, t0                  
          mv    sp, fp                  
          lw    ra, -4(fp)              
          lw    fp, -8(fp)              
          ret                           
