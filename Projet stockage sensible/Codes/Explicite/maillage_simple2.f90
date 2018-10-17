program test
implicit none

double precision,allocatable,dimension(:,:)::T_1,T_2
double precision::T_stock, Ray, L_entre_C, T_entree, L, T_initial_C
integer::w,i
integer::Nc, Nb_iteration_espace, Nb_iteration_temps, Pression, Nb_tube_largeur
double precision::t_max, dt, t, temp, temp_ecriture
double precision::debit, Cp_cof, rho_cof, beta, largeur, cond, debit_total



open(UNIT=10,FILE='resultat.txt',STATUS='UNKNOWN',ACTION='WRITE')



!**********************************************************************************************************************
!**************************************** LECTURES DES VARIABLES *************************************************
!**********************************************************************************************************************
call variable(Ray, L, L_entre_C, Nc, Nb_iteration_espace, dt, t_max, temp_ecriture, Pression, T_entree, &
	&T_initial_C, largeur, debit_total)
!*************grosse dépendence au pas de temp pour la convergence dut à un faible rho_air*************
!*************plus le débit est faible mieu sa converge**************



!**********************************************************************************************************************
!**************************************** CONSTANTES **************************************************************
!**********************************************************************************************************************
Nb_iteration_temps=t_max/dt
temp=0
t=0


Cp_cof=900
rho_cof=3120
cond=2

beta=L_entre_C+2*Ray
Nb_tube_largeur=nint(largeur/beta)

debit=debit_total/(Nb_tube_largeur-1)

!**********************************************************************************************************************
!**************************************** RESOLUTION **************************************************************
!**********************************************************************************************************************
allocate( T_1(Nc+1, Nb_iteration_espace) )
allocate( T_2(Nc+1, Nb_iteration_espace) )

!********************* Condition initial *********************
T_1=T_initial_C
T_1(Nc+1,:)=T_entree

!********************* Boucle en temps *********************	
do i=1,Nb_iteration_temps
	T_stock=T_entree
	!********************* Boucle en espace *********************	
	do w=1,Nb_iteration_espace
		call element(T_1, T_2, T_stock, w, Nc, dt, Pression, Ray, L, L_entre_C, debit, Cp_cof, rho_cof, beta, largeur, cond, T_entree)
	end do
	T_1=T_2

	!***************************** ECRITURE DES RESULTATS *******************************
	t=t+dt					! Critere d'arret de la boucle

	temp=temp+dt

	if(temp>=temp_ecriture) then
		WRITE(10,*),'t=',t,'secondes'
		WRITE(10,*),T_1(:,Nb_iteration_espace)
		WRITE(10,*),
		WRITE(10,*),
		WRITE(10,*),
		temp=0
	ENDIF
end do
CLOSE(10)
















contains



subroutine element(T_1, T_2, T_stock, w, Nc, dt, Pression, Ray, L, L_entre_C, debit, Cp_cof, rho_cof, beta, largeur, cond,T_entree)
implicit none

integer,intent(in)::w, Nc, Pression
double precision,dimension(:,:),intent(inout)::T_1, T_2
double precision,intent(inout)::T_stock
double precision,intent(in)::dt, Ray, L, L_entre_C, debit, Cp_cof, rho_cof, beta, largeur, cond,T_entree

double precision::A_V, A_S, V_maille, masse_C, masse_A
double precision,dimension(:),allocatable::C_S, e 

double precision::T_air
double precision::phi_cond_n, phi_cond_s, phi_conv_cof, phi_conv_air, phi_adv
double precision::rho_air, mu_air, Cp_air, Pr

integer::i,j
double precision,parameter:: Pi = 3.1415

double precision,dimension(:),allocatable::T
double precision,dimension(:),allocatable::T_n

double precision:: h


!******* Allocation tableaux


allocate(T_n(Nc+1))
allocate(T(Nc+1))


!*********************Lecture des températures au temps précédent*********************	
do i=1,Nc+1
	T(i)=T_1(i,w)
end do

!*********************En entrée de l'échangeur la température de l'air est de 500K *********************	
if (w==1) then
	T_stock=T_entree
end if


!**********************************************************************************************************************
!****************************************CALCULE DES VOLUMES ET SURFACES **************************************
!**********************************************************************************************************************
call geometrie_simple(e, A_V,A_S,C_S,V_maille, Nc, Ray, L, L_entre_C)




!**********************************************************************************************************************
!****************************************CALCULE DE PARAMETRES PHYSIQUES **************************************
!**********************************************************************************************************************
T_air=T(Nc+1)
if (Pression==70) then
	call rho_air70(T_air,rho_air)
	call Cp_air70(T_air,Cp_air)
	call Pr70(T_air,Pr)
	call mu_air70(T_air,mu_air)
else if (Pression==27) then
	call rho_air27(T_air,rho_air)
	call Cp_air27(T_air,Cp_air)
	call Pr27(T_air,Pr)
	call mu_air27(T_air,mu_air)
else
	call rho_air4(T_air,rho_air)
	call Cp_air4(T_air,Cp_air)
	call Pr4(T_air,Pr)
	call mu_air4(T_air,mu_air)
end if
Cp_air=Cp_air*1000


Call convection(h,Ray,beta,debit,largeur,L,T_air,T(Nc),Pression,rho_air,Pr,mu_air)
Call Calcule_masse(Nc,rho_air,rho_cof,V_maille,A_V,masse_C,masse_A)



!**********************************************************************************************************************
!****************************************CALCULE DES TEMPERATURES ***********************************************
!**********************************************************************************************************************

!********************* Cas 1 ( i = 1) *********************
phi_adv = 0
phi_conv_air = 0
phi_cond_n = cond*C_S(1)/e(1) * ( T(2)-T(1) )
phi_cond_s = 0
phi_conv_cof = 0

T_n(1) = dt/masse_C/Cp_cof * ( phi_cond_n + phi_cond_s + phi_conv_cof + phi_conv_air + phi_adv ) + T(1)


!********************* Cas 2 ( i = 2 , Nc-1 )  *********************
do i=2, Nc-1
	phi_adv = 0
	phi_conv_air = 0
	phi_cond_n = cond*C_S(i)/e(i) * ( T(i+1)-T(i) )
	phi_cond_s = cond*C_S(i-1)/e(i-1) * ( T(i-1)-T(i) )
	phi_conv_cof = 0

	T_n(i) = dt/masse_C/Cp_cof * ( phi_cond_n + phi_cond_s + phi_conv_cof + phi_conv_air + phi_adv ) + T(i)
end do


!********************* Cas 3 ( i = Nc) *********************
phi_adv = 0
phi_conv_air = 0
phi_cond_n = 0
phi_cond_s = cond*C_S(Nc-1)/e(Nc-1) * ( T(Nc-1)-T(Nc) )
phi_conv_cof = h*C_S(Nc) * ( T(Nc+1)-T(Nc) )

T_n(Nc) = dt/masse_C/Cp_cof * ( phi_cond_n + phi_cond_s + phi_conv_cof + phi_conv_air + phi_adv ) + T(Nc)


!********************* Cas 4 ( i = Nc+1 )  *********************
phi_adv = debit*Cp_air * ( T_stock-T(Nc+1) )
phi_conv_air = h*C_S(Nc) * ( T(Nc)-T(Nc+1) )
phi_cond_n = 0
phi_cond_s = 0
phi_conv_cof = 0

T_n(Nc+1) = dt/masse_A/Cp_air * ( phi_cond_n + phi_cond_s + phi_conv_cof + phi_conv_air + phi_adv ) + T(Nc+1)


!********************* Stockage de la température de l'air *********************	
T_stock=T_n(Nc+1)		


!********************* Ecriture des résultats dans la matrice générale*********************	
do i=1,Nc+1
	T_2(i,w)=T_n(i)
end do

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

!********************* Calcule des surfaces des maille de Cofalite************************
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
subroutine Calcule_masse(Nc,v_air,v_cof,V_maille,A_V,masse_C,masse_A)
implicit none

integer,intent(in)::Nc
double precision,intent(out)::masse_C, masse_A
double precision,intent(in)::v_air, v_cof, V_maille, A_V
integer::i

masse_C=V_maille*v_cof
masse_A=A_V*v_air
end subroutine




!**********************************************************************************************************************
!****************************************CALCULE DE H ***************************************************************
!**********************************************************************************************************************
subroutine convection(h,Ray,beta,debit,largeur,L,T_air,T_cofalit,Pression,rho_air,Pr,mu_air)
implicit none

double precision,intent(out)::h
double precision,intent(in)::Ray,beta,debit,largeur,L,T_air,T_cofalit,rho_air,Pr,mu_air
double precision::S_l,S_d,S_t,Re,V_max,Nu,k,Pr_s,T_m
integer::Pression

k=0.0338
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


T_m=(T_air+T_cofalit)/2

if (Pression==70) then
	call Pr70(T_cofalit,Pr_s)
else if (Pression==27) then
	call Pr27(T_cofalit,Pr_s)
else
	call Pr4(T_cofalit,Pr_s)
end if


Re=rho_air*V_max*2*Ray/mu_air


If (Re<500) then
	Nu=1.04*Re**0.4*Pr**0.36*(Pr/Pr_s)**0.25
elseif (Re<1000) then
	Nu=0.71*Re**0.5*Pr**0.36*(Pr/Pr_s)**0.25
elseif (Re<200000) then
	Nu=0.35*(S_t/S_l)**0.2*Re**0.6*Pr**0.36*(Pr/Pr_s)**0.25
else
	Nu=0.031*(S_t/S_l)**0.2*Re**0.8*Pr**0.36*(Pr/Pr_s)**0.25
endif


h=Nu*k/(2*Ray)	
end subroutine




!**********************************************************************************************************************
!****************************************  PROPRIETE DE L'AIR  ******************************************************
!**********************************************************************************************************************

! *************************Proprietes de l'air Ã  70 bar*************************
subroutine rho_air70(T,rho_air)
implicit none
double precision, intent(in) :: T
double precision, intent(out) :: rho_air

rho_air =-6*10**(-8.)*T**3 + 7*10**(-5.)*T**2 - 0.0298*T + 8.9153
end subroutine


subroutine Cp_air70(T,Cp_air)
implicit none
double precision, intent(in) :: T
double precision, intent(out) :: Cp_air

Cp_air = -8*10**(-10.)*T**3 + 9*10**(-7.)*T**2 - 9*10**(-5.)*T + 1.0193
end subroutine


subroutine Pr70(T,Pr)
implicit none
double precision, intent(in) :: T
double precision, intent(out) :: Pr

Pr = 5*10**(-13.)*T**4 - 1*10**(-9.)*T**3 + 8*10**(-7)*T**2 - 0.0002*T + 0.7181
end subroutine


subroutine mu_air70(T,mu_air)
implicit none
double precision, intent(in) :: T
double precision, intent(out) :: mu_air

mu_air = 8*10**(-15.)*T**3 - 2*10**(-11.)*T**2 + 5*10**(-8.)*T + 2*10**(-5.)
end subroutine



!*************************Proprietes de l'air Ã  4 bar*************************
subroutine Cp_air4(T,Cp_air)
implicit none
double precision, intent(in) :: T
double precision, intent(out) :: Cp_air

Cp_air = 2.955*10**(-13.)*T**4 - 8.39*10**(-10.)*T**3 + 7.652*10**(-7.)*T**2 - &
 4.878*10**(-5.)*T + 1.0127
end subroutine


subroutine rho_air4 (T,rho_air)
implicit none
double precision, intent(in) :: T
double precision, intent(out) :: rho_air

rho_air =-1.601*10**(-14.)*T**5 + 5.069*10**(-11.)*T**4 - 6.391*10**(-8.)*T**3 + &
4.227*10**(-5.)*T**2 - 0.017*T + 5.0841
end subroutine


subroutine Pr4(T,Pr)
implicit none
double precision, intent(in) :: T
double precision, intent(out) :: Pr

Pr = 2.468*10**(-13.)*T**4 - 6.865*10**(-10.)*T**3 + 6.447*10**(-7.)*T**2 - &
0.0002*T + 0.7144
end subroutine


subroutine mu_air4(T,mu_air)
implicit none
double precision, intent(in) :: T
double precision, intent(out) :: mu_air

mu_air = 7.823*10**(-15.)*T**3 - 2.193*10**(-11.)*T**2 + 4.745*10**(-8.)*T + &
1.736*10**(-5.)
end subroutine



!*************************Proprietes de l'air Ã  27 bar*************************
subroutine Cp_air27(T,Cp_air)
implicit none
double precision, intent(in) :: T
double precision, intent(out) :: Cp_air

Cp_air = -7.338*10**(-16.)*T**5 + 2.469*10**(-12.)*T**4 - 3.304*10**(-9.)*T**3 + &
2.119*10**(-6.)*T**2 - 0.0004*T + 1.0637
end subroutine


subroutine rho_air27 (T,rho_air)
implicit none
double precision, intent(in) :: T
double precision, intent(out) :: rho_air

rho_air =-1.287*10**(-13.)*T**5 + 4.024*10**(-10.)*T**4 - 4.972*10**(-7.)*T**3 + &
0.0003*T**2 - 0.1223*T + 34.686
end subroutine


subroutine Pr27(T,Pr)
implicit none
double precision, intent(in) :: T
double precision, intent(out) :: Pr

Pr =  - 1.913*10**(-15)*T**5 + 3.154*10**(-12.)*T**4 - 62.914*10**(-9.)*T**3 +&
 1.566*10**(-6.)*T**2 - 0.0004*T + 0.74
end subroutine


subroutine mu_air27(T,mu_air)
implicit none
double precision, intent(in) :: T
double precision, intent(out) :: mu_air

mu_air = 7.26*10**(-15.)*T**3 - 2.07*10**(-11.)*T**2 + 4.649*10**(-8.)*T + &
1.776*10**(-5.)
end subroutine




!**********************************************************************************************************************
!****************************************  LECTURE DES VARIABLES  **************************************************
!**********************************************************************************************************************
subroutine variable(Ray, L, L_entre_c, Nc, Nb_iteration_espace, dt, t_max, temp_ecriture, Pression,&
	&T_entree, T_initial_C, largeur, debit_total )
implicit none

double precision,intent(out)::Ray, L, L_entre_c, dt, t_max, temp_ecriture
double precision,intent(out):: T_entree, T_initial_C, largeur, debit_total
integer,intent(out)::Nc, Nb_iteration_espace, Pression

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
read(11,*)temp_ecriture
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

close(unit=11)
end subroutine







end program 