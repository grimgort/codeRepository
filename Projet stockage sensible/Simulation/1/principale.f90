program generale

INTEGER(4) I, errnum
integer::j,n
double precision::t1,t2
n=8

CALL CPU_TIME(t1)
! Compilation de tous les programme fortran utilisé dans la simulation
 
   ! I = SYSTEM("gfortran -o increase_stock increase_stock.f90")
	I = SYSTEM("gfortran -o pause_stockage pause_stockage.f90")
    I = SYSTEM("gfortran -o destockage destockage.f90")
    I = SYSTEM("gfortran -o stockage stockage.f90")
    I = SYSTEM("gfortran -o pause_destockage pause_destockage.f90")
    
! Premier calcul du nombre de cofalites nécessaire

	I = SYSTEM("stockage")
	print*,'Fichier Increase_stock executé; execution de pause'
	print*,
	
	I = SYSTEM("pause_stockage")
	print*,'Fichier pause executé; execution de destockage'
	print*,
	
	I = SYSTEM("destockage")
	print*,'Fichier destockage executé; execution de pause_destockage'
	print*,
	
	I = SYSTEM("pause_destockage")	
	print*,'pause_destockage exucuté'
	print*,
	
	print*,'iteration pour production nominal'
! iteration du procédé pour simuler un stockage et destockage en production nominale.
do j=1,n
	I = SYSTEM("./stockage")
	I = SYSTEM("./pause_stockage")
	I = SYSTEM("./destockage")
	I = SYSTEM("pause_destockage")
enddo

CALL CPU_TIME(t2)
print*,t2-t1

end program