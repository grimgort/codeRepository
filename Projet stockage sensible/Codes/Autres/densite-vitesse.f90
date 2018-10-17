!****************************************************************************
! *************************Proprietes de l'air à 70 bar*************************

subroutine rho_air70 (T,rho_air70)
double precision, intent(in) :: T
double precision, intent(out) :: rho_air70

rho_air70 =-6*10**(-8.)*T**3 + 7*10**(-5.)*T**2 - 0,0298*T + 8,9153
end subroutine


subroutine Cp_air70(T,Cp_air70)
double precision, intent(in) :: T
double precision, intent(out) :: Cp_air70

Cp_air70 = -8*10**(-10.)*T**3 + 9E*10**(-7.)*T**2 - 9*10**(-5.)*T + 1,0193
end subroutine


subroutine Pr70(T,Pr70)
double precision, intent(in) :: T
double precision, intent(out) :: Pr70

Pr70 = 5*10**(-13.)*T**4 - 1*10**(-9.)*T**3 + 8*10**(-7)*T**2 - 0,0002*T + 0,7181
end subroutine


subroutine mu_air70(T,mu_air70)
double precision, intnent(in) :: T
double precision, intent(out) :: mu_air70

mu_air70 = 8*10**(-15.)*T**3 - 2*10**(-11.)*T**2 + 5*10**(-8.)*T + 2*10**(-5.)
end subroutine




!*************************Proprietes de l'air à 4 bar*************************

subroutine Cp_air4(T,Cp_air4)
double precision, intent(in) :: T
double precision, intent(out) :: Cp_air4

Cp_air4 = 2,955*10**(-13.)*T**4 - 8,39*10**(-10.)*T**3 + 7,652*10**(-7.)*T**2 - &
 4,878*10**(-5.)*T + 1,0127
end subroutine


subroutine rho_air4 (T,rho_air4)
double precision, intent(in) :: T
double precision, intent(out) :: rho_air4

rho_air4 =-1,601*10**(-14.)*T**5 + 5,069*10**(-11.)*T**4 - 6,391*10**(-8.)*T**3 + &
4,227*10**(-5.)*T**2 - 0,017*T + 5,0841
end subroutine


subroutine Pr4(T,Pr4)
double precision, intent(in) :: T
double precision, intent(out) :: Pr4

Pr4 = 2,468*10**(-13.)*T**4 - 6,865*10**(-10.)*T**3 + 6,447*10**(-7.)*T**2 - &
0,0002*T + 0,7144
end subroutine


subroutine mu_air4(T,mu_air4)
double precision, intnent(in) :: T
double precision, intent(out) :: mu_air4

mu_air4 = 7,823*10**(-15.)*T**3 - 2,193*10**(-11.)*T**2 + 4,745*10**(-8.)*T + &
1,736*10**(-5.)
end subroutine






!*************************Proprietes de l'air à 27 bar*************************

subroutine Cp_air27(T,Cp_air4)
double precision, intent(in) :: T
double precision, intent(out) :: Cp_air27

Cp_air27 = -7,338*10**(-16.)*T**5 + 2,469*10**(-12.)*T**4 - 3,304*10**(-9.)*T**3 + &
2,119*10**(-6.)*T**2 - 0,0004*T + 1,0637
end subroutine


subroutine rho_air27 (T,rho_air27)
double precision, intent(in) :: T
double precision, intent(out) :: rho_air27

rho_air27 =-1,287*10**(-13.)*T**5 + 4,024*10**(-10.)*T**4 - 4,972*10**(-7.)*T**3 + &
0,0003*T**2 - 0,1223*T + 34,686
end subroutine


subroutine Pr27(T,Pr27)
double precision, intent(in) :: T
double precision, intent(out) :: Pr27

Pr27 =  - 1,913*10**(-15)*T**5 + 3,154*10**(-12.)*T**4 - 62,914*10**(-9.)*T**3 +&
 1,566*10**(-6.)*T**2 - 0,0004*T + 0,74
end subroutine


subroutine mu_air27(T,mu_air27)
double precision, intnent(in) :: T
double precision, intent(out) :: mu_air27

mu_air27 = 7,26*10**(-15.)*T**3 - 2,07*10**(-11.)*T**2 + 4,649*10**(-8.)*T + &
1,776*10**(-5.)
end subroutine





!****************** tableaux vitesses******************************


subroutine vitesse(rho_air,A_Ss,A_Sn,debit,v)
double precision, intent(in):: rho_air , debit
double precision, dimension(:),intent(in) :: A_Ss , A_Sn 
double precision, dimension(:), allocatable, intent(out) :: v

double precision, dimension(:), allocatable :: Sm
integer :: n , i

n=size(A_Ss)

allocate(Sm(n))
allocate(v(n))

do i=1,n
	Sm(i)=(A_Ss(i) + A_Sn(i))/2

	v(i)=debit/rho_air/Sm(i)
enddo

end subroutine