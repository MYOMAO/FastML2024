C
	FUNCTION FGP1(X)
        COMMON/PACT/BB,B1,PHI,Z1
        SAVE  /PACT/
        COMMON/HIPARNT/HIPR1(100),IHPR2(50),HINT1(100),IHNT2(50)
        SAVE  /HIPARNT/
        EXTERNAL FGP2
        PHI=X
        FGP1=2.0*GAUSS3(FGP2,0.0,HIPR1(34),0.01)
	RETURN
	END
