C
C
C
	FUNCTION GHVQ(Y1,Y2,AMT2)
	IMPLICIT REAL*8 (A-H,O-Z)
	REAL HIPR1(100),HINT1(100)
        COMMON/HIPARNT/HIPR1,IHPR2(50),HINT1,IHNT2(50)
        SAVE  /HIPARNT/
	DIMENSION F(2,7)
	XT=2.0*DSQRT(AMT2)/HINT1(1)
	X1=0.50*XT*(DEXP(Y1)+DEXP(Y2))
	X2=0.50*XT*(DEXP(-Y1)+DEXP(-Y2))
	SS=X1*X2*HINT1(1)**2
	AF=4.0
	IF(IHPR2(18).NE.0) AF=5.0
	DLAM=HIPR1(15)
	APH=12.0*3.1415926/(33.0-2.0*AF)/DLOG(AMT2/DLAM**2)
C
	CALL PARTON(F,X1,X2,AMT2)
C
	Gqq=4.0*(COSH(Y1-Y2)+HIPR1(7)**2/AMT2)/(1.D0+COSH(Y1-Y2))/9.0
     &      *(F(1,1)*F(2,2)+F(1,2)*F(2,1)+F(1,3)*F(2,4)
     &        +F(1,4)*F(2,3)+F(1,5)*F(2,6)+F(1,6)*F(2,5))
	Ggg=(8.D0*COSH(Y1-Y2)-1.D0)*(COSH(Y1-Y2)+2.0*HIPR1(7)**2/AMT2
     &      -2.0*HIPR1(7)**4/AMT2**2)/(1.0+COSH(Y1-Y2))/24.D0
     &      *F(1,7)*F(2,7)
C
	GHVQ=(Gqq+Ggg)*HIPR1(23)*3.14159*APH**2/SS**2
	RETURN
	END
