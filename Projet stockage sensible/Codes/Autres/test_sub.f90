program annexe
implicit none


!$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ DECLARATION $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

double precision::theta,gamma,Ray,L
double precision::delta_r
double precision::cond,Cp,Cp_air,v_air,v_cof,m
double precision::dt,t_max
double precision::phi_cond_d,phi_cond_b,phi_conv_cof,phi_cond_h,phi_cond_g,phi_conv_air,phi_adv
double precision::beta,alpha

integer::i,j,Nt,Nr
integer::aide_gamma,nb_iteration,t0

double precision,parameter:: Pi = 3.1415



double precision,dimension(:),allocatable::C_Sn
double precision,dimension(:),allocatable::C_Ss
double precision,dimension(:),allocatable::C_Se
double precision,dimension(:),allocatable::C_So
double precision,dimension(:),allocatable::C_V

double precision,dimension(:),allocatable::A_Sn
double precision,dimension(:),allocatable::A_Ss
double precision,dimension(:),allocatable::A_Se
double precision,dimension(:),allocatable::A_So
double precision,dimension(:),allocatable::A_V

double precision,dimension(:),allocatable::e

double precision,dimension(:),allocatable::T
double precision,dimension(:),allocatable::T_n
double precision,dimension(:),allocatable::T_stock		!permet de stocker la tempÈrature de l'air des autres subroutines

double precision,dimension(:),allocatable::masse

Ray=0.05
L=1
Cp_air=1130
Cp=900
beta=0.1
theta=9999
cond=10
Nt=4
Nr=5
v_air=1000 !masse volumique
v_cof=1000
m=0.001
nb_iteration=10800*2+2
! L=
! largeur=
! d√©bit= 
delta_r=Ray/Nr

!******* Allocation tableaux


allocate(T_n(Nt*(Nr+1)))
allocate(T(Nt*(Nr+1)))
allocate(T_stock(nb_iteration*2))

!**********************************************************************************************************************
!****************************************CALCULE DES VOLUMES ET SURFACES **************************************
!**********************************************************************************************************************
call Geometrie(e,C_So,C_Se,C_Ss,C_Sn,C_V,A_Se,A_So,A_Ss,A_Sn,A_V,Nt,Nr,delta_r,Ray,L,beta)


contains

!**********************************************************************************************************************
!****************************************CALCULE DES VOLUMES ET SURFACES **************************************
!**********************************************************************************************************************
subroutine Geometrie(e,C_So,C_Se,C_Ss,C_Sn,C_V,A_Se,A_So,A_Ss,A_Sn,A_V,Nt,Nr,delta_r,Ray,L,beta)
implicit none

double precision,dimension(:),allocatable,intent(out)::e
double precision,dimension(:),allocatable,intent(out)::C_So,C_Se,C_Ss,C_Sn,C_V
double precision,dimension(:),allocatable,intent(out)::A_Se,A_So,A_Ss,A_Sn,A_V
double precision,intent(in)::Ray,delta_r,L,beta
integer,intent(in)::Nt,Nr


double precision::aide_gamma,gamma,theta
integer::i,beta_i,alpha_i

!Allocation
allocate(e(Nt*Nr))
allocate(C_So(Nt*Nr))
allocate(C_Se(Nt*Nr))
allocate(C_Ss(Nt*Nr))
allocate(C_Sn(Nt*Nr))
allocate(C_V(Nt*Nr))
allocate(A_Se(Nt*(Nr+1)))
allocate(A_Ss(Nt*(Nr+1)))
allocate(A_Sn(Nt*(Nr+1)))
allocate(A_V(Nt*(Nr+1)))
allocate(A_So(Nt*(Nr+1)))


! Calcul preliminaire



theta=Pi/Nt

!********************* Surfaces et volumes des mailles de Cofalit *********************
do i=1,Nt*Nr	


	
	beta_i=i-(i/Nt)*Nt
	alpha_i=((i-beta_i)/Nt)
	
	if(beta_i==0) then
		beta_i=Nt
		alpha_i=alpha_i-1
	endif
	
	
	
	!print*,alpha_i,beta_i
	!print*,'-------------------------'

	C_So(i)=theta*L*delta_r*alpha_i
	C_Se(i)=theta*L*delta_r*(alpha_i+1)
	C_Ss(i)=L*delta_r
	C_Sn(i)=C_Ss(i)
	
	
	
	C_V(i)=L*theta*(delta_r)**2*(2*alpha_i+1)/2
	
	e(i)=(C_Ss(i)+C_Sn(i))/(2*L)
	
	!print*,C_V(i)
	!print*,'-----'
	
enddo



!********************* Surfaces et volumes des mailles d'air *********************
	! initialisation des tableaux
	
	A_Se=0
	A_So=0
	A_Ss=0
	A_Sn=0
	A_V=0

do i=(Nr*Nt+1),(Nt*(Nr+1))	
	
	aide_gamma=i-Nr*Nt
	
	A_So(i)=C_Se(i-Nt)
	A_Se(i)=Ray*abs(sin(aide_gamma*theta)-sin((aide_gamma-1)*theta))
	
	A_Sn(i) = (beta-sin(aide_gamma*theta)*Ray)*L
	A_Ss(i) = (beta-sin((aide_gamma-1)*theta)*Ray)*L
	
	A_V(i) = ((A_Sn(i)+A_Ss(i))/2)*A_Se(i)/L

		
	
	print*,A_V(i),A_Ss(i),A_Sn(i),A_Se(i),A_So(i)
	print*,'-------------------------------------------------------'
enddo


end subroutine


!************************************************************************************************
!*************************  CALCUL DE H selon les correlation  **********************************
!************************************************************************************************


subroutine convection(h,Ray,beta,debit,largeur,L,T_air,T_cofalit,Pression,rho_air,Pr,mu_air)
implicit none


double precision,intent(out)::h
double precision,intent(in)::Ray,beta,debit,largeur,L,T_air,T_cofalit,Pression,rho_air,Pr,mu_air
double precision::S_l,S_d,S_t,Re,V_max,Nu,k

k=0.0338
S_l=2*Ray
S_t=beta
S_d=sqrt(S_l**2+(S_t/2)**2)

If(S_d<((S_t+2*Ray)/2)) then
	V_max=(S_t/(2*(S_d-2*Ray)))*debit/(largeur*L)
		! la vitesse max du fluide se situe sur la diagonale entre deux cofalites de ligne différentes 
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



end program 