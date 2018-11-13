c$$$      module constants
c$$$      implicit none
c$$$      real*8, parameter  :: lower_limit=0, upper_limit=4
c$$$      real*8             :: start_step=1
c$$$      real*8             :: eps_4 = 0.0001, eps_5 = 0.00001
c$$$      integer, parameter :: MAX_ARR_SIZE = 1024 * 1024 * 8
c$$$      end module constants
c$$$      
c$$$      function integrate_function(x) result(res)
c$$$      real*8, intent(in) :: x
c$$$      real*8             :: res
c$$$      res = 1.0/(1 + x**(0.5))
c$$$      end function integrate_function
c$$$      
c$$$      
c$$$      subroutine fill_arrays(x_array, y_array, arr_size, step)
c$$$      use constants
c$$$      real*8, intent(in) :: step
c$$$      real*8             :: integrate_function
c$$$      real*8             :: x_array(0:MAX_ARR_SIZE)
c$$$      real*8             :: y_array(0:MAX_ARR_SIZE)
c$$$      real*8             :: value
c$$$      integer            :: iter, arr_size
c$$$      iter  = 0
c$$$      value = lower_limit      
c$$$      do while (value <= upper_limit .and. iter < MAX_ARR_SIZE-1)
c$$$        x_array(iter) = value
c$$$        y_array(iter) = integrate_function(value)
c$$$        value = value + step
c$$$        iter = iter + 1
c$$$      end do
c$$$      arr_size = iter
c$$$      end subroutine fill_arrays
c$$$
c$$$      function integral_by_trapezoid(array,arr_size,h) result(integral)
c$$$      use constants      
c$$$      integer, intent(in)   :: arr_size      
c$$$      real*8, intent(in)    :: h
c$$$      real*8                :: integral
c$$$      real*8                :: array(0:MAX_ARR_SIZE)
c$$$      real*8                :: array_sum=0
c$$$      do i = 1, (arr_size - 2)         
c$$$        array_sum = array_sum + array(i)
c$$$      end do      
c$$$      integral = h/2 * (array(0) + 2*array_sum + array(arr_size-1))
c$$$      end function integral_by_trapezoid
c$$$
c$$$      function integral_by_simpson(arr,arr_size,h) result(integral)
c$$$      use constants      
c$$$      integer, intent(in)   :: arr_size      
c$$$      real*8, intent(in)    :: h
c$$$      real*8                :: integral
c$$$      real*8                :: arr(0:MAX_ARR_SIZE)
c$$$      real*8                :: sum_by_odd=0
c$$$      real*8                :: sum_by_even=0
c$$$      do i = 1, (arr_size - 2)
c$$$         if (mod(i, 2) == 0) then
c$$$            sum_by_odd = sum_by_odd + arr(i)
c$$$         else
c$$$            sum_by_even = sum_by_even + arr(i)
c$$$         end if
c$$$      end do      
c$$$      integral=h/3*(arr(0)+4*sum_by_even+2*sum_by_odd+arr(arr_size-1))
c$$$      end function integral_by_simpson
c$$$      
c$$$      function integral_calc(integral_type, step) result(integral)
c$$$      use constants
c$$$      real*8    :: integral
c$$$      real*8    :: x_list(0:MAX_ARR_SIZE)
c$$$      real*8    :: y_list(0:MAX_ARR_SIZE)
c$$$      real*8    :: h, step
c$$$      real*8    :: integral_by_trapezoid
c$$$      real*8    :: integral_by_simpson
c$$$      integer :: list_size
c$$$      integer :: integral_type
c$$$      call fill_arrays(x_list, y_list, list_size, step)
c$$$      h = (upper_limit - lower_limit) / list_size
c$$$      if (integral_type == 0) then
c$$$         integral = integral_by_trapezoid(y_list, list_size, h)
c$$$      else
c$$$         integral = integral_by_simpson(y_list, list_size, h)
c$$$      end if
c$$$      end function integral_calc
c$$$      
c$$$      subroutine check_accuracy(integr_type,eps,integral,accuracy,step)
c$$$      use constants
c$$$      real*8 :: step
c$$$      real*8 :: eps
c$$$      real*8 :: integral, accuracy      
c$$$      real*8 :: integr_n, integr_o, accur_t
c$$$      real*8 :: integral_calc
c$$$      real*8 :: step_t
c$$$      real   :: coef
c$$$      step_t=step
c$$$      if (integr_type == 0) then
c$$$         coef = 3
c$$$      else
c$$$         coef = 15
c$$$      end if      
c$$$      step_t = step
c$$$      integr_o = integral_calc(integr_type,step_t)
c$$$      integr_n = integral_calc(integr_type,step_t/2)
c$$$      accur_t=dabs(integr_n-integr_o)/coef
c$$$      integr_o = integr_n
c$$$      do while (eps < accur_t)
c$$$         step_t = step_t / 10
c$$$         write(*, '(5es16.4e3)') accur_t
c$$$         integr_n = integral_calc(integr_type,step_t)
c$$$         accur_t=dabs(integr_n-integr_o)/coef
c$$$         integr_o=integr_n
c$$$      end do
c$$$      integral = integr_n
c$$$      accuracy = accur_t
c$$$      end subroutine check_accuracy
c$$$
c$$$      program main
c$$$      use constants
c$$$      real*8 :: integrate_function
c$$$      real*8 :: integral
c$$$      real*8 :: accuracy
c$$$      real*8 :: stp=1
c$$$!     0 - trapezoidal
c$$$!     1 - simpson     
c$$$      integer:: integral_type=0
c$$$
c$$$      integral_type = 0
c$$$      call check_accuracy(integral_type, eps_5,integral,accuracy,stp)
c$$$      write(*, '(5es16.4e3)') eps_4
c$$$      print*, "integral",integral,"accuracy",accuracy
c$$$      write(*, '(5es16.4e3)') integral
c$$$      write(*, '(5es16.4e3)') accuracy
c$$$      
c$$$c$$$      integral_type = 1
c$$$c$$$      call check_accuracy(integr_type, eps_5,integral,accuracy,stp)
c$$$c$$$      write(*, '(5es16.4e3)') eps_5
c$$$c$$$      print*, "integral",integral,"accuracy",accuracy
c$$$c$$$      write(*, '(5es16.4e3)') integral
c$$$c$$$      write(*, '(5es16.4e3)') accuracy
c$$$      end program main
c$$$ 
      module constants
      implicit none
      real*8, parameter  :: lower_limit=0, upper_limit=4
      real*8             :: start_step=1
      real*8             :: eps_4 = 0.0001, eps_5 = 10**(-5)
      integer, parameter :: MAX_ARR_SIZE = 1024 * 1024 * 8
      end module constants
     
      
      function integrate_function(x) result(res)
      real*8, intent(in) :: x
      real*8             :: res
      res = 1.0/(1 + x**(0.5))
      end function integrate_function
      
      
      subroutine fill_arrays(x_array, y_array, arr_size, step)
      use constants
      real*8, intent(in) :: step
      real*8             :: integrate_function
      real*8             :: x_array(0:MAX_ARR_SIZE)
      real*8             :: y_array(0:MAX_ARR_SIZE)
      real*8             :: value
      integer            :: iter, arr_size
      
      iter  = 0
      value = lower_limit
      
      do while (value <= upper_limit .and. iter < MAX_ARR_SIZE-1)
        x_array(iter) = value
        y_array(iter) = integrate_function(value)
        value = value + step
        iter = iter + 1
      end do
      arr_size = iter
      end subroutine fill_arrays

      function integral_by_trapezoid(array,arr_size,h) result(integral)
      use constants      
      integer, intent(in)   :: arr_size      
      real*8, intent(in)    :: h
      real*8                :: integral
      real*8                :: array(0:MAX_ARR_SIZE)
      real*8                :: array_sum=0
      array_sum = 0
      integral  = 0
      do i = 0, arr_size-2
        array_sum = array_sum + array(i)
      end do      
      integral = h/2 * (array(0) + 2*array_sum + array(arr_size-1))
      end function integral_by_trapezoid

      function integral_by_simpson(arr,arr_size,h) result(integral)
      use constants      
      integer, intent(in)   :: arr_size      
      real*8, intent(in)    :: h
      real*8                :: integral
      real*8                :: arr(0:MAX_ARR_SIZE)
      real*8                :: sum_by_odd=0
      real*8                :: sum_by_even=0
      do i = 1, (arr_size - 2)
         if (mod(i, 2) == 0) then
            sum_by_odd = sum_by_odd + arr(i)
         else
            sum_by_even = sum_by_even + arr(i)
         end if
      end do      
      integral=h/3*(arr(0)+4*sum_by_even+2*sum_by_odd+arr(arr_size-1))
      end function integral_by_simpson      
      
      function integral_calculation(integr_type, step) result(integral)
      use constants
      real*8    :: integral
      real*8    :: x_list(0:MAX_ARR_SIZE)
      real*8    :: y_list(0:MAX_ARR_SIZE)
      real*8    :: h, step
      real*8    :: integral_by_trapezoid
      real*8    :: integral_by_simpson
      integer :: list_size
      call fill_arrays(x_list, y_list, list_size, step)
      h = (x_list(list_size-1) - x_list(0)) / list_size
      if (integral_type == 0) then
         integral = integral_by_trapezoid(y_list, list_size, h)
      else
         integral = integral_by_simpson(y_list, list_size, h)
      end if
      end function integral_calculation
      
      subroutine check_accuracy(integral, accuracy, step)
      use constants
      real*8 :: step
      real*8 :: integral, accuracy      
      real*8 :: integr_n, integr_o, accur_t
      real*8 :: integral_calculation
      real*8 :: step_t
      integer :: t=0
      step_t=step
      step_t = step
      integr_o = integral_calculation(integr_type,step_t)
      integr_n = integral_calculation(integr_type,step_t/2)
      accur_t=dabs(integr_n-integr_o)/coef
      integr_o = integr_n
      do while (eps < accur_t)
         step_t = step_t / 2
         integr_n = integral_calculation(integr_type,step_t)
         accur_t=dabs(integr_n-integr_o)/coef
         integr_o=integr_n
      end do
      integral = integr_n
      accuracy = accur_t
      end subroutine check_accuracy

      program main
      use constants
      real*8 :: integrate_function
      real*8 :: integral
      real*8 :: accuracy
      real*8 :: stp=1
      write(*, '(5es16.4e3)') eps_4
      call check_accuracy(integral, accuracy, stp)
      print*, "integral",integral,"accuracy",accuracy
      write(*, '(5es16.4e3)') integral
      write(*, '(5es16.4e3)') accuracy
      end program main
