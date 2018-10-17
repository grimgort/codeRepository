program test
implicit none

!**********************************************************************************************************************
!**************************************** REMARQUE COMPLEMENTAIRE  ****************************************************
!**********************************************************************************************************************

!Validite du code pour une temperature d'air de -20°C a 350°C
!Pression possible : 4 bar, 27 bar, 50 bar, 70 bar


!**********************************************************************************************************************
!**************************************** DECLARATION  ****************************************************************
!**********************************************************************************************************************
!********************* Boucles et autres **********************
integer::i,k,boucle_temps,pas

!********************* Température ***************************
double precision,allocatable,dimension(:)::T_cof
double precision,dimension(:),allocatable::T_air_entree_stockage, T_cof_final, T_air_entree_destockage

!********************* Géométrie ******************************
double precision:: A_S, A_V, V_maille
double precision,dimension(:),allocatable::e, C_S

!********************* Lecture des variable *********************
integer::Nc, Nb_iteration_espace, Pression_stockage
double precision::dt_stockage
double precision::T_arret, largeur, debit_total_stockage
double precision::Ray, L_entre_C, T_entree_stockage, L, T_initial_C

!********************* Constante physique **********************
double precision::debit_stockage, Cp_cof, rho_cof, beta, cond_cof, masse_C, cond_air
integer:: Nb_iteration_temps, Nb_tube_largeur


!********************* Calcule du temps CPU *********************
double precision::t1, t2

!********************* Optimisation de A *********************
double precision::A_1_i_plus_1, A_1_i_i
double precision,dimension(:),allocatable::A_cof_cst_i_plus_1, A_cof_cst_i_moins_1, A_cof_cst_i_i

!********************* Calcul du gradient *********************
double precision::T_air_nb_tube_1, T_air_nb_tube_2, Gradient
integer::Parite

!********************* Criétre d'arret *********************
double precision::Hauteur,h_max

!********************* Calcul du nombre de tube *********************
double precision::coef_correctif

!********************* Calcul de l'energie echange *********************
double precision::energie,deltat_T,energie_d,deltat_T_d

!********************* Declaration version final *********************
integer::taille_matrice, indice_temps, w, x,m
double precision::t_max_stockage

double precision:: dt_destockage, t_max_destockage, T_entree_destockage, debit_total_destockage 
integer::Pression_destockage, Nb_iteration_temps_destockage, Nb_boucle_rajout_tube
double precision::debit_destockage
double precision,dimension(:),allocatable::T_verif_debit,rho_air



!**********************************************************************************************************************
!**************************************** LANCEMENT DU TEMPS CPU ******************************************************
!**********************************************************************************************************************
!**********************************************************************************************************************
CALL cpu_time(t1)




!**********************************************************************************************************************
!**************************************** LECTURES DES VARIABLES ******************************************************
!**********************************************************************************************************************
!**********************************************************************************************************************
call variable(Ray, L, L_entre_c, Nc, Nb_iteration_espace, dt_stockage, t_max_stockage, Pression_stockage, T_arret,&
				&T_entree_stockage, T_initial_C, largeur, debit_total_stockage,&
				&dt_destockage, t_max_destockage, Pression_destockage,&
				&T_entree_destockage, debit_total_destockage, Nb_boucle_rajout_tube )
				



!**********************************************************************************************************************
!**************************************** CALCULE DES VOLUMES ET SURFACES *********************************************
!**********************************************************************************************************************
!**********************************************************************************************************************
call Geometrie_simple(e, A_V, A_S, C_S, V_maille, Nc, Ray, L, L_entre_C)




!**********************************************************************************************************************
!**************************************** OUVERTURE DU FICHIER ECRITURE ***********************************************
!**********************************************************************************************************************
!**********************************************************************************************************************
open(UNIT=17,FILE='Temperature_stockage.txt',STATUS='UNKNOWN',form='formatted',ACTION='WRITE')
open(UNIT=18,FILE='Temperature_destockage.txt',STATUS='UNKNOWN',ACTION='WRITE')
open(UNIT=19,FILE='Energie_stocke.txt',STATUS='UNKNOWN',ACTION='WRITE')
open(UNIT=20,FILE='Energie_destocke.txt',STATUS='UNKNOWN',ACTION='WRITE')
open(UNIT=21,FILE='Temperature_sortie_air_stockage.txt',STATUS='UNKNOWN',ACTION='WRITE')
open(UNIT=22,FILE='Temperature_sortie_air_destockage.txt',STATUS='UNKNOWN',ACTION='WRITE')
open(UNIT=23,FILE='Temperature_air_stockage.txt',STATUS='UNKNOWN',ACTION='WRITE')
open(UNIT=24,FILE='Temperature_air_destockage.txt',STATUS='UNKNOWN',ACTION='WRITE')




!**********************************************************************************************************************
!**************************************** CONSTANTES ******************************************************************
!**********************************************************************************************************************
!**********************************************************************************************************************

!***************  Discretisation temporelle  ***************
Nb_iteration_temps=ceiling(t_max_stockage/dt_stockage)
Nb_iteration_temps_destockage=ceiling(t_max_destockage/dt_destockage)

!**************  Cofalite et air  **************
Cp_cof=900.
rho_cof=3120.
cond_cof=2.
masse_C=V_maille*rho_cof
cond_air=3.38D-2

!**************  Géométrie, débit **************
beta=L_entre_C+2.*Ray
Nb_tube_largeur=nint(largeur/beta)
debit_stockage=debit_total_stockage/(Nb_tube_largeur-1)/2
debit_destockage=debit_total_destockage/(Nb_tube_largeur-1)/2

!**************  Autres  **************
deltat_T=0
Energie=0
deltat_T_d=0
Energie_d=0




!*********************************************************************************************************************
!****************************************VERIFICATION DE LA CONSERVATION DE LA MASSE *********************************
!*********************************************************************************************************************
!*********************************************************************************************************************
allocate(T_verif_debit((Nc+1)*Nb_iteration_espace) )
T_verif_debit=T_entree_stockage
if (Pression_destockage==70) then
	call rho_air70(T_verif_debit,rho_air, Nb_iteration_espace)
else if (Pression_destockage==27) then
	call rho_air27(T_verif_debit,rho_air, Nb_iteration_espace)
else if (Pression_destockage==50) then
	call rho_air50(T_verif_debit,rho_air, Nb_iteration_espace)
else
	call rho_air4(T_verif_debit,rho_air, Nb_iteration_espace)
end if

if ( rho_air(Nc+1)*A_V <= debit_destockage*dt_destockage ) then
	print*,"temperature de verification=",T_entree_stockage	
	print*,"debit_destockage*dt_destockage=",debit_destockage*dt_destockage
	print*,"rho_air(T_entree_stockage)*A_V*2=",rho_air(Nc+1)*A_V
	print*,"Le programme diverge pour non conservation de la masse dans les mailles d'air. Diminuez le pas de temps ou le débit"
	print*,"pas de temps conseillé <",rho_air(Nc+1)*A_V/debit_destockage
	stop
end if

if ( rho_air(Nc+1)*A_V <= debit_stockage*dt_stockage ) then
	print*,"temperature de verification=",T_entree_stockage	
	print*,"debit_destockage*dt_destockage=",debit_stockage*dt_stockage
	print*,"rho_air(T_entree_stockage)*A_V*2=",rho_air(Nc+1)*A_V
	print*,"Le programme diverge pour non conservation de la masse dans les mailles d'air. Diminuez le pas de temps ou le débit"
	print*,"pas de temps conseillé <",rho_air(Nc+1)*A_V/debit_stockage
	stop
end if




!**********************************************************************************************************************
!**************************************** BOUCLE SUR LE NOMBRE DE TUBE  ***********************************************
!**********************************************************************************************************************
!**********************************************************************************************************************

!********************************************************************				
!************** Allocation et initialisation  ***********************
!********************************************************************				

allocate(T_cof( (Nc+1)*Nb_iteration_espace ) )
allocate(T_air_entree_stockage(Nb_iteration_temps))
allocate(T_cof_final( (Nc+1)*Nb_iteration_espace ))
allocate(T_air_entree_destockage(Nb_iteration_temps_destockage))

T_air_entree_destockage = T_entree_destockage
T_air_entree_stockage = T_entree_stockage


!********************************************************************				
!**************  Boucle sur le rajout de tube ***********************
!********************************************************************				
do x=1, Nb_boucle_rajout_tube
	print*,"Pour",x,"boucles de rajout de tubes"
	


	!*******************************				
	!**********  Stockage **********
	!*******************************	
	
	!************** Calcule des constantes **************
	call optimisation(A_1_i_plus_1, A_1_i_i, masse_C, Cp_cof, cond_cof, C_S, e, A_cof_cst_i_plus_1,&
				&A_cof_cst_i_moins_1, A_cof_cst_i_i, dt_stockage, Nc)
				
	!************** Initialisation pour le stockage  **************
	indice_temps=0
	T_cof=T_initial_C

	!************** Resolution pour le stockage (boucle sur le temps) **************
	Do w=1, Nb_iteration_temps
		!Calcule de l'indice de temps
		indice_temps=indice_temps+1

		!Resolution des equations physiques
		call element(T_cof, Nb_iteration_espace, Nc, dt_stockage, Pression_stockage, Ray, L, L_entre_C, debit_stockage, Cp_cof,&
				&rho_cof, beta, largeur, cond_cof, T_air_entree_stockage, A_S, A_V, C_S, V_maille, e, masse_C,&
				&cond_air, A_1_i_plus_1, A_1_i_i, A_cof_cst_i_plus_1, A_cof_cst_i_moins_1, A_cof_cst_i_i, indice_temps, debit_total_stockage)
		!Stockage de la nouvelle valeur de la temperature de l'air a l'instant dt
		T_air_entree_stockage(indice_temps)=T_cof( (Nc+1)*Nb_iteration_espace )
		
		!Ecriture des temperature du dernier element a cette instant  
		call ecriture_stock(x, w, T_cof, Nb_iteration_espace, dt_stockage, t_max_stockage)
	end do

	!************** Ecriture pour le stockage **************
	write(17,*),"rajout de tube n°",x
	do pas=1,(Nc+1)*Nb_iteration_espace
		Write(17,*),T_cof(pas)
	end do
	write(17,*)
	
	!************** Ecriture de l'energie stocke **************
	Do i=0,(Nb_iteration_espace-1)
		do k=i*(Nc+1)+1,i*(Nc+1)+Nc
	Deltat_T=Deltat_T+(T_cof(k)-T_initial_C)
		enddo
	enddo

	Energie=Deltat_T*Cp_cof*V_maille*rho_cof
	write(19,*)
	write(19,*),'Energie emmagasine par le cofalit (J) pour',x,'tubes'
	write(19,*)Energie
	write(19,*)

		print*,'Energie stockee:',Energie,'J'
	
	!************** Ecriture de la temperature de sortie de l'air **************
	do pas=1,Nb_iteration_espace
		Write(23,*),T_cof(pas*(Nc+1))
	end do





	!*******************************				
	!**********  De-stockage *******
	!*******************************				
	
	!************** Calcule des constantes **************
	call optimisation(A_1_i_plus_1, A_1_i_i, masse_C, Cp_cof, cond_cof, C_S, e, A_cof_cst_i_plus_1,&
				&A_cof_cst_i_moins_1, A_cof_cst_i_i, dt_destockage, Nc)
				
	!************** Initialisation pour le de-stockage  **************
	T_cof_final=T_cof
	indice_temps=0

	
	!************** Resolution pour le de-stockage (boucle sur le temps) **************
	do w=1, Nb_iteration_temps_destockage
		
		!Calcul de l'indice de temps
		indice_temps=indice_temps+1
		
		!Resolution des equations physiques
		call element(T_cof_final, Nb_iteration_espace, Nc, dt_destockage, Pression_destockage, Ray, L, L_entre_C, &
				&debit_destockage, Cp_cof,&
				&rho_cof, beta, largeur, cond_cof, T_air_entree_destockage, A_S, A_V, C_S, V_maille, e, masse_C,&
				&cond_air, A_1_i_plus_1, A_1_i_i, A_cof_cst_i_plus_1, A_cof_cst_i_moins_1, A_cof_cst_i_i, indice_temps, debit_total_destockage)
		
		!Recuperation de la temperature de l'air a cet instant
		T_air_entree_destockage(indice_temps)= T_cof_final( (Nc+1)*Nb_iteration_espace )
		
		!Ecriture des temperatures du dernier element a cette instant
		call ecriture_destock(x, w, T_cof_final, Nb_iteration_espace, dt_destockage, t_max_destockage)
	end do


	!************** Ecriture pour le de-stockage **************
	write(18,*),"rajout de tube n°",x
	do pas=1,(Nc+1)*Nb_iteration_espace
		Write(18,*),T_cof_final(pas)
	end do
	write(18,*)


	!************** Ecriture de l'energie de-stocke **************
	Do i=0,(Nb_iteration_espace-1)
		do k=i*(Nc+1)+1,i*(Nc+1)+Nc
	Deltat_T_d=Deltat_T_d+(T_cof_final(k)-T_cof(k))
		enddo
	enddo

	Energie_d=Deltat_T_d*Cp_cof*V_maille*rho_cof
	write(20,*)
	write(20,*),'Energie emmagasine par le cofalit (J) pour',x,'tubes'
	write(20,*)Energie_d
	write(20,*)	

	do pas=1,Nb_iteration_espace
		Write(24,*),T_cof_final(pas*(Nc+1))
	end do


		print*,'Energie destockee:',Energie_d,'J'





	!*******************************				
	!********** Test d'arret *******
	!*******************************	

if ( T_cof_final( (Nc+1)*Nb_iteration_espace ) > T_arret ) then
	print*, "La température de consigne sur le destockage a été atteint"
	print*, "T_sortie=",T_cof_final( (Nc+1)*Nb_iteration_espace )
	CALL cpu_time(t2)
	print*,"temps CPU de l'increase stockage=",t2-t1,'s'
	stop
end if




	!*******************************				
	!**Nouvelle iteration en espace**
	!*******************************	
	print*,"Longeur actuelle de l'echangeur =", x*Nb_iteration_espace*Ray*2,"m"
	print*, "Temperature de sortie de l'air =",T_cof_final( (Nc+1)*Nb_iteration_espace )
	print*,
	print*,"***************************************************************"
	print*,"**************** Iteration suivante : *************************"
	print*,"***************************************************************"
	

end do

print*,"Nombre de boucle maximal atteinte"




!**********************************************************************************************************************
!**************************************** FERMETURE DES FICHIERS ECRITURES ********************************************
!**********************************************************************************************************************
close(17)
close(18)
close(19)
close(20)
close(21)
close(22)
close(23)
close(24)

!**********************************************************************************************************************
!**************************************** TEMPS CPU *******************************************************************
!**********************************************************************************************************************

CALL cpu_time(t2)
print*,"temps CPU de l'increase stockage=",t2-t1,'s'



















!**********************************************************************************************************************
!**********************************************************************************************************************
!**************************************** SUBROUTINE ******************************************************************
!**********************************************************************************************************************
!**********************************************************************************************************************

contains
!****************************************************************************************************
!**************************************** ELEMENT ***************************************************
!****************************************************************************************************
!****************************************************************************************************
subroutine element(T_1, Nb_iteration_espace, Nc, dt, Pression, Ray, L, L_entre_C, debit, Cp_cof,&
				&rho_cof, beta, largeur, cond_cof, T_entree, A_S, A_V, C_S, V_maille, e, masse_C,&
				&cond_air, A_1_i_plus_1, A_1_i_i, A_cof_cst_i_plus_1, A_cof_cst_i_moins_1, A_cof_cst_i_i,&
				& Indice_temps,debit_total)
implicit none

!***********************************************************************************
!**************************************** DECLARATION ******************************
!***********************************************************************************
!********************* Boucle et autre  ************************
integer::i,j,k,pas
double precision,parameter:: Pi = 4.*atan(1.0)

!********************* Discrétisation  ************************
integer,intent(in):: Nc, Nb_iteration_espace
double precision,intent(in)::dt

!********************* Température  ************************
double precision,dimension(:),intent(inout)::T_1
double precision, allocatable, dimension(:)::T_2

!********************* Géométrie  ************************
double precision, intent(in)::A_V, A_S, V_maille, Ray, L, L_entre_C, beta, largeur
double precision,dimension(:),intent(in)::e,C_S

!********************* Variable physique  ************************
double precision,intent(in):: debit, Cp_cof, rho_cof, cond_cof, masse_C, cond_air, debit_total
integer,intent(in)::Pression
double precision,dimension(:),allocatable:: masse_A 
double precision,dimension(:),allocatable::rho_air, mu_air, Cp_air, Pr, h

!********************* Matrice de résolution  ************************
double precision,dimension(:,:),allocatable::A
double precision,dimension(:),allocatable::B

!********************* Optimisation de A  ************************
double precision, intent(in)::A_1_i_plus_1, A_1_i_i
double precision, dimension(:),intent(in)::A_cof_cst_i_plus_1, A_cof_cst_i_moins_1, A_cof_cst_i_i

!******************** Version final ***********************
double precision,dimension(:),intent(inout)::T_entree
integer,intent(in)::indice_temps
double precision::T_avant



T_avant=T_entree(Indice_temps)

!*****************************************************************************************
!****************************************ALLOCATION DE A ET B ****************************
!*****************************************************************************************
allocate(A((Nc+1)*Nb_iteration_espace,(Nc+1)*Nb_iteration_espace))
allocate(B((Nc+1)*Nb_iteration_espace))




!*******************************************************************************************
!****************************************CALCULE DE PARAMETRES PHYSIQUES *******************
!*******************************************************************************************
if (Pression==70) then
	call rho_air70(T_1,rho_air, Nb_iteration_espace)
	call Cp_air70(T_1,Cp_air, Nb_iteration_espace, T_avant)
else if (Pression==27) then
	call rho_air27(T_1,rho_air, Nb_iteration_espace)
	call Cp_air27(T_1,Cp_air, Nb_iteration_espace, T_avant)
else if (Pression==50) then
	call rho_air50(T_1,rho_air, Nb_iteration_espace)
	call Cp_air50(T_1,Cp_air, Nb_iteration_espace, T_avant)
else
	call rho_air4(T_1,rho_air, Nb_iteration_espace)
	call Cp_air4(T_1,Cp_air,Nb_iteration_espace, T_avant)
end if
Cp_air(:)=Cp_air(:)*1000.

call Convection(h, T_1, Pression, rho_air, Nb_iteration_espace, cond_air,largeur,L,debit_total)
call Calcule_masse(rho_air, A_V, masse_A, Nb_iteration_espace)




!********************************************************************************************
!****************************************CALCULE DES TEMPERATURES ***************************
!********************************************************************************************
A=0
B=0

!********************* Cas générale : premières mailles cofalite  ******************
do k=1, Nb_iteration_espace
	i=(k-1)*(Nc+1)+1
	A(i,i+1)=A_1_i_plus_1
	A(i,i)=A_1_i_i
	B(i)=T_1(i)
end do

!********************* Cas générale : mailles centrales cofalite  *******************
do k=1,Nb_iteration_espace
	do i=(k-1)*(Nc+1)+2, (k-1)*(Nc+1)+(Nc-1)
		j=i-( (k-1)*(Nc+1) )
		A(i,i+1)=A_cof_cst_i_plus_1(j)
		A(i,i-1)=A_cof_cst_i_moins_1(j)
		A(i,i)=A_cof_cst_i_i(j)
		B(i)=T_1(i)
	end do
end do

!********************* Cas générale : dernières mailles cofalite  ********************
do k=2,Nb_iteration_espace
	i=(k-1)*(Nc+1)+Nc
	A(i,i-1)=-dt/(masse_C*Cp_cof) * ( cond_cof*C_S(nc-1)/e(Nc-1) )
	A(i,i+1)=-dt/(masse_C*Cp_cof) * ( h(i+1)*C_S(Nc) )
	A(i,i)=1+dt/(masse_C*Cp_cof) * ( cond_cof*C_S(Nc-1)/e(Nc-1) +h(i+1)*C_S(Nc) )
	B(i)=T_1(i)
end do

!********************* Cas générale : mailles air  *********************************
do k=2,Nb_iteration_espace
	i=(Nc+1)*k
	A(i,i-(Nc+1))=-dt/(masse_A(i)*Cp_air(i)) * ( debit*Cp_air(i-(Nc+1)) )
	A(i,i-1)=-dt/(masse_A(i)*Cp_air(i)) * ( h(i)*C_S(Nc) )
	A(i,i)=1+dt/(masse_A(i)*Cp_air(i)) * ( h(i)*C_S(Nc)+debit*Cp_air(i) )
	B(i)=T_1(i)	
end do

!********************* Premier élément : dernière maille cofalite  *******************
A(Nc,Nc-1)=-dt/(masse_C*Cp_cof) * ( cond_cof*C_S(Nc-1)/e(Nc-1) )
A(Nc,Nc+1)=-dt/(masse_C*Cp_cof) * ( h(Nc+1)*C_S(Nc) )
A(Nc,Nc)=1+dt/(masse_C*Cp_cof) * ( cond_cof*C_S(Nc-1)/e(Nc-1) +h(Nc+1)*C_S(Nc) )
B(Nc)=T_1(Nc)

!********************* Premier élément : maille air  ********************************
A(Nc+1,Nc)=-dt/(masse_A(Nc+1)*Cp_air(Nc+1)) * ( h(Nc+1)*C_S(Nc) )
A(Nc+1,Nc+1)=1+dt/(masse_A(Nc+1)*Cp_air(Nc+1)) * ( h(Nc+1)*C_S(Nc)+debit*Cp_air(Nc+1) )

B(Nc+1)=T_1(Nc+1) + dt/(masse_A(Nc+1)*Cp_air(Nc+1)) * ( debit*Cp_air(1) * T_entree(indice_temps) )

!********************* Résolution  ************************************************
allocate(T_2((Nc+1)*Nb_iteration_espace))
call gauss1(A,B,T_2)
T_1(:)=T_2(:)

!********************* Désallocation  *********************************************
deallocate(T_2)
deallocate(h)
deallocate(rho_air)
deallocate(Cp_air)
deallocate(masse_A)
deallocate(B)
deallocate(A)

end subroutine





!****************************************************************************************************
!****************************************CALCULE DES VOLUMES ET SURFACES ****************************
!****************************************************************************************************
!****************************************************************************************************

subroutine geometrie_simple(e, A_V, A_S, C_S, V_maille, Nc, Ray, L, L_entre_C)
implicit none

integer,intent(in)::Nc
double precision,intent(in)::Ray,L,L_entre_C
double precision,dimension(:),allocatable,intent(out)::C_S, e
double precision,intent(out)::V_maille, A_V, A_S

integer::i
double precision::V_total
double precision,dimension(Nc):: r, n

double precision,parameter:: Pi = 4.*atan(1.0)

allocate( C_S(Nc) )
allocate( e(Nc-1))

V_total=pi*Ray**2./2.*L
V_maille=V_total/Nc

!********************* Calcule des rayons des maille ************************
do i=1,Nc
	r(i)=(2./pi/L*i*V_maille)**0.5
end do

!********************* Calcule des surfaces des maille de Cofalite************
do i=1,Nc
	C_S(i)=pi*r(i)*L
end do

!********************* Position des noeuds************************
n(1)=r(1)/2
do i=2,Nc
	n(i)=( r(i)+r(i-1) )/2.
end do

!********************* Epaisseur des mailles************************
do i=1,Nc-1
	e(i)=n(i+1)-n(i)
end do

!********************* Volume de l'air ************************
A_V=2.*Ray*L*(L_entre_C/2.+2.*Ray)-V_total

!********************* Surface nord et sud de l'air ************************
A_S=(L_entre_C/2.+2.*Ray)*L
end subroutine




!****************************************************************************************************
!****************************************CALCULE DE LA MASSE DE L'AIR *******************************
!****************************************************************************************************
!****************************************************************************************************
subroutine Calcule_masse(rho_air, A_V, masse_A, Nb_iteration_espace)
implicit none

double precision, dimension(:), allocatable, intent(out)::masse_A
double precision,intent(in)::A_V
double precision,dimension(:), intent(in)::rho_air
integer, intent(in)::Nb_iteration_espace
integer::k, i, NN

NN=size(rho_air)

allocate(masse_A(NN))




masse_A=0
do k=1,Nb_iteration_espace
	i=(Nc+1)*k
	masse_A(i)=A_V*rho_air(i)	
end do
end subroutine




!****************************************************************************************************
!****************************************CALCULE DE H ***********************************************
!****************************************************************************************************
!****************************************************************************************************
subroutine Convection(h, T_1, Pression, rho_air, Nb_iteration_espace, cond_air,largeur,L,debit_total)
implicit none

double precision,dimension(:),intent(in)::T_1
double precision,dimension(:),intent(in)::rho_air
double precision,dimension(:),allocatable,intent(out)::h
double precision,intent(in)::cond_air,largeur,L
integer,intent(in)::Nb_iteration_espace, Pression
double precision::S_l, S_t, S_d, debit_total

double precision,allocatable,dimension(:)::T_m, Re, Pr_s, Nu, mu_air, Pr,V_max
integer::NN,k

!********************* Allocation ************************************
NN=size(T_1)
allocate(h(NN))
allocate(T_m(NN))
allocate(Re(NN))
allocate(Pr_s(NN))
allocate(Nu(NN))
allocate(Pr(NN))
allocate(mu_air(NN))
allocate(V_max(NN))

!*********************  Vitesse  ******************************************
S_l=2.*Ray
S_t=beta
S_d=sqrt(S_l**2.+(S_t/2.)**2.)

!********************* Initialisation  ************************************
h=0

!*********************  Calcule de h  *******************************
do k=1,Nb_iteration_espace
	i=k*(Nc+1)
	T_m(i)=( T_1(i-1)+T_1(i) )/2.
end do

	if (Pression==70) then
		call Pr70(T_1,Pr_s,Nb_iteration_espace)
		call Pr70(T_m,Pr,Nb_iteration_espace)
		call mu_air70(T_1,mu_air,Nb_iteration_espace)
	else if (Pression==27) then
		call Pr27(T_1,Pr_s,Nb_iteration_espace)
		call Pr27(T_m,Pr,Nb_iteration_espace)
		call mu_air27(T_1,mu_air,Nb_iteration_espace)
	else if (Pression==50) then
		call Pr50(T_1,Pr_s,Nb_iteration_espace)
		call Pr50(T_m,Pr,Nb_iteration_espace)
		call mu_air50(T_1,mu_air,Nb_iteration_espace)
	else
		call Pr4(T_1,Pr_s,Nb_iteration_espace)
		call Pr4(T_m,Pr,Nb_iteration_espace)
		call mu_air4(T_1,mu_air,Nb_iteration_espace)
	end if


V_max=0
Re=0
Nu=0
do k=1,Nb_iteration_espace
	i=k*(Nc+1)
	
	If(S_d<((S_t+2.*Ray)/2.)) then
		V_max(i)=(S_t/(2.*(S_d-2.*Ray)))*debit_total/(largeur*L*rho_air(i))
		! la vitesse max du fluide se situe sur la diagonale entre deux cofalites de ligne différentes 
	else
		V_max(i)=(S_t/(S_t-2.*Ray))*debit_total/(largeur*L*rho_air(i))
		! La vitesse max est entre deux cofalites de la meme ligne
	endif
	
	Re(i)=rho_air(i)*V_max(i)*2*Ray/mu_air(i)

	If (Re(i)<500) then
		Nu(i)=1.04*Re(i)**0.4*Pr(i)**0.36*(Pr(i)/Pr_s(i))**0.25
	elseif (Re(i)<1000) then
		Nu(i)=0.71*Re(i)**0.5*Pr(i)**0.36*(Pr(i)/Pr_s(i))**0.25
	elseif (Re(i)<200000) then
		Nu(i)=0.35*(S_t/S_l)**0.2*Re(i)**0.6*Pr(i)**0.36*(Pr(i)/Pr_s(i))**0.25
	else
		Nu(i)=0.031*(S_t/S_l)**0.2*Re(i)**0.8*Pr(i)**0.36*(Pr(i)/Pr_s(i))**0.25
	endif

	h(i)=Nu(i)*cond_air/(2*Ray)
	
end do


deallocate(T_m)
deallocate(Re)
deallocate(Nu)
deallocate(Pr_s)
deallocate(mu_air)
deallocate(Pr)

end subroutine




!****************************************************************************************************
!****************************************  PROPRIETE DE L'AIR  **************************************
!****************************************************************************************************
!****************************************************************************************************

! *************************Proprietes de l'air Ã  70 bar*************************
subroutine rho_air70(T,rho_air,Nb_iteration_espace)
implicit none
double precision, dimension(:), intent(in) :: T
double precision, dimension(:), allocatable, intent(out) :: rho_air
integer,intent(in)::Nb_iteration_espace
integer::NN

NN=size(T)
allocate(rho_air(NN))
rho_air=0

do k=1,Nb_iteration_espace
	i=k*(Nc+1)
	rho_air(i) =5*10**(-14.)*(T(i)-273.15)**6 - 6*10**(-11.)*(T(i)-273.15)**5 + &
		&3*10**(-08.)*(T(i)-273.15)**4 - 1*10**(-05.)*(T(i)-273.15)**3 + 0.0021*(T(i)-273.15)**2 - 0.4188*(T(i)-273.15) + 91.888

end do
end subroutine


subroutine Cp_air70(T,Cp_air,Nb_iteration_espace,T_entree)
implicit none
double precision, dimension(:), intent(in) :: T
double precision,intent(in)::T_entree
double precision, dimension(:), allocatable, intent(out) :: Cp_air
integer,intent(in)::Nb_iteration_espace
integer::NN

NN=size(T)
allocate(Cp_air(NN))

Cp_air=0
Cp_air(1)= 4*10**(-16.)*(T_entree-273.15)**6 - 6*10**(-13.)*(T_entree-273.15)**5 + &
		&3*10**(-10.)*(T_entree-273.15)**4 - 8*10**(-08.)*(T_entree-273.15)**3 + &
		&10**(-05.)*(T_entree-273.15)**2 - 0.0017*(T_entree-273.15) + 1.1666

do k=1,Nb_iteration_espace
	i=k*(Nc+1)
	Cp_air(i) = 4*10**(-16.)*(T(i)-273.15)**6 - 6*10**(-13.)*(T(i)-273.15)**5 + &
		&3*10**(-10.)*(T(i)-273.15)**4 - 8*10**(-08.)*(T(i)-273.15)**3 + &
		&10**(-05.)*(T(i)-273.15)**2 - 0.0017*(T(i)-273.15) + 1.1666

end do
end subroutine


subroutine Pr70(T,Pr, Nb_iteration_espace)
implicit none
double precision, dimension(:), intent(in) :: T
double precision, dimension(:), allocatable, intent(out) :: Pr
integer,intent(in)::Nb_iteration_espace
integer::NN

NN=size(T)
allocate(Pr(NN))
Pr=0

do k=1,Nb_iteration_espace
	i=k*(Nc+1)
	Pr(i) = 3*10**(-16.)*(T(i)-273.15)**6 - 3*10**(-13.)*(T(i)-273.15)**5 + 2*10**(-10.)*(T(i)-273.15)**4&
		&- 4*10**(-08.)*(T(i)-273.15)**3 + 8*10**(-06.)*(T(i)-273.15)**2 - 0.001*(T(i)-273.15) + 0.7847
end do
end subroutine


subroutine mu_air70(T,mu_air, Nb_iteration_espace)
implicit none
double precision, dimension(:), intent(in) :: T
double precision, dimension(:), allocatable, intent(out) :: mu_air
integer,intent(in)::Nb_iteration_espace
integer::NN

NN=size(T)
allocate(mu_air(NN))
mu_air=0

do k=1,Nb_iteration_espace
	i=k*(Nc+1)
	mu_air(i) = 1*10**(-21.)*(T(i)-273.15)**6 - 2*10**(-18.)*(T(i)-273.15)**5 + 8*10**(-16.)*(T(i)-273.15)**4 &
		&- 2*10**(-13.)*(T(i)-273.15)**3 + 1*10**(-11.)*(T(i)-273.15)**2 + 4*10**(-08.)*(T(i)-273.15) + 2*10**(-05.)
end do
end subroutine

!*************************Proprietes de l'air Ã  50 bar*************************
subroutine Cp_air50(T,Cp_air, Nb_iteration_espace, T_entree)
implicit none
double precision, dimension(:), intent(in) :: T
double precision,intent(in)::T_entree
double precision, dimension(:), allocatable, intent(out) :: Cp_air
integer,intent(in)::Nb_iteration_espace
integer::NN

NN=size(T)
allocate(Cp_air(NN))

Cp_air=0
Cp_air(1) = 3*10**(-16.)*(T_entree-273.15)**6 - 4*10**(-13.)*(T_entree-273.15)**5 + 2*10**(-10.)*(T_entree-273.15)**4&
		&- 5*10**(-08.)*(T_entree-273.15)**3 + 10**(-05.)*(T_entree-273.15)**2 - 0.0012*(T_entree-273.15) + 1.1198

do k=1,Nb_iteration_espace
	i=k*(Nc+1)
	Cp_air(i) = 3*10**(-16.)*(T(i)-273.15)**6 - 4*10**(-13.)*(T(i)-273.15)**5 + 2*10**(-10.)*(T(i)-273.15)**4&
		&- 5*10**(-08.)*(T(i)-273.15)**3 + 10**(-05.)*(T(i)-273.15)**2 - 0.0012*(T(i)-273.15) + 1.1198
end do
end subroutine


subroutine rho_air50 (T,rho_air, Nb_iteration_espace)
implicit none
double precision, dimension(:), intent(in) :: T
double precision, dimension(:), allocatable, intent(out) :: rho_air
integer,intent(in)::Nb_iteration_espace
integer::NN

NN=size(T)
allocate(rho_air(NN))
rho_air=0

do k=1,Nb_iteration_espace
	i=k*(Nc+1)
	rho_air(i) =3*10**(-14.)*(T(i)-273.15)**6 - 3*10**(-11.)*(T(i)-273.15)**5 + 2*10**(-08.)*(T(i)-273.15)**4 &
			&- 6*10**(-06.)*(T(i)-273.15)**3 + 0.0014*(T(i)-273.15)**2 - 0.2833*(T(i)-273.15) + 65.333
end do
end subroutine


subroutine Pr50(T,Pr, Nb_iteration_espace)
implicit none
double precision, dimension(:), intent(in) :: T
double precision, dimension(:), allocatable, intent(out) :: Pr
integer,intent(in)::Nb_iteration_espace
integer::NN

NN=size(T)
allocate(Pr(NN))
Pr=0

do k=1,Nb_iteration_espace
	i=k*(Nc+1)
	Pr(i) =  2*10**(-16.)*(T(i)-273.15)**6 - 2*10**(-13.)*(T(i)-273.15)**5 + 1*10**(-10.)*(T(i)-273.15)**4 -&
		&3*10**(-08.)*(T(i)-273.15)**3 + 5*10**(-06.)*(T(i)-273.15)**2 - 0.0007*(T(i)-273.15) + 0.7648

end do
end subroutine


subroutine mu_air50(T,mu_air,Nb_iteration_espace)
implicit none
double precision, dimension(:), intent(in) :: T
double precision, dimension(:), allocatable, intent(out) :: mu_air
integer,intent(in)::Nb_iteration_espace
integer::NN

NN=size(T)
allocate(mu_air(NN))
mu_air=0

do k=1,Nb_iteration_espace
	i=k*(Nc+1)
	mu_air(i) = 5*10**(-22.)*(T(i)-273.15)**6 - 6*10**(-19.)*(T(i)-273.15)**5 + 3*10**(-16.)*(T(i)-273.15)**4 -&
		&7*10**(-14.)*(T(i)-273.15)**3 - 1*10**(-11.)*(T(i)-273.15)**2 + 5.*10**(-08.)*(T(i)-273.15) + 2.*10**(-05.)
end do
end subroutine


!*************************Proprietes de l'air Ã  27 bar*************************
subroutine Cp_air27(T,Cp_air, Nb_iteration_espace, T_entree)
implicit none
double precision, dimension(:), intent(in) :: T
double precision,intent(in)::T_entree
double precision, dimension(:), allocatable, intent(out) :: Cp_air
integer,intent(in)::Nb_iteration_espace
integer::NN

NN=size(T)
allocate(Cp_air(NN))

Cp_air=0
Cp_air(1) = 1*10**(-16.)*(T_entree-273.15)**6 - 2*10**(-13.)*(T_entree-273.15)**5 + &
		&8*10**(-11.)*(T_entree-273.15)**4 - 2*10**(-08.)*(T_entree-273.15)**3 +&
		&5*10**(-06.)*(T_entree-273.15)**2 - 0.0006*(T_entree-273.15) + 1.0655

do k=1,Nb_iteration_espace
	i=k*(Nc+1)
	Cp_air(i) = 1*10**(-16.)*(T(i)-273.15)**6 - 2*10**(-13.)*(T(i)-273.15)**5 + &
		&8*10**(-11.)*(T(i)-273.15)**4 - 2*10**(-08.)*(T(i)-273.15)**3 +&
		&5*10**(-06.)*(T(i)-273.15)**2 - 0.0006*(T(i)-273.15) + 1.0655
end do
end subroutine


subroutine rho_air27 (T,rho_air, Nb_iteration_espace)
implicit none
double precision, dimension(:), intent(in) :: T
double precision, dimension(:), allocatable, intent(out) :: rho_air
integer,intent(in)::Nb_iteration_espace
integer::NN

NN=size(T)
allocate(rho_air(NN))
rho_air=0

do k=1,Nb_iteration_espace
	i=k*(Nc+1)
	rho_air(i) =9*10**(-15.)*(T(i)-273.15)**6 - 1*10**(-11.)*(T(i)-273.15)**5 + 7*10**(-09.)*(T(i)-273.15)**4 &
		&- 2*10**(-06.)*(T(i)-273.15)**3 + 0.0006*(T(i)-273.15)**2 - 0.1412*(T(i)-273.15) + 34.959
end do
end subroutine


subroutine Pr27(T,Pr, Nb_iteration_espace)
implicit none
double precision, dimension(:), intent(in) :: T
double precision, dimension(:), allocatable, intent(out) :: Pr
integer,intent(in)::Nb_iteration_espace
integer::NN

NN=size(T)
allocate(Pr(NN))
Pr=0

do k=1,Nb_iteration_espace
	i=k*(Nc+1)
	Pr(i) = 6*10**(-17.)*(T(i)-273.15)**6 - 8*10**(-14.)*(T(i)-273.15)**5 + 4*10**(-11.)*(T(i)-273.15)**4 -&
		&1*10**(-08.)*(T(i)-273.15)**3 + 3*10**(-06.)*(T(i)-273.15)**2 - 0.0004*(T(i)-273.15) + 0.7404
end do
end subroutine


subroutine mu_air27(T,mu_air,Nb_iteration_espace)
implicit none
double precision, dimension(:), intent(in) :: T
double precision, dimension(:), allocatable, intent(out) :: mu_air
integer,intent(in)::Nb_iteration_espace
integer::NN

NN=size(T)
allocate(mu_air(NN))
mu_air=0

do k=1,Nb_iteration_espace
	i=k*(Nc+1)
	mu_air(i) = 3*10**(-23.)*(T(i)-273.15)**6 - 5*10**(-20.)*(T(i)-273.15)**5 + 2*10**(-17.)*(T(i)-273.15)**4 +&
		&2*10**(-14.)*(T(i)-273.15)**3 - 3*10**(-11.)*(T(i)-273.15)**2 + 5*10**(-08.)*(T(i)-273.15) + 2*10**(-05.)
end do
end subroutine

!*************************Proprietes de l'air Ã  4 bar*************************
subroutine Cp_air4(T,Cp_air, Nb_iteration_espace,T_entree)
implicit none
double precision, dimension(:), intent(in) :: T
double precision,intent(in)::T_entree
double precision, dimension(:), allocatable, intent(out) :: Cp_air
integer,intent(in)::Nb_iteration_espace
integer::NN

NN=size(T)
allocate(Cp_air(NN))

Cp_air=0
Cp_air(1) = 3*10**(-17.)*(T(i)-273.15)**6 - 4*10**(-14.)*(T(i)-273.15)**5 +&
		&2*10**(-11.)*(T(i)-273.15)**4 - 4*10**(-09.)*(T(i)-273.15)**3 +&
		&1*10**(-06.)*(T(i)-273.15)**2 - 5*10**(-05.)*(T(i)-273.15) + 1.0127

do k=1,Nb_iteration_espace
	i=k*(Nc+1)
	Cp_air(i) = 3*10**(-17.)*(T(i)-273.15)**6 - 4*10**(-14.)*(T(i)-273.15)**5 +&
		&2*10**(-11.)*(T(i)-273.15)**4 - 4*10**(-09.)*(T(i)-273.15)**3 +&
		&1*10**(-06.)*(T(i)-273.15)**2 - 5*10**(-05.)*(T(i)-273.15) + 1.0127

end do
end subroutine


subroutine rho_air4 (T,rho_air, Nb_iteration_espace)
implicit none
double precision, dimension(:), intent(in) :: T
double precision, dimension(:), allocatable, intent(out) :: rho_air
integer,intent(in)::Nb_iteration_espace
integer::NN

NN=size(T)
allocate(rho_air(NN))
rho_air=0

do k=1,Nb_iteration_espace
	i=k*(Nc+1)
	rho_air(i) = 7*10**(-16.)*(T(i)-273.15)**6 - 1*10**(-12.)*(T(i)-273.15)**5 + 6*10**(-10.)*(T(i)-273.15)**4 -&
		&2*10**(-07.)*(T(i)-273.15)**3 + 7*10**(-05.)*(T(i)-273.15)**2 - 0.019*(T(i)-273.15) + 5.1135

end do
end subroutine


subroutine Pr4(T,Pr, Nb_iteration_espace)
implicit none
double precision, dimension(:), intent(in) :: T
double precision, dimension(:), allocatable, intent(out) :: Pr
integer,intent(in)::Nb_iteration_espace
integer::NN

NN=size(T)
allocate(Pr(NN))
Pr=0

do k=1,Nb_iteration_espace
	i=k*(Nc+1)
	Pr(i) = 7*10**(-18.)*(T(i)-273.15)**6 - 8*10**(-15.)*(T(i)-273.15)**5 + 4*10**(-12.)*(T(i)-273.15)**4 -&
		&1*10**(-09.)*(T(i)-273.15)**3 + 7*10**(-07.)*(T(i)-273.15)**2 - 0.0002*(T(i)-273.15) + 0.7146
end do
end subroutine


subroutine mu_air4(T,mu_air,Nb_iteration_espace)
implicit none
double precision, dimension(:), intent(in) :: T
double precision, dimension(:), allocatable, intent(out) :: mu_air
integer,intent(in)::Nb_iteration_espace
integer::NN

NN=size(T)
allocate(mu_air(NN))
mu_air=0

do k=1,Nb_iteration_espace
	i=k*(Nc+1)
	mu_air(i) = 3*10**(-23.)*(T(i)-273.15)**6 - 7*10**(-21.)*(T(i)-273.15)**5 - 3*10**(-17.)*(T(i)-273.15)**4 +&
		&4*10**(-14.)*(T(i)-273.15)**3 - 4*10**(-11.)*(T(i)-273.15)**2 + 5*10**(-08.)*(T(i)-273.15) + 2*10**(-05.)
end do
end subroutine








!****************************************************************************************************
!****************************************  LECTURE DES VARIABLES  ***********************************
!****************************************************************************************************
!****************************************************************************************************
subroutine variable(Ray, L, L_entre_c, Nc, Nb_iteration_espace, dt_stockage, t_max_stockage, Pression_stockage, T_arret,&
				&T_entree_stockage, T_initial_C, largeur, debit_total_stockage,&
				&dt_destockage, t_max_destockage, Pression_destockage,&
				&T_entree_destockage, debit_total_destockage, Nb_boucle_rajout_tube )
implicit none

double precision,intent(out)::Ray, L, L_entre_c, dt_stockage, t_max_stockage
double precision,intent(out):: T_entree_stockage, T_initial_C, largeur, debit_total_stockage 

integer,intent(out)::Nc, Nb_iteration_espace, Pression_stockage
integer,intent(out)::Nb_boucle_rajout_tube

double precision,intent(out):: dt_destockage, t_max_destockage, T_entree_destockage, debit_total_destockage 
integer,intent(out)::Pression_destockage

double precision,intent(out)::T_arret




open(unit=11,FILE='donnees_echangeur.txt',status='old')
read(11,*)
read(11,*)
read(11,*)
read(11,*)
read(11,*)Ray
read(11,*)
read(11,*)L
read(11,*)
read(11,*)L_entre_c
read(11,*)
read(11,*)largeur
read(11,*)
read(11,*)
read(11,*)
read(11,*)Nc
close(11)



open(unit=12,FILE='donnees_stockage.txt',status='old')
read(12,*)
read(12,*)
read(12,*)
read(12,*)
read(12,*)dt_stockage
read(12,*)
read(12,*)t_max_stockage
read(12,*)
read(12,*)
read(12,*)
read(12,*)Pression_stockage
read(12,*)
read(12,*)T_entree_stockage
read(12,*)
read(12,*)T_initial_C
read(12,*)
read(12,*)debit_total_stockage
close(unit=12)





open(unit=13,FILE='Donnees_destockage.txt')
read(13,*)
read(13,*)
read(13,*)
read(13,*)
read(13,*)dt_destockage
read(13,*)
read(13,*)t_max_destockage
read(13,*)
read(13,*)
read(13,*)
read(13,*)Pression_destockage
read(13,*)
read(13,*)T_entree_destockage
read(13,*)
read(13,*)debit_total_destockage
close(unit=13)




open(unit=14,FILE='donnees_tube.txt',status='old')
read(14,*)
read(14,*)
read(14,*)
read(14,*)
read(14,*)Nb_iteration_espace
read(14,*)
read(14,*)Nb_boucle_rajout_tube
read(14,*)
read(14,*)T_arret
close(unit=14)
end subroutine





!****************************************************************************************************
!***************************************** FONCTION DE RESOLUTION TYPE GAUSS ************************
!****************************************************************************************************
!****************************************************************************************************
subroutine gauss1(A,B,T_2)
implicit none

	double precision, dimension(:),intent(in)::B
	double precision, dimension(:,:),intent(in):: A
	double precision, allocatable,dimension(:),intent(out)::T_2
	
	double precision, allocatable,dimension(:,:)::xA
	double precision, allocatable,dimension(:)::xB
		
	integer::nn,j,k,l
	double precision::di,somme,dp
	
	nn=size(A,1)
	allocate(T_2(nn))
	allocate(xA(nn,nn))
	allocate(xB(nn))

!print*,size(xa),xb

xA=A
xB=B

!    Premier Balayage
	do k=1,nn-1
	   dp=1/xA(k,k)
	   do j=k,nn
	      xA(k,j)=xA(k,j)*dp
	   end do
	   xB(k)=xB(k)*dp
	   do j=k+1,nn
		  di=xA(j,k)
		  do l=k,nn
		     xA(j,l)=xA(j,l)-di*xA(k,l)
		  end do
		  xB(j)=xB(j)-di*XB(k)
	   end do
	end do

 !    Second Balayage
	T_2(nn)=xB(nn)/xA(nn,nn)
	do i=1,nn-1
	   Somme=0
	   do j=nn-i+1,nn
	      Somme=Somme+xA(nn-i,j)*T_2(j)
	      T_2(nn-i)=xB(nn-i)-Somme
	   end do
	end do
     
deallocate(xA)
deallocate(xB)

end subroutine







!****************************************************************************************************
!***************************************** ECRITURE *************************************************
!****************************************************************************************************
!****************************************************************************************************
subroutine ecriture_stock(x, w, T_cof, Nb_iteration_espace, dt, t_max)
implicit none

integer,intent(in):: w, Nb_iteration_espace,x
double precision,intent(in)::t_max,dt
double precision::temps,t_test
double precision,dimension(:), intent(in)::T_cof
integer::b

temps=w*dt
t_test=t_max/10

if ( abs(temps-dt)<10**(-5.) ) then
	write(21,*)
	write(21,*) "iteration numero=",x
end if

do b=1,10 
	if ( abs(temps-(b*t_test))<10**(-5.) ) then
		write(21,*) temps, T_cof( (Nc+1)*Nb_iteration_espace )
	end if
end do

end subroutine


subroutine ecriture_destock(x, w, T_cof, Nb_iteration_espace, dt, t_max)
implicit none

integer,intent(in)::w, Nb_iteration_espace, x
double precision,intent(in)::t_max,dt
double precision::temps,t_test
double precision,dimension(:), intent(in)::T_cof
integer::b

temps=w*dt
t_test=t_max/10

if ( abs(temps-dt)<10**(-5.) ) then
	write(22,*)
	write(22,*) "iteration numero=",x
end if

do b=1,10 
	if ( abs(temps-(b*t_test))<10**(-5.) ) then
		write(22,*) temps, T_cof( (Nc+1)*Nb_iteration_espace )
	end if
end do

end subroutine



!**********************************************************************************************************************
!**************************************** OPTIMISATION DE A : calcul des constantes ***********************************
!**********************************************************************************************************************
!**********************************************************************************************************************
subroutine optimisation(A_1_i_plus_1, A_1_i_i, masse_C, Cp_cof, cond_cof, C_S, e, A_cof_cst_i_plus_1,&
		&A_cof_cst_i_moins_1, A_cof_cst_i_i, dt, Nc)
		
double precision,intent(in)::masse_C,Cp_cof,cond_cof,dt
double precision,dimension(:),intent(in)::C_S,e
integer,intent(in)::Nc 
double precision,intent(out)::A_1_i_plus_1,A_1_i_i
double precision,dimension(:),allocatable,intent(out)::A_cof_cst_i_plus_1,A_cof_cst_i_moins_1,A_cof_cst_i_i

!************** Cas générale : premières mailles cofalite  **************
A_1_i_plus_1=-dt/(masse_C*Cp_cof) * ( cond_cof*C_S(1)/e(1) )
A_1_i_i=1+dt/(masse_C*Cp_cof) * ( cond_cof*C_S(1)/e(1) )

!************** Cas générale : mailles centrales cofalite  **************
allocate(A_cof_cst_i_plus_1(Nc-1))
allocate(A_cof_cst_i_moins_1(Nc-1))
allocate(A_cof_cst_i_i(Nc-1))

A_cof_cst_i_plus_1=0
A_cof_cst_i_moins_1=0
A_cof_cst_i_i=0

do k=2,Nc-1
		A_cof_cst_i_plus_1(k)=-dt/(masse_C*Cp_cof) * ( cond_cof*C_S(k)/e(k) )
		A_cof_cst_i_moins_1(k)=-dt/(masse_C*Cp_cof) * ( cond_cof*C_S(k-1)/e(k-1) )
		A_cof_cst_i_i(k)=1+dt/(masse_C*Cp_cof) * ( cond_cof*C_S(k)/e(k)+cond_cof*C_S(k-1)/e(k-1) )
end do



end subroutine















!**********************************************************************************************************************
!**********************************************************************************************************************
!**************************************** FIN DU PROGRAMME ************************************************************
!**********************************************************************************************************************
!**********************************************************************************************************************
end program 