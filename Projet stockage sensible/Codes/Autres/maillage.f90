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
double precision,dimension(:),allocatable::T_stock		!permet de stocker la température de l'air des autres subroutines

double precision,dimension(:),allocatable::masse

Ray=0.05
L=0.1
Cp_air=1130
Cp=900
beta=0.1
theta=9999
cond=10
Nt=3
Nr=3
v_air=1000 !masse volumique
v_cof=1000
m=0.001
nb_iteration=10800*2+2
! L=
! largeur=
! dÃ©bit= 
delta_r=Ray/Nr

!******* Allocation tableaux


allocate(T_n(Nt*(Nr+1)))
allocate(T(Nt*(Nr+1)))
allocate(T_stock(nb_iteration*2))

!**********************************************************************************************************************
!****************************************CALCULE DES VOLUMES ET SURFACES **************************************
!**********************************************************************************************************************
call Geometrie(e,C_So,C_Se,C_Ss,C_Sn,C_V,A_Se,A_So,A_Ss,A_Sn,A_V,Nt,Nr,delta_r,Ray,L,beta)

A_V(Nt*(Nr+1))=A_V(Nt*Nr+1)
call Calcule_masse(Nt,Nr,v_air,v_cof,C_V,A_V,masse)
!print*,C_V(Nt*(Nr+1)),A_V(Nt*(Nr+1))

!**********************************************************************************************************************
!****************************************CALCULE DE PARAMETRES PHYSIQUES **************************************
!**********************************************************************************************************************
dt=0.5
t_max=10800
t0=0

!Condition initial
T=298
do i=Nt*Nr+1,Nt*(Nr+1)
T(i)=500
end do

T_stock=500

do while(t0<=t_max)
t0=t0+1



!**********************************************************************************************************************
!****************************************CALCULE DES TEMPERATURES ***********************************************
!**********************************************************************************************************************


!********************* Cas 1 ( i = Nt*Nr) *********************
phi_cond_h = 0
phi_cond_g = 0
phi_adv = 0
phi_conv_air = 0
phi_cond_d = cond*C_Se(Nt*Nr)/e(Nt*Nr)*(T(Nt*Nr-1)-T(Nt*Nr))
phi_cond_b = cond*C_Ss(Nt*Nr)/delta_r*(T(Nt*Nr-Nt)-T(Nt*Nr))
phi_conv_cof = h(theta)*C_Sn(Nt*Nr)*(T(Nt*Nr+Nt)-T(Nt*Nr))

T_n(Nt*Nr)=dt/masse(Nt*Nr)/Cp*(phi_cond_d+phi_cond_b+phi_conv_cof+phi_cond_h+phi_cond_g+phi_conv_air+phi_adv)+T(Nt*Nr)

!********************* Cas 2 ( i = 2Nt,Nt*(Nr-1)) *********************
do i=2*Nt,Nt*(Nr-1),Nt
	phi_cond_d = cond*C_Se(i)/e(i-1)*(T(i-1)-T(i))
	phi_cond_b = cond*C_Ss(i)/delta_r*(T(i-Nt)-T(i))
	phi_conv_cof = 0
	phi_cond_h = cond*C_Sn(i)/delta_r*(T(i+Nt)-T(i))
	phi_cond_g = 0
	phi_conv_air = 0
	phi_adv = 0

	T_n(i)=dt/masse(i)/Cp*(phi_cond_d+phi_cond_b+phi_conv_cof+phi_cond_h+phi_cond_g+phi_conv_air+phi_adv)+T(i)
end do

!********************* Cas 3 ( i = Nt) *********************
phi_cond_d = cond*C_Se(Nt)/e(Nt-1)*(T(Nt-1)-T(Nt))
phi_cond_b = 0
phi_conv_cof = 0
phi_cond_h = cond*C_Sn(Nt)/delta_r*(T(Nt+Nt)-T(Nt))
phi_cond_g = 0
phi_conv_air = 0
phi_adv = 0

T_n(Nt)=dt/masse(Nt)/Cp*(phi_cond_d+phi_cond_b+phi_conv_cof+phi_cond_h+phi_cond_g+phi_conv_air+phi_adv)+T(Nt)

!********************* Cas 4 ( i = 2, Nt-1)  *********************
do i=2,Nt-1
	phi_cond_d = cond*C_Se(i)/e(i-1)*(T(i-1)-T(i))
	phi_cond_b = 0
	phi_conv_cof = 0
	phi_cond_h = cond*C_Sn(i)/delta_r*(T(i+Nt)-T(i))
	phi_cond_g = cond*C_Ss(i)/e(i)*(T(i+1)-T(i))
	phi_conv_air = 0
	phi_adv = 0

	T_n(i)=dt/masse(i)/Cp*(phi_cond_d+phi_cond_b+phi_conv_cof+phi_cond_h+phi_cond_g+phi_conv_air+phi_adv)+T(i)
end do

!********************* Cas 5 ( i=1 )  *********************
phi_cond_d = 0
phi_cond_b = 0
phi_conv_cof = 0
phi_cond_h = cond*C_Sn(1)/delta_r*(T(1+Nt)-T(1))
phi_cond_g = cond*C_Ss(1)/e(1)*(T(2)-T(1))
phi_conv_air = 0
phi_adv = 0

T_n(1)=dt/masse(1)/Cp*(phi_cond_d+phi_cond_b+phi_conv_cof+phi_cond_h+phi_cond_g+phi_conv_air+phi_adv)+T(1)

!********************* Cas 6 ( i=Nt+1,Nt*(Nr-2)+1,Nt)  *********************
do i=Nt+1,Nt*(Nr-2)+1,Nt
	phi_cond_d = 0
	phi_cond_b = cond*C_Ss(i)/delta_r*(T(i-Nt)-T(i))
	phi_conv_cof = 0
	phi_cond_h = cond*C_Sn(i)/delta_r*(T(i+Nt)-T(i))
	phi_cond_g = cond*C_Ss(i)/e(i)*(T(i+1)-T(i))
	phi_conv_air = 0
	phi_adv = 0

	T_n(i)=dt/masse(i)/Cp*(phi_cond_d+phi_cond_b+phi_conv_cof+phi_cond_h+phi_cond_g+phi_conv_air+phi_adv)+T(i)
end do

!********************* Cas 7 ( i=Nt*(Nr-1)+1 )  *********************
phi_cond_d = 0
phi_cond_b = cond*C_Ss(Nt*(Nr-1)+1)/delta_r*(T(Nt*(Nr-1)+1)-T(Nt*(Nr-1)+1))
phi_conv_cof = h(theta)*C_Sn(Nt*(Nr-1)+1)*(T(Nt*(Nr-1)+1+Nt)-T((Nt*(Nr-1)+1)))
phi_cond_h = 0
phi_cond_g = cond*C_Ss(Nt*(Nr-1)+1)/e(Nt*(Nr-1)+1)*(T((Nt*(Nr-1)+1)+1)-T((Nt*(Nr-1)+1)))
phi_conv_air = 0
phi_adv = 0

T_n(Nt*(Nr-1)+1)=dt/masse(Nt*(Nr-1)+1)/Cp*(phi_cond_d+phi_cond_b+phi_conv_cof+phi_cond_h+phi_cond_g+phi_conv_air+phi_adv)&
	&+T(Nt*(Nr-1)+1)

!********************* Cas 8 (centrale) *********************
Do j=Nt,Nt*(Nr-2),Nt
	Do i=j+2,j+Nt-1
		phi_cond_d = cond*C_Se(i)/e(i-1)*(T(i-1)-T(i))
		phi_cond_b = cond*C_Ss(i)/delta_r*(T(i-Nt)-T(i))
		phi_conv_cof = 0
		phi_cond_h = cond*C_Sn(i)/delta_r*(T(i+Nt)-T(i))
		phi_cond_g = cond*C_Ss(i)/e(i)*(T(i+1)-T(i))
		phi_conv_air = 0
		phi_adv = 0

		T_n(i)=dt/masse(i)/Cp*(phi_cond_d+phi_cond_b+phi_conv_cof+phi_cond_h+phi_cond_g+phi_conv_air+phi_adv)+T(i)
	end do
end do

!********************* Cas 9 (i=Nt*(Nr-1)+2,Nt*Nr-1) *********************
do i=Nt*(Nr-1)+2,Nt*Nr-1
	phi_cond_d = cond*C_Se(i)/e(i-1)*(T(i-1)-T(i))
	phi_cond_b = cond*C_Ss(i)/delta_r*(T(i-Nt)-T(i))
	phi_conv_cof = h(theta)*C_Sn(i)*(T(i+Nt)-T(i))
	phi_cond_h = 0
	phi_cond_g = cond*C_Ss(i)/e(i)*(T(i+1)-T(i))
	phi_conv_air = 0
	phi_adv = 0
		
	T_n(i)=dt/masse(i)/Cp*(phi_cond_d+phi_cond_b+phi_conv_cof+phi_cond_h+phi_cond_g+phi_conv_air+phi_adv)+T(i)
end do

!********************* Cas 10 (i=Nt*Nr+2, Nt*(Nr+1)-1) *********************
do i=Nt*Nr+2,Nt*(Nr+1)-1
	phi_cond_d = 0
	phi_cond_b = 0
	phi_conv_cof = 0
	phi_cond_h = 0
	phi_cond_g = 0
	phi_conv_air = h(theta)*C_Sn(i-Nt)*(T(i-Nt)-T(i))
	phi_adv = m*Cp_air*(T(i+1)-T(i))

	T_n(i)=dt/masse(i)/Cp_air*(phi_cond_d+phi_cond_b+phi_conv_cof+phi_cond_h+phi_cond_g+phi_conv_air+phi_adv)+T(i)
	
	
end do

!********************* Cas 11 (i=Nt*(Nr+1)) *********************
phi_cond_d = 0
phi_cond_b = 0
phi_conv_cof = 0
phi_cond_h = 0
phi_cond_g = 0
phi_conv_air = h(theta)*C_Sn(Nt*(Nr+1)-Nt)*(T(Nt*(Nr+1)-Nt)-T(Nt*(Nr+1)))
phi_adv = m*Cp_air*(T_stock(2*t0-1)-T(Nt*(Nr+1)))

T_n(Nt*(Nr+1))=dt/masse(Nt*(Nr+1))/Cp*(phi_cond_d+phi_cond_b+phi_conv_cof+phi_cond_h+phi_cond_g+phi_conv_air+phi_adv)&
	&+T(Nt*(Nr+1))


!********************* Cas 12 (i=Nt*Nr+1) *********************	(à corriger)
phi_cond_d = 0
phi_cond_b = 0
phi_conv_cof = 0
phi_cond_h = 0
phi_cond_g = 0
phi_conv_air = h(theta)*C_Sn(Nt*Nr+1-Nt)*(T(Nt*Nr+1-Nt)-T(Nt*Nr+1))
phi_adv = m*Cp_air*(T_stock(2*t0)-T(Nt*Nr+1))

T_n(Nt*Nr+1)=dt/masse(Nt*Nr+1)/Cp_air*(phi_cond_d+phi_cond_b+phi_conv_cof+phi_cond_h+phi_cond_g+phi_conv_air+phi_adv)&
	&+T(Nt*Nr+1)


T_stock(2*t0+1)=T_n(Nt*Nr+1)
T_stock(2*t0+2)=T_n(Nt*(Nr+1))

T=T_n


enddo

print*,T_n









contains

!**********************************************************************************************************************
!****************************************CALCULE DES VOLUMES ET SURFACES **************************************
!**********************************************************************************************************************
subroutine Geometrie(e,C_So,C_Se,C_Ss,C_Sn,C_V,A_Se,A_So,A_Ss,A_Sn,A_V,Nt,Nr,delta_r,Ray,L,beta)
implicit none

integer,intent(in)::Nt,Nr
double precision,dimension(:),allocatable,intent(out)::e
double precision,dimension(:),allocatable,intent(out)::C_So,C_Se,C_Ss,C_Sn,C_V
double precision,dimension(:),allocatable,intent(out)::A_Se,A_So,A_Ss,A_Sn,A_V
double precision,intent(in)::Ray,delta_r,L,beta

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
	
	!print*,C_So(i),C_Se(i)
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
	
	
	if (aide_gamma<Nt/2) then
		gamma = aide_gamma*theta
		A_Sn(i) = (beta-tan(gamma)*Ray)*L
		A_Ss(i) = (beta-tan(gamma-theta)*Ray)*L
		A_Se(i)=(beta-tan(gamma)*Ray)*L

		A_V(i) = A_Sn(i)*Ray*(1-cos(gamma))+Ray*(1-cos(gamma))*Ray*sin(gamma)/2*L
		
	else
		gamma = (Nt-aide_gamma)*theta
		A_Ss(i) = (beta-tan(gamma)*Ray)*L
		A_Sn(i) = (beta-tan(gamma-theta)*Ray)*L
		A_Se(i)=(beta-tan(gamma)*Ray)*L

		A_V(i) = A_Ss(i)*Ray*(1-cos(gamma))+Ray*(1-cos(gamma))*Ray*sin(gamma)/2*L
		
	end if
	!print*,A_So(i)
	!print*,'-------------------------------------------------------'
enddo
end subroutine

subroutine Calcule_masse(Nt,Nr,v_air,v_cof,C_V,A_V,masse)
implicit none

integer,intent(in)::Nt,Nr
double precision,dimension(:),intent(in)::C_V,A_V
double precision,dimension(:),allocatable,intent(out)::masse
double precision,intent(in)::v_air,v_cof
integer::i

allocate(masse(Nt*(Nr+1)))

do i=1,Nt*Nr
	masse(i)=v_cof*C_V(i)
end do

do i=Nt*Nr+1,Nt*(Nr+1)
masse(i)=v_air*A_V(i)
end do


end subroutine

function h(theta)
implicit none
double precision::h
double precision,intent (in)::theta
h=50

end function



end program 