      SUBROUTINE GWS3(IYD,SEC,ALT,GLAT,GLONG,STL,F107A,F107,AP,W)
C      Horizontal wind model HWM87
C      A. E. HEDIN  (11/3/87)
C      10/8/88 CHANGE HANDLING OF SW(9)
C      Currently intended only for winds above about 220 km.
C      Calling argument list made similar to GTS5 subroutine for
C       MSIS-86 density model.  Some arguments not currently used but
C       left in for compatability and expected use in later versions.
C     INPUT:
C        IYD - YEAR AND DAY AS YYDDD
C        SEC - UT(SEC)
C        ALT - ALTITUDE(KM) $not used$
C        GLAT - GEODETIC LATITUDE(DEG)
C        GLONG - GEODETIC LONGITUDE(DEG)
C        STL - LOCAL APPARENT SOLAR TIME(HRS)
C        F107A - 3 MONTH AVERAGE OF F10.7 FLUX $not used$
C        F107 - DAILY F10.7 FLUX FOR PREVIOUS DAY $not used$
C        AP - MAGNETIC INDEX(DAILY) OR WHEN SW(9)=-1. :
C             AP(2)=CURRENT 3HR AP INDEX
C      OUTPUT
C        W(1) = MERIDIANAL (m/sec + North)
C        W(2) = ZONAL (m/sec + East)
C          ADDITIONAL COMMENTS
C               TO TURN ON AND OFF PARTICULAR VARIATIONS CALL TSELEC(SW)
C               WHERE SW IS A 25 ELEMENT ARRAY CONTAINING 0. FOR OFF, 1. 
C               FOR ON, OR 2. FOR MAIN EFFECTS OFF BUT CROSS TERMS ON
C               FOR THE FOLLOWING VARIATIONS
C               1 - F10.7 EFFECT ON MEAN  2 - TIME INDEPENDENT
C               3 - SYMMETRICAL ANNUAL    4 - SYMMETRICAL SEMIANNUAL
C               5 - ASYMMETRICAL ANNUAL   6 - ASYMMETRICAL SEMIANNUAL
C               7 - DIURNAL               8 - SEMIDIURNAL
C               9 - DAILY AP             10 - ALL UT/LONG EFFECTS
C              11 - LONGITUDINAL         12 - UT AND MIXED UT/LONG
C              13 - MIXED AP/UT/LONG     14 - TERDIURNAL
C              24 - ALL B FIELDS (DIV)   25 - ALL C FIELDS (CURL)
C
C              To get current values of SW: CALL TRETRV(SW)
C
      DIMENSION AP(1),W(1),WINDF(2)
      COMMON/PARMW3/PWB(150),PWC(200)
      COMMON/CSW/SW(25),ISW,SWC(25)
      COMMON/DATIME/ISDATE(3),ISTIME(2),NAME(2)
      DATA IFL/0/
      IF(IFL.EQ.1.AND.NAME(1).EQ.'HWM8'.AND.NAME(2).EQ.'7   ') GO TO 5
      CALL PRMW3
      IFL=1
    5 CONTINUE
      YRD=IYD
      CALL GLOBW3(YRD,SEC,GLAT,GLONG,STL,F107A,F107,AP,PWB,PWC,WINDF)
      W(1)= WINDF(1)
      W(2)= WINDF(2)
      RETURN
      END
      SUBROUTINE GLOBW3(YRD,SEC,LAT,LONG,STL,F107A,F107,AP,PB,PC,WW)
      REAL LAT,LONG
      DIMENSION WB(2,15),WC(2,15),PB(1),PC(1),WW(2),BT(20,20),BP(20,20)
      DIMENSION AP(1),SV(25)
      COMMON/CSW/SW(25),ISW,SWC(25)
      COMMON/HWMC/WBT(2),WCT(2)
      COMMON/WTEST/WB,WC
      DATA DGTR/.017453/,SR/7.2722E-5/,HR/.2618/,DR/1.72142E-2/
      DATA NSW/14/,WB/30*0/,WC/30*0/,TLL/-99./,XL/-999./,XLONG/-999./
      DATA SV/25*1./,SW9/1./
      G0(A)=(A-4.+(PB(26)-1.)*(A-4.+(EXP(-ABS(PB(25))*(A-4.))-1.)/
     * ABS(PB(25))))
      IF(ISW.NE.64999) CALL TSELEC(SV)
      WB(1,11)=0
      WB(2,11)=0
      WB(1,12)=0
      WB(2,12)=0
      WB(1,13)=0
      WB(2,13)=0
      WC(1,11)=0
      WC(2,11)=0
      WC(1,12)=0
      WC(2,12)=0
      WC(1,13)=0
      WC(2,13)=0
      IF(SW(9).GT.0) SW9=1.
      IF(SW(9).LT.0) SW9=-1.
      IYR = YRD/1000.
      DAY = YRD - IYR*1000.
      IF(XL.NE.LAT) THEN
        CALL VSPHER(LAT,12,3,BT,BP,20)
        XL=LAT
      ENDIF
      IF(TLL.NE.STL)  THEN
        SSTL = SIN(HR*STL)
        CSTL = COS(HR*STL)
        S2STL = SIN(2.*HR*STL)
        C2STL = COS(2.*HR*STL)
        S3STL = SIN(3.*HR*STL)
        C3STL = COS(3.*HR*STL)
        TLL = STL
      ENDIF
      IF(DAY.NE.DAYL.OR.PB(14).NE.PB14) CD14=COS(DR*(DAY-PB(14)))
      DAYL=DAY
      PB14=PB(14)
      IF(XLONG.NE.LONG) THEN
        SLONG=SIN(DGTR*LONG)
        CLONG=COS(DGTR*LONG)
        XLONG=LONG
      ENDIF
C          F10.7 EFFECT
       F1=1.
       F2=1.
C       TIME INDEPENDENT
      WB(1,2)=PB(2)*BT(3,1)+PB(3)*BT(5,1)+PB(23)*BT(7,1)
      WC(2,2)=-(PC(2)*BT(2,1)+PC(3)*BT(4,1)+PC(23)*BT(6,1))
     $ -(PC(27)*BT(3,1)+PC(15)*BT(5,1)+PC(60)*BT(7,1)
     $ +PC(161)*BT(9,1)+PC(162)*BT(11,1)+PC(163)*BT(13,1))
C       ANNUAL
      WB(1,5)=(PB(10)*BT(2,1)+PB(11)*BT(4,1))*CD14*F1
      WC(2,5)=-(PC(10)*BT(3,1)+PC(11)*BT(5,1))*CD14*F1
C       DIURNAL      
      WB(1,7)=(PB(7)*BT(2,2)+PB(8)*BT(4,2)+PB(29)*BT(6,2)
     $  )*SSTL*F2
     $ + (PB(4)*BT(2,2)+PB(5)*BT(4,2)+PB(28)*BT(6,2)
     $  )*CSTL*F2
      WB(2,7)=-(PB(4)*BP(2,2)+PB(5)*BP(4,2)+PB(28)*BP(6,2)
     $  )*SSTL*F2
     $ + (PB(7)*BP(2,2)+PB(8)*BP(4,2)+PB(29)*BP(6,2)
     $  )*CSTL*F2
      DCASY=1.
      WC(1,7)=-(PC(4)*BP(3,2)+PC(5)*BP(5,2)+PC(28)*BP(7,2)
     $ +PC(141)*BP(9,2)+PC(143)*BP(11,2)
     $ +PC(181)*BP(2,2)+PC(183)*BP(4,2)+PC(185)*BP(6,2)
     $ +PC(187)*BP(8,2)+PC(189)*BP(10,2)
     $ +PC(12)*BP(2,2)*CD14*SWC(5)
     $  )*SSTL*DCASY
     $ +(PC(7)*BP(3,2)+PC(8)*BP(5,2)+PC(29)*BP(7,2)
     $ +PC(142)*BP(9,2)+PC(144)*BP(11,2)
     $ +PC(182)*BP(2,2)+PC(184)*BP(4,2)+PC(186)*BP(6,2)
     $ +PC(188)*BP(8,2)+PC(190)*BP(10,2)
     $ +PC(13)*BP(2,2)*CD14*SWC(5)
     $  )*CSTL*DCASY
      WC(2,7)=-(PC(7)*BT(3,2)+PC(8)*BT(5,2)+PC(29)*BT(7,2)
     $ +PC(142)*BT(9,2)+PC(144)*BT(11,2)
     $ +PC(182)*BT(2,2)+PC(184)*BT(4,2)+PC(186)*BT(6,2)
     $ +PC(188)*BT(8,2)+PC(190)*BT(10,2)
     $ +PC(13)*BT(2,2)*CD14*SWC(5)
     $  )*SSTL*DCASY
     $ -(PC(4)*BT(3,2)+PC(5)*BT(5,2)+PC(28)*BT(7,2)
     $ +PC(141)*BT(9,2)+PC(143)*BT(11,2)
     $ +PC(181)*BT(2,2)+PC(183)*BT(4,2)+PC(185)*BT(6,2)
     $ +PC(187)*BT(8,2)+PC(189)*BT(10,2)
     $ +PC(12)*BT(2,2)*CD14*SWC(5)
     $  )*CSTL*DCASY
C       SEMIDIURNAL
      WB(1,8)=(PB(9)*BT(3,3)+PB(43)*BT(5,3)
     $  )*S2STL*F2
     $ + (PB(6)*BT(3,3)+PB(42)*BT(5,3)
     $  )*C2STL*F2
      WB(2,8)=-(PB(6)*BP(3,3)+PB(42)*BP(5,3)
     $  )*S2STL*F2
     $ + (PB(9)*BP(3,3)+PB(43)*BP(5,3)
     $  )*C2STL*F2
      WC(1,8)=-(PC(6)*BP(4,3)+PC(42)*BP(6,3)
     $ +PC(24)*BP(3,3)*CD14*SWC(5)
     $  )*S2STL*DCASY
     $ + (PC(9)*BP(4,3)+PC(43)*BP(6,3)
     $ +PC(34)*BP(3,3)*CD14*SWC(5)
     $  )*C2STL*DCASY
     $ -(PC(151)*BP(3,3)+PC(153)*BP(5,3)+PC(155)*BP(7,3)
     $ +PC(157)*BP(9,3)
     $ )*S2STL
     $ + (PC(152)*BP(3,3)+PC(154)*BP(5,3)+PC(156)*BP(7,3)
     $ +PC(158)*BP(9,3)
     $ )*C2STL
      WC(2,8)=-(PC(9)*BT(4,3)+PC(43)*BT(6,3)
     $ +PC(34)*BT(3,3)*CD14*SWC(5)
     $  )*S2STL*DCASY
     $ - (PC(6)*BT(4,3)+PC(42)*BT(6,3)
     $ +PC(24)*BT(3,3)*CD14*SWC(5)
     $  )*C2STL*DCASY
     $ -(PC(151)*BT(3,3)+PC(153)*BT(5,3)+PC(155)*BT(7,3)
     $ +PC(157)*BT(9,3)
     $ )*C2STL
     $ -(PC(152)*BT(3,3)+PC(154)*BT(5,3)+PC(156)*BT(7,3)
     $ +PC(158)*BT(9,3)
     $ )*S2STL
C        TERDIURNAL
      WB(1,14)=(PB(40)*BT(4,4)
     $  )*S3STL*F2
     $ + (PB(41)*BT(4,4)
     $  )*C3STL*F2
      WB(2,14)=-(PB(41)*BP(4,4)
     $  )*S3STL*F2
     $ + (PB(40)*BP(4,4)
     $  )*C3STL*F2
C        MAGNETIC ACTIVITY
      IF(SW9.EQ.-1.) GOTO 30
C           daily AP
      APD=AP(1)-4.
      APDF=(APD+(PB(45)-1.)*(APD+(EXP(-PB(44)*APD)-1.)/PB(44)))
      APDFC=APDF
      WB(1,9)=(PB(46)*BT(3,1)+PB(35)*BT(5,1))*APDF
      WC(1,9)=SWC(7)*WC(1,7)*PC(122)*APDFC
      WC(2,9)=-(PC(46)*BT(2,1)+PC(35)*BT(4,1))*APDFC
     $ +SWC(7)*WC(2,7)*PC(122)*APDFC
      GO TO 40
   30 CONTINUE
      IF(PB(25).LT.1.E-4) PB(25)=1.E-4
      APT=G0(AP(2))
      WB(1,9)=(PB(97)*BT(3,1)+PB(55)*BT(5,1))*APT
      WC(1,9)=SWC(7)*WC(1,7)*PC(129)*APT
      WC(2,9)=-(PC(97)*BT(2,1)+PC(55)*BT(4,1))*APT
     $ +SWC(7)*WC(2,7)*PC(129)*APT
  40  CONTINUE
      IF(SW(10).EQ.0) GOTO 49
C        LONGITUDINAL
      DLBASY=1.
      WB(1,11)=(PB(91)*BT(3,2)+PB(92)*BT(5,2)+PB(93)*BT(7,2))
     $  *SLONG*DLBASY
     $ + (PB(65)*BT(3,2)+PB(66)*BT(5,2)+PB(67)*BT(7,2))
     $  *CLONG*DLBASY
      WB(2,11)=-(PB(65)*BP(3,2)+PB(66)*BP(5,2)+PB(67)*BP(7,2))
     $  *SLONG*DLBASY
     $ + (PB(91)*BP(3,2)+PB(92)*BP(5,2)+PB(93)*BP(7,2))
     $  *CLONG*DLBASY
      DLCASY=1.
      WC(1,11)=-(PC(65)*BP(2,2)+PC(66)*BP(4,2)+PC(67)*BP(6,2)
     $ +PC(73)*BP(8,2)+PC(74)*BP(10,2)
     $  )*SLONG*DLCASY
     $ + (PC(91)*BP(2,2)+PC(92)*BP(4,2)+PC(93)*BP(6,2)
     $ +PC(87)*BP(8,2)+PC(88)*BP(10,2)
     $  )*CLONG*DLCASY
      WC(2,11)=-(PC(91)*BT(2,2)+PC(92)*BT(4,2)+PC(93)*BT(6,2)
     $ +PC(87)*BT(8,2)+PC(88)*BT(10,2)
     $  )*SLONG*DLCASY
     $ - (PC(65)*BT(2,2)+PC(66)*BT(4,2)+PC(67)*BT(6,2)
     $ +PC(73)*BT(8,2)+PC(74)*BT(10,2)
     $  )*CLONG*DLCASY
C       UT & MIXED UT/LONG
      UTCASY=1.
      WC(1,12)=(PC(77)*BP(3,3)+PC(78)*BP(5,3)+PC(79)*BP(7,3)
     $ +PC(165)*BP(9,3)+PC(166)*BP(11,3)+PC(167)*BP(13,3)
     $  )*COS(SR*(SEC-PC(80))+2.*DGTR*LONG)
      WC(2,12)=-(PC(69)*BT(3,1)+PC(70)*BT(5,1)+PC(71)*BT(7,1)
     $ +PC(116)*BT(9,1)+PC(117)*BT(11,1)+PC(118)*BT(13,1)
     $  )*COS(SR*(SEC-PC(72)))*UTCASY
     $ + (PC(77)*BT(3,3)+PC(78)*BT(5,3)+PC(79)*BT(7,3)
     $ +PC(165)*BT(9,3)+PC(166)*BT(11,3)+PC(167)*BT(13,3)
     $  )*COS(SR*(SEC-PC(80)+21600.)+2.*DGTR*LONG)
C       MIXED LONG,UT,AP
      IF(SW9.EQ.-1.) GO TO 45
      WB(1,13)=
     $  (PB(84)*BT(2,1)+PB(85)*BT(4,1)+PB(86)*BT(6,1))
     $  *COS(SR*(SEC-PB(76)))*APDF*SWC(12)
      WC(1,13)=SWC(11)*WC(1,11)*PC(61)*APDFC
     $ +SWC(12)*WC(1,12)*PC(84)*APDFC
      WC(2,13)=SWC(11)*WC(2,11)*PC(61)*APDFC
     $ +SWC(12)*WC(2,12)*PC(84)*APDFC
      GOTO 48
   45 CONTINUE
      WB(1,13)=
     $  (PB(56)*BT(2,1)+PB(57)*BT(4,1)+PB(58)*BT(6,1))
     $  *COS(SR*(SEC-PB(59)))*APT*SWC(12)
      WC(1,13)=SWC(11)*WC(1,11)*PC(53)*APT
     $ +SWC(12)*WC(1,12)*PC(56)*APT
      WC(2,13)=SWC(11)*WC(2,11)*PC(53)*APT
     $ +SWC(12)*WC(2,12)*PC(56)*APT
   48 CONTINUE
   49 CONTINUE
      WBT(1)=0
      WBT(2)=0
      WCT(1)=0
      WCT(2)=0
C       SUM WINDS AND CHANGE MERIDIONAL SIGN TO + NORTH
      DO 50 I=1,NSW
        WBT(1)=WBT(1)-ABS(SW(I))*WB(1,I)
        WCT(1)=WCT(1)-ABS(SW(I))*WC(1,I)
        WBT(2)=WBT(2)+ABS(SW(I))*WB(2,I)
        WCT(2)=WCT(2)+ABS(SW(I))*WC(2,I)
   50 CONTINUE
      WW(1)=WBT(1)*SW(24)+WCT(1)*SW(25)
      WW(2)=WBT(2)*SW(24)+WCT(2)*SW(25)
      RETURN
      END
      SUBROUTINE TSELEC(SV)
C        SET SWITCHES
      DIMENSION SV(1),SAV(25),SVV(1)
      COMMON/CSW/SW(25),ISW,SWC(25)
      DO 100 I = 1,25
        SAV(I)=SV(I)
        SW(I)=AMOD(SV(I),2.)
        IF(ABS(SV(I)).GT.0.) THEN
          SWC(I)=1.
        ELSE
          SWC(I)=0.
        ENDIF
  100 CONTINUE
      ISW=64999
      RETURN
      ENTRY TRETRV(SVV)
      DO 200 I=1,25
        SVV(I)=SAV(I)
  200 CONTINUE
      END
      SUBROUTINE VSPHER(THETA,L,M,BT,BP,LMAX)
C      CALCULATE VECTOR SPHERICAL HARMONIC B FIELD THETA AND PHI
C      FUNCTIONS BT,BP FOR GEOGRAPHICAL LATITUDE THETA THROUGH ORDER L,M
C      BT(L+1,M+1)= [(L-M+1) P(L+1,M) - (L+1) P(L,M) SIN(THETA)] /
C                [SQRT(L(L+1)) COS(THETA)]
C      BP(L+1,M+1)= M P(L,M) /[SQRT(L(L+1)) COS(THETA)]
C       RESULT FOR GIVEN L,M SAVED IN BT AND BP AT ONE HIGHER INDEX NUM
      DIMENSION BT(LMAX,1),BP(LMAX,1),PLG(20,20)
      DATA DGTR/1.74533E-2/
      IF(M.GT.L.OR.L.GT.LMAX-1) THEN
        WRITE(6,99) L,M,LMAX
   99 FORMAT(1X,'ILLEGAL INDICIES TO VSPHER',3I5)
        RETURN
      ENDIF
      BT(1,1)=0
      BP(1,1)=0
      IF(L.EQ.0.AND.M.EQ.0) RETURN
      CALL LEGPOL(THETA,L+1,M,PLG,20)
      C=SIN(THETA*DGTR)
      S=COS(THETA*DGTR)     
      IF(ABS(S).LT.1.E-5) THEN
        S=0
        IC=SIGN(1,IFIX(THETA))
      ENDIF
      DO 20 LL=1,L
        SQT=SQRT(FLOAT(LL)*(FLOAT(LL)+1))
        LMX=MIN(LL,M)
        DO 10 MM=0,LMX
          IF(S.EQ.0) THEN
            IF(MM.NE.1) THEN
              BT(LL+1,MM+1)=0
              BP(LL+1,MM+1)=0
            ELSE
              BT(LL+1,MM+1)=(LL*(LL+1)*(LL+2)*.5*(IC)**(LL+2)
     $           -(LL+1)*C*LL*(LL+1)*.5*(IC)**(LL+1))/SQT
              BP(LL+1,MM+1)=MM*LL*(LL+1)*.5*(IC)**(LL+1)/SQT
            ENDIF
          ELSE
            BT(LL+1,MM+1)=((LL-MM+1)*PLG(LL+2,MM+1)
     $      -(LL+1)*C*PLG(LL+1,MM+1))/(S*SQT)
            BP(LL+1,MM+1)=MM*PLG(LL+1,MM+1)/(S*SQT)
          ENDIF
   10   CONTINUE
   20 CONTINUE
      END
      SUBROUTINE LEGPOL(GLAT,L,M,PLG,LMAX)
C      CALCULATE LEGENDRE POLYNOMIALS PLG(L+1,M+1) FOR GEOGRAPHICAL
C      LATITUDE GLAT THROUGH ORDER L,M
      DIMENSION PLG(LMAX,1)
      DATA DGTR/1.74533E-2/
      IF(M.GT.L.OR.L.GT.LMAX-1) THEN
        WRITE(6,99) L,M,LMAX
   99 FORMAT(1X,'ILLEGAL INDICIES TO LEGPOL',3I5)
        RETURN
      ENDIF
      PLG(1,1)=1.
      IF(L.EQ.0.AND.M.EQ.0) RETURN
      C=SIN(GLAT*DGTR)
      S=COS(GLAT*DGTR)     
C      CALCULATE L=M CASE AND L=M+1
      DO 10 MM=0,M
        IF(MM.GT.0) PLG(MM+1,MM+1)=PLG(MM,MM)*(2.*MM-1.)*S
        IF(L.GT.MM) PLG(MM+2,MM+1)=PLG(MM+1,MM+1)*(2.*MM+1)*C
   10 CONTINUE
      IF(L.EQ.1) RETURN
      MMX=MIN(M,L-2)
      DO 30 MM=0,MMX
        DO 20 LL=MM+2,L
          PLG(LL+1,MM+1)=((2.*LL-1.)*C*PLG(LL,MM+1)-
     $     (LL+MM-1.)*PLG(LL-1,MM+1))/(LL-MM)
   20   CONTINUE
   30 CONTINUE
      RETURN
      END
      SUBROUTINE PRMW3
C          HWM87     4-NOV-87   
      DIMENSION ISD(3),IST(2)
      COMMON/PARMB/GSURF,RE
      COMMON/PARMW3/PWB1(50),PWB2(50),PWB3(50),PWC1(50),
     * PWC2(50),PWC3(50),PWC4(50)
      COMMON/DATIME/ISDATE(3),ISTIME(2),NAME(2)
      DATA GSURF,RE/980.665,6356.77/
      DATA ISD/' 4-N','OV-8','7   '/,IST/'17:3','9:09'/
      DATA NAME/'HWM8','7   '/
C         IRROTATIONAL WIND FIELD (DIVERGENCE FIELD)
      DATA PWB1/
     *  0.00000E+00,-1.11368E+01,-1.24858E+01, 1.10036E+02, 5.72915E+00,
     *  6.27672E+00, 7.79161E+01, 2.26817E+00, 1.48345E+00, 5.82466E+01,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00,-1.41351E+01, 0.00000E+00,
     *  0.00000E+00, 0.00000E+00, 1.59211E+01, 0.00000E+00, 0.00000E+00,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00, 4.01565E-02,
     *  8.29674E-02, 0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00,
     *  0.00000E+00, 3.36523E+01, 0.00000E+00, 0.00000E+00,-1.38693E+00,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 3.76631E+00,-2.15251E+00, 
     * -8.60431E-01, 0.00000E+00, 0.00000E+00, 4.15805E-02, 1.05645E-01,
     * -1.42416E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00/
      DATA PWB2/
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00,-1.55057E+00,
     *  2.24596E+00, 3.33952E+00, 0.00000E+00, 1.81985E+04, 0.00000E+00,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00,-1.50640E+02,-4.30700E+00,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00,
     *  0.00000E+00, 1.61029E+04, 0.00000E+00, 0.00000E+00, 0.00000E+00,
     * -2.42610E+04, 0.00000E+00, 0.00000E+00, 0.00000E+00,-1.46241E+04,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00,-2.29211E+00,-3.23009E+00,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00,
     *  5.12558E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00,
     *  0.00000E+00,-2.07665E+00,-1.67008E+02, 0.00000E+00, 0.00000E+00/
      DATA PWB3/
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00,-8.48854E+00, 0.00000E+00,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00, 4.75654E+00,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00,
     *  0.00000E+00, 6.85704E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00,
     *  0.00000E+00, 1.47178E+01, 0.00000E+00, 0.00000E+00, 0.00000E+00,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00/
C         SOLENOIDAL WIND FIELD (CURL FIELD)
      DATA PWC1/
     *  0.00000E+00,-5.26044E-01,-4.29072E+01, 2.44965E+01,-1.29728E+01,
     *  0.00000E+00, 3.82628E+01, 5.69565E+00, 0.00000E+00, 5.21497E+01,
     *  0.00000E+00,-1.13601E+01,-6.45490E+00,-8.45400E+00,-4.07973E+00,
     *  0.00000E+00, 0.00000E+00, 1.05267E+02, 0.00000E+00, 0.00000E+00,
     *  0.00000E+00, 0.00000E+00,-3.77522E+01,-4.06479E+00, 1.32400E-01,
     *  2.13300E-01,-7.54067E+00,-5.54014E-01, 1.05543E+01, 0.00000E+00,
     *  0.00000E+00,-1.57793E+02, 0.00000E+00,-4.48707E+00,-3.70910E+00,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 1.13100E-01, 1.69100E-01,
     * -2.29760E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00/
      DATA PWC2/
     *  0.00000E+00, 0.00000E+00, 5.37245E-02, 0.00000E+00,-3.78497E+00,
     *  6.88459E-02, 0.00000E+00, 0.00000E+00, 0.00000E+00,-1.44635E+01,
     *  5.43764E-02, 0.00000E+00, 0.00000E+00, 0.00000E+00,-6.94340E+00,
     * -7.24977E+00,-4.85673E+00, 0.00000E+00,-3.60101E+00,-2.23183E+01,
     * -5.12095E+01, 3.85590E+04,-1.33714E+00,-7.45792E-01, 0.00000E+00,
     * -5.90503E+03, 5.89137E-01,-1.06978E+00,-9.62826E-01, 2.76396E+04,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 4.72006E-02, 0.00000E+00,
     *  0.00000E+00, 5.34227E+00, 2.11187E+00, 0.00000E+00, 0.00000E+00,
     * -4.35022E-01, 7.38162E+00, 6.74852E+00, 0.00000E+00, 0.00000E+00,
     *  0.00000E+00,-2.56217E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00/
      DATA PWC3/
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00,
     * -4.92677E+01,-4.23828E+01, 0.00000E+00,-1.50460E+02, 0.00000E+00,
     *  0.00000E+00, 1.25104E-02, 0.00000E+00, 0.00000E+00, 0.00000E+00,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 2.47276E-02, 0.00000E+00,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00, 5.29549E-01,
     *  6.93886E-01, 7.47271E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00/
      DATA PWC4/
     *  3.28297E-01, 3.05222E+00,-2.57528E-01,-4.91601E-01, 2.13823E-01, 
     *  5.00134E-01, 2.17499E-01, 4.36531E-01, 0.00000E+00, 0.00000E+00,
     * -5.33759E+00,-1.39053E+01,-1.91954E+01, 0.00000E+00,-8.00168E-01,
     * -2.65953E-01, 0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00,
     *  4.24063E+00, 7.14516E+00, 3.89825E-01, 3.84909E+00,-9.55319E-01,
     *  2.20323E+00, 1.46817E-01, 5.46172E+00,-6.35371E-01, 6.19758E+00,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00,
     *  0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00, 0.00000E+00/
      DO 10 I=1,3
        ISDATE(I)=ISD(I)
   10 CONTINUE
      DO 20 I=1,2
        ISTIME(I)=IST(I)
   20 CONTINUE
      NAME(1)='HWM8'
      NAME(2)='7   '
      END
*******************************************************************

C      TEST DRIVER FOR GWS3 (HWM87 HORIZONTAL WIND MODEL)
      DIMENSION W(2,10)
      DIMENSION IDAY(10),UT(10),ALT(10),XLAT(10),XLONG(10),XLST(10),
     $ F107A(10),F107(10),AP(16)    
      COMMON/HWMC/WBT(2),WCT(2)
      DATA IDAY/172,81,8*172/
      DATA UT/29000.,29000.,75000.,7*29000./
      DATA ALT/400.,400.,400.,100.,6*400./
      DATA XLAT/4*60.,0.,5*60./
      DATA XLONG/5*-70.,0.,4*-70./
      DATA XLST/6*16.,4.,3*16./
      DATA F107A/7*150.,70.,150.,150./
      DATA F107/8*150.,180.,150./
      DATA AP/9*4.,40.,6*0/
      DO 10 I=1,10
         CALL GWS3(IDAY(I),UT(I),ALT(I),XLAT(I),XLONG(I),XLST(I),
     $             F107A(I),F107(I),AP(I),W(1,I))
         WRITE(6,100) W(1,I),WBT(1),WCT(1),W(2,I),WBT(2),WCT(2)
   10 CONTINUE
      WRITE(6,200) (IDAY(I),I=1,5)
      WRITE(6,201) (UT(I),I=1,5)
      WRITE(6,202) (ALT(I),I=1,5)
      WRITE(6,203) (XLAT(I),I=1,5)
      WRITE(6,204) (XLONG(I),I=1,5)
      WRITE(6,205) (XLST(I),I=1,5)
      WRITE(6,206) (F107A(I),I=1,5)
      WRITE(6,207) (F107(I),I=1,5)
      WRITE(6,210) (AP(I),I=1,5)
      WRITE(6,208) (W(1,I),I=1,5)
      WRITE(6,209) (W(2,I),I=1,5)
      WRITE(6,200) (IDAY(I),I=6,10)
      WRITE(6,201) (UT(I),I=6,10)
      WRITE(6,202) (ALT(I),I=6,10)
      WRITE(6,203) (XLAT(I),I=6,10)
      WRITE(6,204) (XLONG(I),I=6,10)
      WRITE(6,205) (XLST(I),I=6,10)
      WRITE(6,206) (F107A(I),I=6,10)
      WRITE(6,207) (F107(I),I=6,10)
      WRITE(6,210) (AP(I),I=6,10)
      WRITE(6,208) (W(1,I),I=6,10)
      WRITE(6,209) (W(2,I),I=6,10)
  100 FORMAT(1X,6F10.2)
  200 FORMAT(//' DAY  ',5I12)
  201 FORMAT(' UT   ',5F12.0)
  202 FORMAT(' ALT  ',5F12.0)
  203 FORMAT(' LAT  ',5F12.0)
  204 FORMAT(' LONG ',5F12.0)
  205 FORMAT(' LST  ',5F12.0)
  206 FORMAT(' F107A',5F12.0)
  207 FORMAT(' F107 ',5F12.0)
  210 FORMAT(' AP   ',5F12.0)
  208 FORMAT(/'   U  ',5F12.2)
  209 FORMAT('   V  ',5F12.2)
      STOP
      END
