program generale

INTEGER(4) I, errnum
integer::j,n
n=2

! Compilation de tous les programme fortran utilisé dans la simulation
 
    I = SYSTEM("gfortran -o increase_stock increase_stock.f90")
	I = SYSTEM("gfortran -o pause_stockage pause_stockage.f90")
    I = SYSTEM("gfortran -o destockage destockage.f90")
    I = SYSTEM("gfortran -o stockage stockage.f90")
    I = SYSTEM("gfortran -o pause_destockage pause_destockage.f90")
    
! Premier calcul du nombre de cofalites nécessaire

	I = SYSTEM("increase_stock")
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
	
! iteration du procédé pour simuler un stockage et destockage en production nominale.
!do j=1,n
!	I = SYSTEM("./stockage")
!	I = SYSTEM("./pause")
!	I = SYSTEM("./destockage")
!enddo


end program