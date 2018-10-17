program test
implicit none

!**********************************************************************************************************************
!**************************************** DECLARATION  *************************************************************
!**********************************************************************************************************************
!********************* Boucles et autres *********************
integer::i,k,boucle_temps,ios,ios_fermeture

!********************* Température *********************
double precision,allocatable,dimension(:)::T_1

!********************* Géométrie *********************
double precision:: A_S, A_V, V_maille
double precision,dimension(:),allocatable::e, C_S


!********************* Lecture des variable *********************
integer::Nc, Nb_iteration_espace, Pression, augmentation_hauteur
double precision::t_max, dt
double precision::T_arret, largeur, debit_total
double precision::Ray, L_entre_C, T_entree, L, T_initial_C


!********************* Constante physique *********************
double precision::debit, Cp_cof, rho_cof, beta, cond
integer:: Nb_iteration_temps, Nb_tube_largeur

!********************* Calcule du temps CPU *********************
double precision::t1, t2




!**********************************************************************************************************************
!**************************************** LANCEMENT DU TEMPS CPU *************************************************
!**********************************************************************************************************************
CALL cpu_time(t1)




!**********************************************************************************************************************
!**************************************** LECTURES DES VARIABLES *************************************************
!**********************************************************************************************************************
call variable(Ray, L, L_entre_C, Nc, Nb_iteration_espace, dt, t_max, Pression, T_entree, &
	&T_initial_C, largeur, debit_total, augmentation_hauteur, T_arret)
!*************grosse dépendence au pas de temp pour la convergence dut à un faible rho_air*************
!*************plus le débit est faible mieux sa converge**************

!*************debugage ( à suprimer)**************
dt=1
t_max=100
Nc=3
Nb_iteration_espace=3
augmentation_hauteur=20
T_arret=150
!***********************************************




!**********************************************************************************************************************
!**************************************** OUVERTURE DU FICHIER ECRITURE ****************************************
!**********************************************************************************************************************
open(UNIT=12,FILE='temperature.txt',STATUS='UNKNOWN',form='formatted',ACTION='WRITE',iostat=ios)
print*,'controle de l''ouverture du fichier',ios,'(0:oui, 1+:non)'
WRITE(12,*),ios

!**********************************************************************************************************************
!**************************************** CONSTANTES **************************************************************
!**********************************************************************************************************************
Nb_iteration_temps=t_max/dt


Cp_cof=900
rho_cof=3120
cond=2

beta=L_entre_C+2*Ray
Nb_tube_largeur=nint(largeur/beta)

debit=debit_total/(Nb_tube_largeur-1)




!**********************************************************************************************************************
!****************************************CALCULE DES VOLUMES ET SURFACES **************************************
!**********************************************************************************************************************
call Geometrie_simple(e, A_V, A_S, C_S, V_maille, Nc, Ray, L, L_entre_C)




!**********************************************************************************************************************
!**************************************** BOUCLE SUR LE NOMBRE DE TUBE  *****************************************
!**********************************************************************************************************************
!******** initialisation pour entree dans la boucle DO WHILE avec correction de l'increment 
!Nb_iteration_espace=Nb_iteration_espace-augmentation_hauteur  (A REMETTRE SI ON PART D'UNE HAUTEUR MAX; Nb_iteration_espace DOIT ETRE SUPERIEUR A augmentation_hauteur)
allocate(T_1( (Nc+1)*Nb_iteration_espace ) )
T_1( (Nc+1)*Nb_iteration_espace )=T_entree

DO WHILE(T_1((Nc+1)*Nb_iteration_espace)>T_arret+273.15)




!**********************************************************************************************************************
!**************************************** RESOLUTION **************************************************************
!**********************************************************************************************************************
deallocate(T_1)

Nb_iteration_espace=Nb_iteration_espace+augmentation_hauteur
allocate(T_1( (Nc+1)*Nb_iteration_espace ) )

!********************* Condition initial *********************
T_1=T_initial_C
do k=1, Nb_iteration_espace
	i=k*(Nc+1)
	T_1(i)=T_entree
end do

!********************* Boucle en temps *********************	
do boucle_temps=1,Nb_iteration_temps
	call Element(T_1, Nb_iteration_espace, Nc, dt, Pression, Ray, L, L_entre_C, debit, Cp_cof, rho_cof, beta,&
				&largeur, cond,T_entree, A_S, A_V, C_S, V_maille, e)			
				
end do

call ecriture(T_1,Nb_iteration_espace,Nc)


enddo


CLOSE(12,status='keep',iostat=ios_fermeture)

print*,'ficier correctement ferme:',ios_fermeture,'(0:oui, 1+:non)'
call cpu_time(t2)
print*,t2-t1














contains



subroutine element(T_1, Nb_iteration_espace, Nc, dt, Pression, Ray, L, L_entre_C, debit, Cp_cof,&
				&rho_cof, beta, largeur, cond,T_entree, A_S, A_V, C_S, V_maille, e)
implicit none

!**********************************************************************************************************************
!**************************************** DECLARATION **************************************************************
!**********************************************************************************************************************
!********************* Boucle et autre  ************************
integer::i,j,k,pas
double precision,parameter:: Pi = 3.1415

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
double precision,intent(in):: debit, Cp_cof, rho_cof, cond,T_entree
integer,intent(in)::Pression
double precision:: masse_C
double precision,dimension(:),allocatable:: masse_A 
double precision,dimension(:),allocatable::rho_air, mu_air, Cp_air, Pr, h


!********************* Matrice de résolution  ************************
double precision,dimension(:,:),allocatable::A
double precision,dimension(:),allocatable::B




!**********************************************************************************************************************
!****************************************ALLOCATION DE A ET B *****************************************************
!**********************************************************************************************************************
allocate(A((Nc+1)*Nb_iteration_espace,(Nc+1)*Nb_iteration_espace))
allocate(B((Nc+1)*Nb_iteration_espace))
A=0




!**********************************************************************************************************************
!****************************************CALCULE DE PARAMETRES PHYSIQUES **************************************
!**********************************************************************************************************************
if (Pression==70) then
	call rho_air70(T_1,rho_air, Nb_iteration_espace)
	call Cp_air70(T_1,Cp_air, Nb_iteration_espace, T_entree)
else if (Pression==27) then
	call rho_air27(T_1,rho_air, Nb_iteration_espace)
	call Cp_air27(T_1,Cp_air, Nb_iteration_espace, T_entree)
else
	call rho_air4(T_1,rho_air, Nb_iteration_espace)
	call Cp_air4(T_1,Cp_air,Nb_iteration_espace, T_entree)
end if
Cp_air(:)=Cp_air(:)*1000


call Convection(h, Ray, beta, debit, largeur, L, T_1, Pression, rho_air,Nb_iteration_espace)
call Calcule_masse(rho_air,rho_cof,V_maille,A_V,masse_C,masse_A, Nb_iteration_espace)




!**********************************************************************************************************************
!****************************************CALCULE DES TEMPERATURES ***********************************************
!**********************************************************************************************************************

!********************* Cas générale : premières mailles cofalite  ************************
do k=1, Nb_iteration_espace
	i=(k-1)*(Nc+1)+1
	A(i,i+1)=-dt/(masse_C*Cp_cof) * ( cond*C_S(1)/e(1) )
	A(i,i)=1+dt/(masse_C*Cp_cof) * ( cond*C_S(1)/e(1) )
	B(i)=T_1(i)
end do

!********************* Cas générale : mailles centrales cofalite  ************************
do k=1,Nb_iteration_espace
	do i=(k-1)*(Nc+1)+2, (k-1)*(Nc+1)+(Nc-1)
		j=i-( (k-1)*(Nc+1) )
		A(i,i+1)=-dt/(masse_C*Cp_cof) * ( cond*C_S(j)/e(j) )
		A(i,i-1)=-dt/(masse_C*Cp_cof) * ( cond*C_S(j-1)/e(j-1) )
		A(i,i)=1+dt/(masse_C*Cp_cof) * ( cond*C_S(j)/e(j)+cond*C_S(j-1)/e(j-1) )
		B(i)=T_1(i)
	end do
end do

!********************* Cas générale : dernières mailles cofalite  ************************
do k=2,Nb_iteration_espace
	i=(k-1)*(Nc+1)+Nc
	A(i,i-1)=-dt/(masse_C*Cp_cof) * ( cond*C_S(nc-1)/e(Nc-1) )
	A(i,i+1)=-dt/(masse_C*Cp_cof) * ( h(i+1)*C_S(Nc) )
	A(i,i)=1+dt/(masse_C*Cp_cof) * ( cond*C_S(Nc-1)/e(Nc-1) +h(i+1)*C_S(Nc) )
	B(i)=T_1(i)
end do

!********************* Cas générale : mailles air  ************************
do k=2,Nb_iteration_espace
	i=(Nc+1)*k
	A(i,i-(Nc+1))=-dt/(masse_A(i)*Cp_air(i)) * ( debit*Cp_air(i-(Nc+1)) )
	A(i,i-1)=-dt/(masse_A(i)*Cp_air(i)) * ( h(i)*A_S )
	A(i,i)=1+dt/(masse_A(i)*Cp_air(i)) * ( h(i)*A_S+debit*Cp_air(i) )
	B(i)=T_1(i)	
end do

!********************* Premier élément : dernière maille cofalite  ************************
A(Nc,Nc-1)=-dt/(masse_C*Cp_cof) * ( cond*C_S(nc-1)/e(Nc-1) )
A(Nc,Nc+1)=-dt/(masse_C*Cp_cof) * ( h(Nc+1)*C_S(Nc) )
A(Nc,Nc)=1+dt/(masse_C*Cp_cof) * ( cond*C_S(Nc-1)/e(Nc-1) +h(Nc+1)*C_S(Nc) )
B(Nc)=T_1(Nc)

!********************* Premier élément : maille air  ************************************
A(Nc+1,Nc)=-dt/(masse_A(Nc+1)*Cp_air(Nc+1)) * ( h(Nc+1)*A_S )
A(Nc+1,Nc+1)=1+dt/(masse_A(Nc+1)*Cp_air(Nc+1)) * ( h(Nc+1)*A_S+debit*Cp_air(Nc+1) )

B(Nc+1)=T_1(Nc+1) + dt/(masse_A(Nc+1)*Cp_air(Nc+1)) * ( debit*Cp_air(1) * T_entree )

!********************* Résolution  ****************************************************
allocate(T_2((Nc+1)*Nb_iteration_espace))
call gauss1(A,B,T_2)










T_1(:)=T_2(:)


deallocate(T_2)
deallocate(h)
deallocate(rho_air)
deallocate(Cp_air)
deallocate(masse_A)
deallocate(B)
deallocate(A)

end subroutine





!**********************************************************************************************************************
!****************************************CALCULE DES VOLUMES ET SURFACES **************************************
!**********************************************************************************************************************
subroutine geometrie_simple(e, A_V, A_S, C_S, V_maille, Nc, Ray, L, L_entre_C)
implicit none

integer,intent(in)::Nc
double precision,intent(in)::Ray,L,L_entre_C
double precision,dimension(:),allocatable,intent(out)::C_S, e
double precision,intent(out)::V_maille, A_V, A_S

integer::i
double precision::V_total
double precision,dimension(Nc):: r, n

double precision,parameter:: Pi = 3.1415

allocate( C_S(Nc) )
allocate( e(Nc-1))

V_total=pi*Ray**2/2*L
V_maille=V_total/Nc

!********************* Calcule des rayons des maille ************************
do i=1,Nc
	r(i)=(2/pi/L*i*V_maille)**0.5
end do

!********************* Calcule des surfaces des maille de Cofalite************
do i=1,Nc
	C_S(i)=pi*r(i)*L
end do

!********************* Position des noeuds************************
n(1)=r(1)/2
do i=2,Nc
	n(i)=( r(i)+r(i-1) )/2
end do

!********************* Epaisseur des mailles************************
do i=1,Nc-1
	e(i)=n(i+1)-n(i)
end do

!********************* Volume de l'air ************************
A_V=2*Ray*L*(L_entre_C/2+2*Ray)-V_total

!********************* Surface nord et sud de l'air ************************
A_S=(L_entre_C/2+2*Ray)*L
end subroutine




!**********************************************************************************************************************
!****************************************CALCULE DE LA MASSE ******************************************************
!**********************************************************************************************************************
subroutine Calcule_masse(rho_air,rho_cof,V_maille,A_V,masse_C,masse_A, Nb_iteration_espace)
implicit none

double precision,intent(out)::masse_C 
double precision, dimension(:), allocatable, intent(out)::masse_A
double precision,intent(in)::rho_cof, V_maille, A_V
double precision,dimension(:), intent(in)::rho_air
integer, intent(in)::Nb_iteration_espace
integer::k, i, NN

NN=size(rho_air)

allocate(masse_A(NN))


masse_C=V_maille*rho_cof

masse_A=0
do k=1,Nb_iteration_espace
	i=(Nc+1)*k
	masse_A(i)=A_V*rho_air(i)	
end do


end subroutine




!**********************************************************************************************************************
!****************************************CALCULE DE H ***************************************************************
!**********************************************************************************************************************
subroutine convection(h, Ray, beta, debit, largeur, L, T_1, Pression, rho_air,Nb_iteration_espace)
implicit none

double precision,dimension(:),intent(in)::T_1
double precision,dimension(:),intent(in)::rho_air
double precision,dimension(:),allocatable,intent(out)::h
double precision,intent(in)::Ray,beta,debit,largeur,L
integer,intent(in)::Nb_iteration_espace

double precision,allocatable,dimension(:)::T_m, Re, Pr_s, Nu, mu_air, Pr
double precision::S_l,S_d,S_t,V_max,cond_air
integer::NN,k
integer::Pression

!********************* Allocation ************************************
NN=size(T_1)
allocate(h(NN))
allocate(T_m(NN))
allocate(Re(NN))
allocate(Pr_s(NN))
allocate(Nu(NN))
allocate(Pr(NN))
allocate(mu_air(NN))


!********************* Initialisation  ************************************
h=0

!*********************  Calcule des constante  *******************************
cond_air=0.0338
S_l=2*Ray
S_t=beta
S_d=sqrt(S_l**2+(S_t/2)**2)

If(S_d<((S_t+2*Ray)/2)) then
	V_max=(S_t/(2*(S_d-2*Ray)))*debit/(largeur*L)
		! la vitesse max du fluide se situe sur la diagonale entre deux cofalites de ligne diffÃ©rentes 
else
	V_max=(S_t/(S_t-2*Ray))*debit/(largeur*L)
		! La vitesse max est entre deux cofalites de la meme ligne
endif


!*********************  Calcule de h  *******************************
do k=1,Nb_iteration_espace
	i=k*(Nc+1)
	T_m(i)=( T_1(i-1)+T_1(i) )/2
end do

	if (Pression==70) then
		call Pr70(T_1,Pr_s,Nb_iteration_espace)
		call Pr70(T_m,Pr,Nb_iteration_espace)
		call mu_air70(T_1,mu_air,Nb_iteration_espace)
	else if (Pression==27) then
		call Pr27(T_1,Pr_s,Nb_iteration_espace)
		call Pr27(T_m,Pr,Nb_iteration_espace)
		call mu_air27(T_1,mu_air,Nb_iteration_espace)
	else
		call Pr4(T_1,Pr_s,Nb_iteration_espace)
		call Pr4(T_m,Pr,Nb_iteration_espace)
		call mu_air4(T_1,mu_air,Nb_iteration_espace)
	end if


Re=0
Nu=0
do k=1,Nb_iteration_espace
	i=k*(Nc+1)
	
	Re(i)=rho_air(i)*V_max*2*Ray/mu_air(i)

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




!**********************************************************************************************************************
!****************************************  PROPRIETE DE L'AIR  ******************************************************
!**********************************************************************************************************************

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
	rho_air(i) =-6*10**(-8.)*T(i)**3 + 7*10**(-5.)*T(i)**2 - 0.0298*T(i) + 8.9153
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
Cp_air(1)= -8*10**(-10.)*T_entree**3 + 9*10**(-7.)*T_entree**2 - 9*10**(-5.)*T_entree + 1.0193

do k=1,Nb_iteration_espace
	i=k*(Nc+1)
	Cp_air(i) = -8*10**(-10.)*T(i)**3 + 9*10**(-7.)*T(i)**2 - 9*10**(-5.)*T(i) + 1.0193
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
	Pr(i) = 5*10**(-13.)*T(i)**4 - 1*10**(-9.)*T(i)**3 + 8*10**(-7)*T(i)**2 - 0.0002*T(i) + 0.7181
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
	mu_air(i) = 8*10**(-15.)*T(i)**3 - 2*10**(-11.)*T(i)**2 + 5*10**(-8.)*T(i) + 2*10**(-5.)
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
Cp_air(1) = 2.955*10**(-13.)*T_entree**4 - 8.39*10**(-10.)*T_entree**3 + 7.652*10**(-7.)*T_entree**2 - &
		& 4.878*10**(-5.)*T_entree + 1.0127

do k=1,Nb_iteration_espace
	i=k*(Nc+1)
	Cp_air(i) = 2.955*10**(-13.)*T(i)**4 - 8.39*10**(-10.)*T(i)**3 + 7.652*10**(-7.)*T(i)**2 - &
		& 4.878*10**(-5.)*T(i) + 1.0127
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
	rho_air(i) =-1.601*10**(-14.)*T(i)**5 + 5.069*10**(-11.)*T(i)**4 - 6.391*10**(-8.)*T(i)**3 + &
		& 4.227*10**(-5.)*T(i)**2 - 0.017*T(i) + 5.0841
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
	Pr(i) = 2.468*10**(-13.)*T(i)**4 - 6.865*10**(-10.)*T(i)**3 + 6.447*10**(-7.)*T(i)**2 - &
		& 0.0002*T(i) + 0.7144
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
	mu_air(i) = 7.823*10**(-15.)*T(i)**3 - 2.193*10**(-11.)*T(i)**2 + 4.745*10**(-8.)*T(i) + &
		& 1.736*10**(-5.)
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
Cp_air(1) = -7.338*10**(-16.)*T_entree**5 + 2.469*10**(-12.)*T_entree**4 - 3.304*10**(-9.)*T_entree**3 + &
		& 2.119*10**(-6.)*T_entree**2 - 0.0004*T_entree + 1.0637

do k=1,Nb_iteration_espace
	i=k*(Nc+1)
	Cp_air(i) = -7.338*10**(-16.)*T(i)**5 + 2.469*10**(-12.)*T(i)**4 - 3.304*10**(-9.)*T(i)**3 + &
		& 2.119*10**(-6.)*T(i)**2 - 0.0004*T(i) + 1.0637
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
	rho_air(i) =-1.287*10**(-13.)*T(i)**5 + 4.024*10**(-10.)*T(i)**4 - 4.972*10**(-7.)*T(i)**3 + &
		& 0.0003*T(i)**2 - 0.1223*T(i) + 34.686
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
	Pr(i) =  - 1.913*10**(-15)*T(i)**5 + 3.154*10**(-12.)*T(i)**4 - 62.914*10**(-9.)*T(i)**3 +&
		&1.566*10**(-6.)*T(i)**2 - 0.0004*T(i) + 0.74
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
	mu_air(i) = 7.26*10**(-15.)*T(i)**3 - 2.07*10**(-11.)*T(i)**2 + 4.649*10**(-8.)*T(i) + &
		& 1.776*10**(-5.)
end do
end subroutine




!**********************************************************************************************************************
!****************************************  LECTURE DES VARIABLES  **************************************************
!**********************************************************************************************************************
subroutine variable(Ray, L, L_entre_C, Nc, Nb_iteration_espace, dt, t_max, Pression, T_entree, &
	&T_initial_C, largeur, debit_total, augmentation_hauteur, T_arret)
implicit none

double precision,intent(out)::Ray, L, L_entre_c, dt, t_max
double precision,intent(out):: T_entree, T_initial_C, largeur, debit_total, T_arret
integer,intent(out)::Nc, Nb_iteration_espace, Pression, augmentation_hauteur

open(unit=11,FILE='variable.txt',status='old')

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
read(11,*)augmentation_hauteur
read(11,*)
read(11,*)
read(11,*)
read(11,*)Nc
read(11,*)
read(11,*)Nb_iteration_espace
read(11,*)
read(11,*)
read(11,*)
read(11,*)dt
read(11,*)
read(11,*)t_max
read(11,*)
read(11,*)
read(11,*)
read(11,*)Pression
read(11,*)
read(11,*)T_entree
read(11,*)
read(11,*)T_initial_C
read(11,*)
read(11,*)debit_total
read(11,*)
read(11,*)T_arret

close(unit=11)
end subroutine




!***********************************************************************************************
!*****************************************  FONCTION DE RESOLUTION TYPE GAUSS**********
!***********************************************************************************************
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


subroutine ecriture(T_1,Nb_iteration_espace,Nc)
implicit none
double precision, dimension(:), intent(in) :: T_1
integer,intent(in)::Nb_iteration_espace,Nc
integer::pas


print*,'Augmentation du nombre de cofalit sur la hauteur:'
print*,Nb_iteration_espace,'cofalits'
print*,
print*,'Repartition de la tempŽrature sur le dernier element'
print*,(T_1(pas)	,pas=((Nc+1)*(Nb_iteration_espace-1)+1),(Nc+1)*Nb_iteration_espace)
print*,
print*,'***************************************************************'


Write(12,*)Nb_iteration_espace, (T_1(pas)	,pas=((Nc+1)*(Nb_iteration_espace-1)+1),&
				&(Nc+1)*Nb_iteration_espace)
write(12,*)

end subroutine 


end program 