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
  subroutine shikhman()
    integer :: k
    integer :: it
    integer, parameter :: n=2
    real(dp), parameter :: T=1
    real(dp), parameter :: e_dop=0.001
    real(dp), parameter :: tau_max = 0.01
    real(dp), parameter :: tau_min = 0.001
    real(dp) :: x(n), y_k(n), y_k_prev(n), y_k_prev_2(n), y_k_next(n)
    real(dp) :: t_k=0.00000001
    real(dp) :: tau(3)
    real(dp) :: y()
    real(dp) :: tau_k
    real(dp) :: tau_k_next
    real(dp) :: tau_k_prev_2
    real(dp) :: tau_k_prev
    real(dp) :: t_k
    real(dp) :: t_k_next

    tau_k = tau_min
    tau_k_prev = tau_min
    tau_k_prev_2 = tau_min
    tau_k_next = tau_min
    
    k = 0
    it = 0
    t_k = 0.00000001
    
    do while (k < 2)
       t_k_next = t_k + tau_k
       y_k_next = newton(f_temp, f, y_k_next, t_k_next, y_k, tau_k)
    end do
    
    
    

  end subroutine shikhman
  


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
  function func(y_k, y_k_next, t, tau)
    integer, parameter :: n=2
    real(dp), intent(in) :: f(n)
    do k=1,n
       f(k) = y_k_next - y_k - tau * f(y_k_next, t)
    end do
    
    
  end function func

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


  !*******************************************************************************
  function newton()
    integer :: i, imax=500
    integer, parameter :: n=2
    real(dp) :: x(n), x_prev(n), f(n), dx(n), df(n,n)
    real(dp) :: tol=1.0e-9_dp
    real(dp) :: newton(n)
    !-----initial guess
    x=([1.0_dp, 0.0_dp])
    write(*,*) "Nonlinear equations by Newton's method"
    do i=1,imax
       f=func(x)
       df=jacobian(x)
       call gauss(df,-f,dx,n)
       x_prev = x
       x=x+dx
       write(*,*) delta_1(x), delta_2(x_prev, x)
       if(sqrt(dx(1)**2+dx(2)**2)<tol) then
          newton = x(n)
          exit
       end if
    end do
  end function newton

  !******************************************************
  function delta_1(x)
    integer, parameter :: n=2
    real(dp), intent(in) :: x(n)
    real(dp) :: delta_1
    delta_1 = maxval(abs(func(x)))
  end function delta_1

  !******************************************************
  function delta_2(x, x_next)
    integer, parameter :: n=2
    real(dp), intent(in) :: x(n)
    real(dp), intent(in) :: x_next(n)
    real(dp) :: delta_2
    delta_2 = maxval(abs(x_next - x))
  end function delta_2


  !******************************************************
  function jacobian(x)
    implicit none 
    integer :: i
    integer, parameter :: n=2
    real(dp) :: x(n)
    real(dp) :: f1(n), f2(n), h, temp
    real(dp) :: jacobian(n,n)
    h=1.0e-4_dp
    f1=func(x)
    do i=1,n
       temp=x(i)
       x(i)=temp+h
       f2=func(x)
       x(i)=temp
       jacobian(:,i)=(f2-f1)/h
    end do
  end function jacobian

  !*******************************************************************************
  subroutine gauss(a,b,x,n)
    !  Gauss elimination
    implicit none 
    integer :: i,k,idmax
    integer, intent(in) :: n
    real(dp), dimension(1:n,1:n), intent(in) :: a
    real(dp), dimension(1:n), intent(in) :: b
    real(dp), dimension(1:n), intent(out) :: x
    real(dp), dimension(1:n,1:n+1) :: ab
    real(dp), dimension(1:n,1:n) :: aup
    real(dp), dimension(1:n) :: bup
    real(dp), dimension(1:n+1) :: temp1, temp2
    real(dp) :: lam, elmax
    ab(1:n,1:n)=a
    ab(:,n+1)=b
    do k=1,n-1
       elmax=abs(ab(k,k))
       idmax=k
       ! maximum element in the column
       do i=k+1,n
          if(abs(ab(i,k))>elmax) then
             elmax=ab(i,k)
             idmax=i
          end if
       end do
       !  swap two lines
       temp1=ab(k,:)
       temp2=ab(idmax,:)
       ab(k,:)=temp2
       ab(idmax,:)=temp1
       do i=k+1,n  
          lam=ab(i,k)/ab(k,k)
          ab(i,:)=ab(i,:)-lam*ab(k,:)
       end do
    end do
    aup(:,:)=ab(1:n,1:n)
    bup(:)=ab(:,n+1)
    call uptri(aup,bup,x,n)
  end subroutine gauss

  !*******************************************************************************
  subroutine uptri(a,b,x,n)
    implicit none 
    integer :: i,k
    integer, intent(in) :: n
    real(dp), dimension(1:n,1:n), intent(in) :: a
    real(dp), dimension(1:n), intent(in) :: b
    real(dp), dimension(1:n), intent(out) :: x
    real(dp) :: s
    x(n)=b(n)/a(n,n)
    do i=n-1,1,-1
       s=0.0_dp
       do k=i+1,n
          s=s+a(i,k)*x(k)
       end do
       x(i)=(b(i)-s)/a(i,i)
    end do
  end subroutine uptri
  !*******************************************************************************
end module nonlinear
!*******************************************************************************

program main
  use kinds
  use nonlinear
  implicit none 
  call shikhman()
end program main
