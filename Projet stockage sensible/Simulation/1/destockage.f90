program test
implicit none

double precision,allocatable,dimension(:)::T_1,T_ini
double precision::Ray, L_entre_C, T_entree, L, T_initial_C
integer::i,k,boucle_temps
integer::Nc, Nb_iteration_espace, Nb_iteration_temps, Pression, Nb_tube_largeur
double precision::t_max, dt
double precision::debit, Cp_cof, rho_cof, beta, largeur, cond, debit_total,t
double precision::A_V, A_S, V_maille
double precision,allocatable,dimension(:)::C_S, e


!**********************************************************************************************************************
!**************************************** LECTURES DES VARIABLES *************************************************
!**********************************************************************************************************************
call variable(Ray, L, L_entre_C, Nc, Nb_iteration_espace, dt, t_max, Pression, T_entree, &
	&T_initial_C, largeur, debit_total, T_1)


open(UNIT=10,FILE='temperature_destockage.txt',STATUS='UNKNOWN',ACTION='WRITE')

!**********************************************************************************************************************
!**************************************** CONSTANTES **************************************************************
!**********************************************************************************************************************
Nb_iteration_temps=t_max/dt
t=0


Cp_cof=900
rho_cof=3120
cond=2

beta=L_entre_C+2*Ray
Nb_tube_largeur=nint(largeur/beta)

debit=debit_total/(Nb_tube_largeur-1)

!sauvegarde des temperatures initiales pour le calcul de l'energie
allocate(T_ini((Nc+1)*nb_iteration_espace))
T_ini=T_1

!**********************************************************************************************************************
!****************************************CALCULE DES VOLUMES ET SURFACES **************************************
!**********************************************************************************************************************
call Geometrie_simple(e, A_V, A_S, C_S, V_maille, Nc, Ray, L, L_entre_C)


!**********************************************************************************************************************
!**************************************** RESOLUTION **************************************************************
!**********************************************************************************************************************

!********************* Condition initial *********************

!print*,T_1


!********************* Boucle en temps *********************	
do boucle_temps=1,Nb_iteration_temps
	call Element(T_1, t, Nb_iteration_espace, Nc, dt, Pression, Ray, L, L_entre_C, debit, Cp_cof, rho_cof, beta,&
				&largeur, cond,T_entree,e, A_V, A_S, C_S, V_maille)			
				!print*,T_1
				!print*,boucle_temps
				!print*,'***************************************************************'
end do


call ecriture(T_1,T_ini,V_maille)


CLOSE(10)





contains



subroutine Element(T_1, t, Nb_iteration_espace, Nc, dt, Pression, Ray, L, L_entre_C, debit, Cp_cof, rho_cof, beta,&
				&largeur, cond,T_entree,e, A_V, A_S, C_S, V_maille)	
implicit none

integer,intent(in):: Nc, Pression, Nb_iteration_espace

double precision,dimension(:),intent(inout)::T_1
double precision, allocatable, dimension(:)::T_2
double precision,intent(in)::dt, Ray, L, L_entre_C, debit, Cp_cof, rho_cof, beta, largeur, cond,T_entree
double precision,intent(inout)::t

double precision,intent(in)::A_V, A_S, V_maille
double precision::masse_C
double precision,dimension(:),intent(in)::C_S, e 
double precision,dimension(:),allocatable::masse_A 

double precision,dimension(:),allocatable::rho_air, mu_air, Cp_air, Pr, h

integer::i,j,k
double precision,parameter:: Pi = 3.1415

double precision,dimension((Nc+1)*Nb_iteration_espace,(Nc+1)*Nb_iteration_espace)::A
double precision,dimension((Nc+1)*Nb_iteration_espace)::B



!**********************************************************************************************************************
!****************************************CALCULE DE PARAMETRES PHYSIQUES **************************************
!**********************************************************************************************************************
if (Pression==70) then
	call rho_air70(T_1,rho_air, Nb_iteration_espace)
	call Cp_air70(T_1,Cp_air, Nb_iteration_espace, T_entree)
else if (Pression==27) then
	call rho_air27(T_1,rho_air, Nb_iteration_espace)
	call Cp_air27(T_1,Cp_air, Nb_iteration_espace, T_entree)
else if (Pression==50) then
	call rho_air50(T_1,rho_air, Nb_iteration_espace)
	call Cp_air50(T_1,Cp_air, Nb_iteration_espace, T_entree)
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
call gauss1(A,B,T_2)

T_1(:)=T_2(:)

	!***************************** ECRITURE DES RESULTATS *******************************
	t=t+dt					! Critere d'arret de la boucle

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
	else if (Pression==50) then
		call Pr50(T_1,Pr_s,Nb_iteration_espace)
		call Pr50(T_m,Pr,Nb_iteration_espace)
		call mu_air50(T_1,mu_air,Nb_iteration_espace)
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
	Pr(i) =  - 1.913*10**(-15)*T(i)**5 + 3.154*10**(-12.)*T(i)**4 - 2.914*10**(-9.)*T(i)**3 +&
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
Cp_air(1) = -2*10**(-15.)*T_entree**5 + 5*10**(-12.)*T_entree**4 - 6*10**(-9.)*T_entree**3 + &
		& 4*10**(-6.)*T_entree**2 - 0.0008*T_entree + 1.1155

do k=1,Nb_iteration_espace
	i=k*(Nc+1)
	Cp_air(i) = -2*10**(-15.)*T(i)**5 + 5*10**(-12.)*T(i)**4 - 6*10**(-9.)*T(i)**3 + &
		& 4*10**(-6.)*T(i)**2 - 0.0008*T(i) + 1.1155
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
	rho_air(i) =-2.729*10**(-13.)*T(i)**5 + 8.45*10**(-10.)*T(i)**4 - 1.029*10**(-6.)*T(i)**3 + &
		& 0.0006*T(i)**2 - 0.2379*T(i) + 64.697
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
	Pr(i) =  - 4.742*10**(-15)*T(i)**5 + 7.039*10**(-12.)*T(i)**4 - 5.624*10**(-9.)*T(i)**3 +&
		&2.593*10**(-6.)*T(i)**2 - 0.0006*T(i) + 0.7636
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
	mu_air(i) = 6.338*10**(-15.)*T(i)**3 - 1.879*10**(-11.)*T(i)**2 + 4.509*10**(-8.)*T(i) + &
		& 1.825*10**(-5.)
end do
end subroutine





!**********************************************************************************************************************
!****************************************  LECTURE DES VARIABLES  **************************************************
!**********************************************************************************************************************
subroutine variable(Ray, L, L_entre_c, Nc, Nb_iteration_espace, dt, t_max, Pression,&
	&T_entree, T_initial_C, largeur, debit_total, T_1 )
implicit none

double precision,intent(out)::Ray, L, L_entre_c, dt, t_max
double precision,intent(out):: T_entree, T_initial_C, largeur, debit_total
integer,intent(out)::Nc, Nb_iteration_espace, Pression

double precision,dimension(:),allocatable,intent(out)::T_1
integer::inverse, increment

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

close(unit=11)

open(unit=14,FILE='tube.txt',status='old')
read(14,*)Nb_iteration_espace
close(unit=14)



open(unit=12,FILE='donnees_destockage.txt',status='old')

read(12,*)
read(12,*)
read(12,*)
read(12,*)
read(12,*)dt
read(12,*)
read(12,*)t_max
read(12,*)
read(12,*)
read(12,*)
read(12,*)Pression
read(12,*)
read(12,*)T_entree
read(12,*)
read(12,*)debit_total

close(unit=12)

allocate(T_1((Nc+1)*nb_iteration_espace))

open(unit=13,FILE='temperature_pause.txt',status='old')

read(13,*)

do inverse=(Nc+1)*(nb_iteration_espace-1),0,-(Nc+1)
	do increment=1+inverse,(Nc+1)+inverse
	read(13,*)T_1(increment)	
	enddo
enddo


close(unit=13)

end subroutine






!***********************************************************************************************
!*****************************************  FONCTION DE RESOLUTION TYPE GAUSS**********
!***********************************************************************************************
subroutine gauss1(A,B,t)
implicit none

	double precision, dimension(:),intent(in)::B
	double precision, dimension(:,:),intent(in):: A
	double precision, allocatable,dimension(:),intent(out)::t
	
	double precision, allocatable,dimension(:,:)::xA
	double precision, allocatable,dimension(:)::xB
		
	integer::nn,j,k,l
	double precision::di,somme,dp
	
	nn=size(A,1)
	
	allocate(t(nn))
	allocate(xA(nn,nn))
	allocate(xB(nn))
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
	t(nn)=xB(nn)/xA(nn,nn)
	do i=1,nn-1
	   Somme=0
	   do j=nn-i+1,nn
	      Somme=Somme+xA(nn-i,j)*t(j)
	      t(nn-i)=xB(nn-i)-Somme
	   end do
	end do
     
end subroutine



!****************************************************************************************************
!*****************************************  ECRITURE ***********************************************
!****************************************************************************************************
!****************************************************************************************************
subroutine ecriture(T_1,T_ini,V_maille)
implicit none
double precision, dimension(:), intent(in) :: T_1
double precision, dimension(:), intent(in) :: T_ini
double precision,intent(in)::V_maille
integer::inverse, increment
double precision::deltat_T,Energie



Write(10,*),'REPARTITION DE LA TEMPERATURE AU SEIN DE L''ECHANGEUR A LA FIN DU DESTOCKAGE'

do inverse=(Nc+1)*(nb_iteration_espace-1),0,-(Nc+1)
	do increment=1+inverse,(Nc+1)+inverse
	write(10,*)T_1(increment)	
	enddo
enddo


! on cherche le refroidissement du cofalit afin de calculer l'Žnergie restituer.
deltat_T=0


Do i=0,(Nb_iteration_espace-1)
	do k=i*(Nc+1)+1,i*(Nc+1)+Nc
Deltat_T=Deltat_T+(T_1(k)-T_ini(k))
	enddo
enddo

Energie=Deltat_T*Cp_cof*rho_cof*V_maille

write(10,*)
write(10,*),'Energie perdu par le cofalit (J) (soit energie destockŽe)'
write(10,*)Energie
write(10,*)

print*,'Energie destockŽe:',Energie,'J'
! ATTENTION: energie nŽgative, car energie perdu par le systeme
end subroutine 


end program 