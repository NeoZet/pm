module kinds
  implicit none
  integer, parameter :: dp=kind(0.d0)  !  double precision
end module kinds
!*******************************************************************************

module nonlinear
  use kinds
  implicit none
  private 
  public :: euler 
contains

  !*******************************************************************************
  subroutine euler()
    integer :: k=0
    integer, parameter :: n=2
    real(dp), parameter :: T=1
    real(dp) :: x(n), y_k(n), f_vec(n)
    real(dp) :: t_k=0.00000001
    real(dp) :: tau_k

    y_k(1) = 0
    y_k(2) = -0.412

    do while(t_k < T)
       f_vec = f(y_k, t_k)
       tau_k = step(f_vec)
       y_k = y_k + tau_k * f_vec
       t_k = t_k + tau_k
       print*,
       write(*,*) "K: ", k
       print*, "Solution: ["
       write(*,*) "U1 = ",y_k(1)
       write(*,*) "U2 = ",y_k(2)
       print*, "]"
       write(*,*) "T: ", t_k
       print*, "---------------------------"
       k = k + 1
    end do
    
  end subroutine euler

  !******************************************************
  function step(f)
    integer :: i
    integer, parameter :: n=2
    real(dp), parameter :: e_dop = 0.00001
    real(dp), parameter :: tau_max = 0.01
    real(dp), intent(in) :: f(n)
    real(dp) :: tau(n)
    real(dp) :: step

    do i=1,n
       tau(i) = e_dop / abs(f(i)) + e_dop / tau_max
    end do
    step = minval(tau)
  end function step

  !******************************************************
  function f(u,t)
    !  input function
    implicit none
    integer, parameter :: n=2
    integer, parameter :: Om=40
    real(dp) :: a
    real(dp), intent(in) :: u(n)
    real(dp), intent(in) :: t
    real(dp) :: f(n)

    a = 2.5
    a = a + Om/40

    ! f(1) = -u(1)*u(2) + sin(t) / t
    ! f(2) = -(u(2) * u(2)) + a*t/(1+t*t)

    f(1) = -2 * u(1) + 4 * u(2)
    f(2) = -u(1) + 3 * u(2)
 
  end function f

end module nonlinear
!*******************************************************************************

program main
  use kinds
  use nonlinear
  implicit none 
  call euler()
end program main
