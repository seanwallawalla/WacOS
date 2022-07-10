10 REM  CRITICAL PATH METHOD (CPM)
20 REM  A()=START AND END NODES FOR EACH ACTIVITY
30 REM  S()=EARLY START TIMES FOR EACH ACTIVITY
40 REM  F()=LATE FINISH TIMES FOR EACH ACTIVITY
50 REM  E()=DURATIONS AND COSTS OF NORMAL ACTIVITIES
60 REM  C()=DURATIONS AND COSTS OF CRASH ACTIVITIES
70 CLS
80 KEY OFF
90 DIM A(100,2),S(100),F(100),E(100,2),C(100,2)
100 PRINT "CRITICAL PATH METHOD"
110 PRINT
120 PRINT "HOW MANY ACTIVITIES IN THIS NETWORK";
130 INPUT N
140 FOR I=1 TO N
150 PRINT
160 PRINT USING " ENTER START, END NODES FOR ACTIVITY ##"; I;
170 INPUT A(I,1),A(I,2)
180 IF A(I,2)<=A(I,1) THEN 200
190 IF A(I,2)<N THEN 250
200 PRINT " START NODE MUST BE NUMBERED LOWER";
210 PRINT " THAN END NODE, AND END NODE MUST";
220 PRINT " BE LESS THAN THE NUMBER OF ACTIVITIES.";
230 PRINT "       *** TRY ENTRY AGAIN ***"
240 GOTO 150
250 PRINT SPC(16)"ENTER DURATION AND COST";
260 INPUT E(I,1),E(I,2)
270 S(I)=0
280 F(I)=0
290 NEXT I
300 REM  LOOP TO FIND EARLY START TIMES FOR NETWORK
310 FOR I=1 TO N
320 IF S(A(I,2))>=S(A(I,1))+E(I,1) THEN 340
330 S(A(I,2))=S(A(I,1))+E(I,1)
340 NEXT I
350 F(A(N,2))=S(A(N,2))
360 REM  LOOP TO CALCULATE LATE FINISH TIMES FOR NETWORK
370 FOR I=N TO 1 STEP -1
380 IF F(A(I,1))=0 THEN 410
390 IF F(A(I,1))>F(A(I,2))-E(I,1) THEN 410
400 GOTO 420
410 F(A(I,1))=F(A(I,2))-E(I,1)
420 NEXT I
430 C1=0
440 L=0
450 PRINT
460 REM  CALCULATE SLACK TIME IN S1
470 PRINT "START END  EARLY  LATE"
480 PRINT "NODE  NODE START FINISH DUR. SLACK COST"
490 FOR I=1 TO N
500 PRINT A(I,1);TAB(7);A(I,2);TAB(12);S(A(I,1));TAB(18);
510 PRINT F(A(I,2));TAB(25);E(I,1);TAB(30);
520 S1=F(A(I,2))-S(A(I,1))-E(I,1)
530 IF S1>0 THEN 580
540 IF L>=F(A(I,2)) THEN 580
550 PRINT "CRIT.";
560 L=L+E(I,1)
570 GOTO 590
580 PRINT S1;
590 PRINT TAB(36);E(I,2)
600 C1=C1+E(I,2)
610 NEXT I
620 PRINT
630 PRINT "THE CRITICAL PATH LENGTH IS ";L
640 PRINT "TOTAL COST OF THIS NETWORK= ";C1
650 PRINT
660 PRINT "DO YOU WANT TO CHANGE ANY";
670 PRINT " ACTIVITY DURATIONS (Y/N)";
680 INPUT A$
690 IF A$="N" OR A$="n" THEN 870
700 IF A$<>"Y" AND A$<>"y" THEN 650
710 PRINT
720 PRINT "WHICH ACTIVITY ";
730 INPUT I
740 IF I<1 THEN 710
750 IF I>N THEN 710
760 PRINT "CURRENT DURATION IS ";E(I,1)
770 PRINT "COST = ";E(I,2)
780 PRINT "ENTER NEW DURATION AND COST ";
790 INPUT E(I,1),E(I,2)
800 PRINT "-----RECALCULATION NETWORK-----"
810 PRINT
820 FOR I=1 TO N
830 S(I)=0
840 F(I)=0
850 NEXT I
860 GOTO 300
870 END