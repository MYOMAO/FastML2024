C
C
C
	FUNCTION FNSTRUM(X)
	COMMON/HIPARNT/HIPR1(100),IHPR2(50),HINT1(100),IHNT2(50)
	SAVE  /HIPARNT/
	FNSTRUM=1.0/((1.0-X)**2+HIPR1(45)**2/HINT1(1)**2)**HIPR1(46)
     &          /(X**2+HIPR1(45)**2/HINT1(1)**2)**HIPR1(46)
	RETURN
	END
