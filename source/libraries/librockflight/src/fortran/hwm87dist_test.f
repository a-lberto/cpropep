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
