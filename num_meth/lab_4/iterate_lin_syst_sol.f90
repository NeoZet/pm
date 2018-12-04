module  Constants
  implicit none
  real(8), parameter :: eps = 0.00001
end module Constants

program  Main
  implicit none
  integer :: i, j, N, Nmax, NUM_ITERATIONS
  real(8), allocatable :: matrix(:,:), extens(:), sol(:), resid(:)
  character(len=20) :: FNAME
  real :: norm

  Nmax = 50
  FNAME = 'sf'
  open(10, file=FNAME,action='read')
  read(10,*) N
  allocate(matrix(N,N), extens(N), sol(N))
  do i=1, N
     read(10,*) (matrix(i,j), j=1, N), extens(i)
  end do

  write(6,*) ' Jacobi Method '
  write(6,*) '*********************'
  call  Jacobi(N, Nmax, matrix, extens, sol, NUM_ITERATIONS)
  call  Show_Result(N, matrix, extens, sol, NUM_ITERATIONS)
  resid = matmul(matrix, sol) - extens
  norm = maxval(abs(resid))
  write(*, '(a,5es16.5e3)') 'resid:', resid
  write(*, '(a,5es16.5e3)') 'resid norm:', norm
  write(6,*)
  write(6,*) ' Gauss-Seidel Method '
  write(6,*) '*********************'
  call  Gauss_Seidel(N, Nmax, matrix, extens, sol, NUM_ITERATIONS)
  call  Show_Result(N, matrix, extens, sol, NUM_ITERATIONS)
  resid = matmul(matrix, sol) - extens
  norm = maxval(abs(resid))
  write(*, '(a,5es16.5e3)') 'resid:', resid
  write(*, '(a,5es16.5e3)') 'resid norm:', norm
  
  close(10)

  stop
end program Main

!====================================================================!
subroutine  Jacobi(N, Nmax, matrix, extens, sol, NUM_ITERATIONS)
  use  Constants
  implicit none
  integer :: i, j, k
  integer, intent(in) :: N, Nmax
  integer, intent(out) :: NUM_ITERATIONS
  real*8, intent(in) :: matrix(N,N)
  real*8, intent(out) :: sol(N)
  real*8, intent(in) :: extens(N)
  real*8 :: sol_prev(N), accur, r

  do i=1, N
     sol(i) = 1.0
  end do
  
  NUM_ITERATIONS = 0
  accur = eps + 1
  do k=1, Nmax
     NUM_ITERATIONS = NUM_ITERATIONS + 1
     sol_prev = sol
     do i=1, N
        r = extens(i)
        do j=1, N
           if (j == i) cycle
           r = r - matrix(i,j) * sol_prev(j)
        end do
        sol(i) = r / matrix(i,i)        
     end do
     accur = maxval(abs(sol - sol_prev)) / maxval(abs(sol))
     if (accur < eps) return
  end do
  stop 'Iteration did not converge.'
  return
end subroutine Jacobi

!====================================================================!
subroutine  Gauss_Seidel(N, Nmax, matrix, extens, sol, NUM_ITERATIONS)
  use  Constants
  implicit none
  integer, intent(in) :: N, Nmax
  integer, intent(out) :: NUM_ITERATIONS
  integer :: i, j, k
  real(8), intent(inout) :: sol(N)
  real(8), intent(in) :: matrix(N,N), extens(N)
  real(8) :: accur, r, sol_prev(N)
  do i=1, N
     sol(i) = 1.0
  end do

  NUM_ITERATIONS = 0
  accur = eps + 1
  do k=1, Nmax
     NUM_ITERATIONS = NUM_ITERATIONS + 1
     do i=1, N
        sol_prev = sol
        r = extens(i) - dot_product(matrix(i,1:N),sol(1:N))
        sol(i) = sol(i) + r / matrix(i,i)
     end do
     accur = maxval(abs(sol - sol_prev)) / maxval(abs(sol))
     if (accur < eps) return
  end do
  stop 'Iteration did not converge.'
  return
end subroutine Gauss_Seidel


subroutine  Show_Result(N, matrix, extens, sol, NUM_ITERATIONS)
  implicit none
  integer, intent(in) :: N, NUM_ITERATIONS
  integer :: i, j
  real(8), intent(in) :: matrix(N,N), extens(N), sol(N)
  character(len=30) :: FM2='(x,a,i1,a,f11.7)'
  character(len=30) :: FM ='(x,a,i3,a)'

  write(6,FM) 'Iteration :', NUM_ITERATIONS, ' times'
  write(6,*)
  do i=1, N
     write(6,FM2) 'x(',i,') =', sol(i)
  end do
  write(6,*) 

  return
end subroutine Show_Result
