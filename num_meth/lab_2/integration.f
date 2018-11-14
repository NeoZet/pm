      module constants
      implicit none
      real*8, parameter  :: lower_limit=0, upper_limit=4
      real*8             :: eps_4 = 0.0001, eps_5 = 0.00001
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

      iter = 0
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
      real*8                :: array_sum
      array_sum = 0
      do i = 0, (arr_size - 2)
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
      real*8                :: sum_by_odd
      real*8                :: sum_by_even
      sum_by_odd  = 0
      sum_by_even = 0
      do i = 1, (arr_size - 2)
         if (mod(i, 2) == 0) then
            sum_by_odd = sum_by_odd + arr(i)
         else
            sum_by_even = sum_by_even + arr(i)
         end if
      end do
      integral=h/3*(arr(0)+4*sum_by_even+2*sum_by_odd+arr(arr_size-1))
      end function integral_by_simpson      
      
      function integral_calc(integr_type, step) result(integral)
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
      end function integral_calc
      
      subroutine check_accuracy(integr_type,eps,integral,accuracy,step)
      use constants
      real*8 :: eps
      real*8 :: step
      real*8 :: integral, accuracy      
      real*8 :: integr_n, integr_o, accur_t
      real*8 :: integral_calc
      real*8 :: step_t
      integer :: integr_type
      integer :: coef
      step_t=step
      if (integr_type == 0) then
         coef = 3
      else
         coef = 15
      end if
      integr_o = integral_calc(integr_type, step_t)
      integr_n = integral_calc(integr_type, step_t/2)
      accur_t=dabs(integr_o-integr_n)/coef
      do while (eps < accur_t)
         step_t = step_t / 10
         integr_n = integral_calc(integr_type, step_t)
         accur_t=dabs(integr_o-integr_n)/coef
         integr_o = integr_n         
      end do
      integral = integr_n
      accuracy = accur_t
      end subroutine check_accuracy

      program main
      use constants
      real*8  :: integrate_function
      real*8  :: integral
      real*8  :: accuracy
      real*8  :: stp
      integer :: integral_type

      stp = 1
      call check_accuracy(0, eps_4, integral, accuracy, stp)
      print *, "TRAPEZIODAL"
      print *, "epsilon:"
      write(*, '(5es16.5e3)') eps_4
      print *, "integral:"
      write(*, '(5es16.5e3)') integral
      print *, "accuracy:"
      write(*, '(5es16.5e3)') accuracy
      print *, "=============================="

      stp = 1
      call check_accuracy(1, eps_4, integral, accuracy, stp)
      print *, "SIMPSON"
      print *, "epsilon:"
      write(*, '(5es16.5e3)') eps_4
      print *, "integral:"
      write(*, '(5es16.5e3)') integral
      print *, "accuracy:"
      write(*, '(5es16.5e3)') accuracy
      print *, "=============================="
      
      stp = 1
      call check_accuracy(0, eps_5, integral, accuracy, stp)
      print *, "TRAPEZIODAL"
      print *, "epsilon:"
      write(*, '(5es16.5e3)') eps_5
      print *, "integral:"
      write(*, '(5es16.5e3)') integral
      print *, "accuracy:"
      write(*, '(5es16.5e3)') accuracy
      print *, "=============================="

      stp = 1
      call check_accuracy(1, eps_5, integral, accuracy, stp)
      print *, "SIMPSON"
      print *, "epsilon:"
      write(*, '(5es16.5e3)') eps_5
      print *, "integral:"
      write(*, '(5es16.5e3)') integral
      print *, "accuracy:"
      write(*, '(5es16.5e3)') accuracy
      
      end program main
